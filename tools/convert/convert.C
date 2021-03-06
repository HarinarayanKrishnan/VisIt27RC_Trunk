/*****************************************************************************
*
* Copyright (c) 2000 - 2013, Lawrence Livermore National Security, LLC
* Produced at the Lawrence Livermore National Laboratory
* LLNL-CODE-442911
* All rights reserved.
*
* This file is  part of VisIt. For  details, see https://visit.llnl.gov/.  The
* full copyright notice is contained in the file COPYRIGHT located at the root
* of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
*
* Redistribution  and  use  in  source  and  binary  forms,  with  or  without
* modification, are permitted provided that the following conditions are met:
*
*  - Redistributions of  source code must  retain the above  copyright notice,
*    this list of conditions and the disclaimer below.
*  - Redistributions in binary form must reproduce the above copyright notice,
*    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
*    documentation and/or other materials provided with the distribution.
*  - Neither the name of  the LLNS/LLNL nor the names of  its contributors may
*    be used to endorse or promote products derived from this software without
*    specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
* ARE  DISCLAIMED. IN  NO EVENT  SHALL LAWRENCE  LIVERMORE NATIONAL  SECURITY,
* LLC, THE  U.S.  DEPARTMENT OF  ENERGY  OR  CONTRIBUTORS BE  LIABLE  FOR  ANY
* DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,  OR   CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
* SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
* CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
* LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
* OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*
*****************************************************************************/

// ************************************************************************* //
//                                  convert.C                                //
// ************************************************************************* //

#include <DatabasePluginManager.h>
#include <DBPluginInfoAttributes.h>
#include <FileOpenOptions.h>

#include <DatabasePluginInfo.h>
#include <VisItInit.h>

#ifdef DBIO_ONLY
// Disable expressions during the VISIT_DBIO_ONLY build mode.
#define DISABLE_EXPRESSIONS
#endif

#ifndef DISABLE_EXPRESSIONS
#include <ExprParser.h>
#include <ParsingExprList.h>
#endif
#include <Utility.h>

#include <avtDatabase.h>
#include <avtDatabaseFactory.h>
#include <avtDatabaseMetaData.h>
#include <avtDatabaseWriter.h>
#ifndef DISABLE_EXPRESSIONS
#include <avtExpressionEvaluatorFilter.h>
#include <avtExprNodeFactory.h>
#endif
#include <avtParallel.h>
#include <DBOptionsAttributes.h>

#include <VisItException.h>
#include <visitstream.h>

#include <string>
#include <vector>

using std::cin;
using std::string;
using std::vector;

static void UsageAndExit(DatabasePluginManager *, const char *);

// ****************************************************************************
//  Method:  FillOptionsFromCommandline
//
//  Purpose:
//    Fill database writer options from the user interactively.
//
//  Arguments:
//    opts       the options to fill
//
//  Programmer:  Jeremy Meredith
//  Creation:    June  3, 2008
//
//  Modifications:
//    Eric Brugger, Fri Aug  8 12:22:32 PDT 2008
//    Made to use strtod instead of strtof if on Windows or HAVE_STRTOF is
//    not defined or HAVE_STRTOF_PROTOTYPE is not defined.
//
//    Hank Childs, Thu Nov  5 17:17:35 PST 2009
//    Only have proc 0 print out error messages.
//
// ****************************************************************************
void
FillOptionsFromCommandline(DBOptionsAttributes *opts)
{
    if (!opts)
        return;

    for (int j=0; j<opts->GetNumberOfOptions(); j++)
    {
        const string &name = opts->GetName(j);
        if (PAR_Rank() == 0)
            cerr << endl << "Enter value for option '"<<name<<"'";
        switch (opts->GetType(j))
        {
          case DBOptionsAttributes::Bool:
            if (PAR_Rank() == 0)
                cerr << " (boolean, default="<<opts->GetBool(name)<<"):\n";
            break;
          case DBOptionsAttributes::Int:
            if (PAR_Rank() == 0)
                cerr << " (integer, default="<<opts->GetInt(name)<<"):\n";
            break;
          case DBOptionsAttributes::Float:
            if (PAR_Rank() == 0)
                cerr << " (float, default="<<opts->GetFloat(name)<<"):\n";
            break;
          case DBOptionsAttributes::Double:
            if (PAR_Rank() == 0)
                cerr << " (double, default="<<opts->GetDouble(name)<<"):\n";
            break;
          case DBOptionsAttributes::String:
            if (PAR_Rank() == 0)
                cerr << " (string, default='"<<opts->GetString(name)<<"'):\n";
            break;
          case DBOptionsAttributes::Enum:
            {
            if (PAR_Rank() == 0)
                cerr << endl;
            stringVector enumValues = opts->GetEnumStrings(name);
            if (PAR_Rank() == 0)
                for (int k=0; k<enumValues.size(); k++)
                    cerr << "    ("<<k<<" = "<<enumValues[k]<<")\n";
            if (PAR_Rank() == 0)
                cerr << " (integer, default="<<opts->GetEnum(name)<<"):\n";
            }
            break;
        }
        char buff[1024];
        if (PAR_Rank() == 0)
            cin.getline(buff,1024);
        string str = buff;
        BroadcastString(str, PAR_Rank());
        if (PAR_Rank() != 0)
           strcpy(buff, str.c_str());
        if (strlen(buff) == 0)
        {
            if (PAR_Rank() == 0)
                cerr << "Accepted default value for '"<<name<<"'\n";
            continue;
        }
        switch (opts->GetType(j))
        {
          case DBOptionsAttributes::Bool:
            opts->SetBool(name, strtol(buff, NULL, 10));
            if (PAR_Rank() == 0)
                cerr << "Set to new value "<<opts->GetBool(name) << endl;
            break;
          case DBOptionsAttributes::Int:
            opts->SetInt(name, strtol(buff, NULL, 10));
            if (PAR_Rank() == 0)
                cerr << "Set to new value "<<opts->GetInt(name) << endl;
            break;
          case DBOptionsAttributes::Float:
#if defined(_WIN32) || !defined(HAVE_STRTOF) || !defined(HAVE_STRTOF_PROTOTYPE)
            opts->SetFloat(name, (float) strtod(buff, NULL));
#else
            opts->SetFloat(name, strtof(buff, NULL));
#endif
            if (PAR_Rank() == 0)
                cerr << "Set to new value "<<opts->GetFloat(name) << endl;
            break;
          case DBOptionsAttributes::Double:
            opts->SetDouble(name, strtod(buff, NULL));
            if (PAR_Rank() == 0)
                cerr << "Set to new value "<<opts->GetDouble(name) << endl;
            break;
          case DBOptionsAttributes::String:
            opts->SetString(name, buff);            
            if (PAR_Rank() == 0)
                cerr << "Set to new value "<<opts->GetString(name) << endl;
            break;
          case DBOptionsAttributes::Enum:
            opts->SetEnum(name, strtol(buff, NULL, 10));
            if (PAR_Rank() == 0)
                cerr << "Set to new value "<<opts->GetEnum(name) << endl;
            break;
        }
    }
    opts->SelectAll();
}

// ****************************************************************************
//  Function: GetPluginInfo
//
//  Programmer: Mark C. Miller
//  Creation:   May 19, 2009 
//
//  Modifications:
//
//    Hank Childs, Thu Nov  5 17:17:35 PST 2009
//    Only have proc 0 print out error messages.
//
// ****************************************************************************

static EngineDatabasePluginInfo *
GetPluginInfo(DatabasePluginManager *dbmgr, const char *arg0, const char *plugin)
{
    //
    // Users want to enter formats like "Silo", not "Silo_1.0".  Make that
    // conversion for them now.
    //
    EngineDatabasePluginInfo *edpi = NULL;
    int index = dbmgr->GetAllIndexFromName(plugin);
    if (index >= 0)
    {
        string id = dbmgr->GetAllID(index);
        if (dbmgr->PluginAvailable(id))
        {
            edpi = dbmgr->GetEnginePluginInfo(id);
        }
    }

    if (edpi == NULL)
    {
        if (PAR_Rank() == 0)
            cerr << "Was not able to identify file type " << plugin <<"\n\n"<<endl;
        UsageAndExit(dbmgr, arg0);
    }

    return edpi;
}

// ****************************************************************************
//  Function: HandleReadOptions
//
//  Purpose: Set read options for the input database.
//
//  Notes: As for write options, the logic here is designed such that read
//  options will ALWAYS be processed in that they well be obtained via 
//  GetReadOptions and then set via
//  avtDatabaseFactory::SetDefaultFileOpenOptions. The noOptions bool controls
//  only whether they are queried from the user, interactively. I believe this
//  design allows for the case where a user's config file settings contain
//  the desired read options.
//
//  Programmer: Mark C. Miller
//  Creation:   May 19, 2009 
//
//  Modifications:
//    Jeremy Meredith, Wed Dec 30 15:04:36 EST 2009
//    We made the "assumed format" in the database factory deprecated.
//    However, it was simple to leave the command-line option in place
//    for convert as a special case, so the argument is simply passed in now.
//
// ****************************************************************************

static void
HandleReadOptions(bool noOptions, DatabasePluginManager *dbmgr,
                  const char *arg0, string assumedFormat)
{
    if (assumedFormat == "")
        return;

    EngineDatabasePluginInfo *edpir = GetPluginInfo(dbmgr, arg0,
                                                    assumedFormat.c_str());
    DBOptionsAttributes *opts = edpir->GetReadOptions();

    // Don't query user for options if they specifically asked NOT to be.
    if (!noOptions)
        FillOptionsFromCommandline(opts);

    // One would think you could just now call the method to set
    // read options for the particular plugin info we've obtained
    // and be done with it.  However, it doesn't work like that.
    // The resulting read options never make it down to the file format
    // objects actually instantiated to read the input database. So,
    // instead, we follow the paradigm in ViewerFileServer::UpdateDBPluginInfo()
    // which is known to work but requires FileOpenOptions and
    // DBPluginInfoAttributes.

    // edpir->SetReadOptions(opts); can't do it this way

    // FileOpenOptions is really a series of buckets of open options, one for
    // each database plugin. DBPluginInfoOptions is also a series of buckets
    // of information about each DB plugin. In this case, we create a
    // DBPluginInfoOptions 'list' of size one, containing only the options
    // for the specific database we are reading from. When this is 'merged'
    // into FileOpenOptions, which is a list of ALL database plugins, it
    // 'does the right thing' and affects to options for the correct
    // plugin we are aiming to open the input database with.

    // The logic below is basically the paradigm in
    // ViewerFileServer::UpdateDBPluginInfo().
    FileOpenOptions *fileOpenOptions = new FileOpenOptions;
    DBPluginInfoAttributes *dbpia = new DBPluginInfoAttributes;
    dbpia->AddDbReadOptions(*opts);

    vector<string> types(1);
    vector<string> fullnames(1);
    vector<int>         hasWriter(1);
    types[0] = edpir->GetName();
    fullnames[0] = edpir->GetID();
    hasWriter[0] = edpir->HasWriter();
    dbpia->SetTypes(types);
    dbpia->SetTypesFullNames(fullnames);
    dbpia->SetHasWriter(hasWriter);

    fileOpenOptions->MergeNewFromPluginInfo(dbpia);
    avtDatabaseFactory::SetDefaultFileOpenOptions(*fileOpenOptions);
}

// ****************************************************************************
//  Function: main
//
//  Purpose:
//      This program reads in files and then writes them out to a specified
//      format.
//
//  Programmer: Hank Childs
//  Creation:   September 9, 2003
//
//  Modifications:
//
//    Hank Childs, Mon Mar  1 09:08:11 PST 2004
//    Send timestep to database factory.
//
//    Hank Childs, Sat Sep 11 12:10:53 PDT 2004
//    Add support for target chunks, target zones, -variable.
//
//    Jeremy Meredith, Wed Nov 24 15:39:02 PST 2004
//    Added expression support back in.  ('3769)
//
//    Hank Childs, Wed Dec 22 11:33:30 PST 2004
//    Make expressions work ('5701), also add better error handling ('5771).
//
//    Jeremy Meredith, Tue Feb 22 18:16:46 PST 2005
//    Added better argument error checking.  Added ability to turn off
//    expression conversion.  Added ability to turn off MIR even when
//    the writer doesn't support mixed materials.  Moved database writer
//    creation to the engine plugin, and moved the test to see if a database
//    plugin has a writer to the general plugin info.
//
//    Mark C. Miller, Tue Feb  7 11:16:18 PST 2006
//    Added return statement to main
//
//    Hank Childs, Thu Mar 30 11:57:05 PST 2006
//    Add basic (i.e. non-crashing) support for write options.
//
//    Hank Childs, Tue Jan 16 08:37:36 PST 2007
//    Add support for logging which plugins were attempted when a database
//    open fails.
//
//    Jeremy Meredith, Tue Jun  3 14:12:23 EDT 2008
//    Added support for fallback_format and default_format.  Added
//    writer options input.  Put attempt to create a writer after the
//    attempt to open the input file.
//
//    Brad Whitlock, Tue Jun 24 16:58:35 PDT 2008
//    The plugin manager is no longer a singleton.
//
//    Dave Pugmire (on behalf of Hank Childs), Thu May  7 13:43:28 EDT 2009
//    Support for running visitconvert in parallel.
//
//    Mark C. Miller, Tue May 19 21:55:37 PDT 2009
//    Added support for handling read options. Allowed for multiple '-variable'
//    options on the command line.
//
//    Mark C. Miller, Wed Jun 17 14:23:51 PDT 2009
//    Replaced CATCH(...) with CATCHALL
//
//    Hank Childs, Thu Nov  5 17:17:35 PST 2009
//    Only have proc 0 print out error messages.  Also call MPI_Finalize.
//
//    Jeremy Meredith, Wed Dec 30 15:04:36 EST 2009
//    We obsoleted the assumed and fallback format command line options.
//    However, it was simple to leave the command-line option in place
//    for convert as a special case for handling read options, so we simply
//    track that argument value manually now.
//
//    Hank Childs, Fri Sep  7 17:54:21 PDT 2012
//    Add -output_zonal option.
//
// ****************************************************************************

int main(int argc, char *argv[])
{
    int  i;

    bool parallel = false;
#ifdef PARALLEL
    parallel = true;
    PAR_Init(argc, argv);
#endif
    VisItInit::Initialize(argc, argv);

    //
    // Initialize the plugin readers.
    //
    DatabasePluginManager *dbmgr = new DatabasePluginManager;
    dbmgr->Initialize(DatabasePluginManager::Engine, parallel);
    dbmgr->LoadPluginsNow();
    if (PAR_Rank() == 0)
        cerr << endl; // whitespace after some expected plugin loading errors

    if (argc < 4)
    {
        UsageAndExit(dbmgr, argv[0]);
    }

    string assumedFormat = "";
    bool noOptions = false;
    bool doClean = false;
    bool disableMIR = false;
    bool disableExpressions = false;
    vector<string> vars;
    int target_chunks = -1;
    bool outputZonal = false;
    long long target_zones = -1;
    EngineDatabasePluginInfo *edpir = NULL;
    if (argc > 4)
    {
        for (int i = 4 ; i < argc ; i++)
        {
            if (strcmp(argv[i], "-clean") == 0)
                doClean = true;
            else if (strcmp(argv[i], "-nomir") == 0)
                disableMIR = true;
            else if (strcmp(argv[i], "-noexpr") == 0)
                disableExpressions = true;
            else if (strcmp(argv[i], "-variable") == 0)
            {
                if ((i+1) >= argc)
                    UsageAndExit(dbmgr, argv[0]);
                i++;
                vars.push_back(argv[i]);
            }
            else if (strcmp(argv[i], "-output_zonal") == 0)
            {
                outputZonal = true;
            }
            else if (strcmp(argv[i], "-target_chunks") == 0)
            {
                if ((i+1) >= argc)
                    UsageAndExit(dbmgr, argv[0]);
                i++;
                target_chunks = atoi(argv[i]);
            }
            else if (strcmp(argv[i], "-target_zones") == 0)
            {
                if ((i+1) >= argc)
                    UsageAndExit(dbmgr, argv[0]);
                i++;
                target_zones = 0;
                int nchars = strlen(argv[i]);
                for (int j = 0 ; j < nchars ; j++)
                {
                    if (isdigit(argv[i][j]))
                    {
                        target_zones *= 10;
                        target_zones += argv[i][j] - '0';
                    }
                }
            }
            else if (strcmp(argv[i], "-assume_format") == 0)
            {
                if ((i+1) >= argc)
                    UsageAndExit(dbmgr, argv[0]);
                i++;

                assumedFormat = argv[i];
            }
            else if (strcmp(argv[i], "-no_options") == 0)
                noOptions = true;

            else
            {
                //Ignore....
            }
        }
    }


    //
    // Handle read options if we need to.
    //
    HandleReadOptions(noOptions, dbmgr, argv[0], assumedFormat);

    //
    // Instantiate the database.
    //
    avtDatabase *db = NULL;
    vector<string> pluginList;
    TRY
    {
        if (strstr(argv[1], ".visit") != NULL)
            db = avtDatabaseFactory::VisitFile(dbmgr, argv[1], 0, pluginList);
        else
            db = avtDatabaseFactory::FileList(dbmgr, argv+1, 1, 0, pluginList);
    }
    CATCHALL
    {
        if (PAR_Rank() == 0)
            cerr << "The file " << argv[1] << " does not exist or could "
                 << "not be opened." << endl;
        PAR_Exit();
        exit(EXIT_FAILURE);
    }
    ENDTRY

    if (db == NULL)
    {
        if (PAR_Rank() == 0)
            cerr << "Could not open file " << argv[1] << ".  Tried using plugins ";
        for (int i = 0 ; i < pluginList.size() ; i++)
        {
            if (PAR_Rank() == 0)
            {
                cerr << pluginList[i];
                if (i != pluginList.size()-1)
                    cerr << ", ";
                else
                    cerr << endl;
            }
        }
    }

    //
    // Set the write options as the default.
    // Walk through the write options and have the user iterate over them
    // from the command line. [MCM] I believe predicating ONLY the FillOptions...
    // call on value of noOptions permits write options to flow from config
    // file settings correctly.
    //
    EngineDatabasePluginInfo *edpi = GetPluginInfo(dbmgr, argv[0], argv[3]);
    DBOptionsAttributes *opts = edpi->GetWriteOptions();
    if (!noOptions)
        FillOptionsFromCommandline(opts);
    edpi->SetWriteOptions(opts);

    //
    // Make sure this format has a writer.
    //
    avtDatabaseWriter *wrtr = edpi->GetWriter();
    if (wrtr == NULL)
    {
        if (PAR_Rank() == 0)
            cerr << "No writer defined for file type " << argv[3] << ".\n"
                 << "Please see a VisIt developer." << endl;
        UsageAndExit(dbmgr, argv[0]);
    }
    if (doClean)
        wrtr->SetShouldAlwaysDoMIR(doClean);
    if (disableMIR)
        wrtr->SetShouldNeverDoMIR(disableMIR);
    if (disableExpressions)
        wrtr->SetShouldNeverDoExpressions(disableExpressions);
    if (target_zones > 0)
    {
        bool canDoIt = wrtr->SetTargetZones(target_zones);
        if (!canDoIt)
        {
            if (PAR_Rank() == 0)
                cerr << "This writer does not support the \"-target_zones\" option"
                     << endl;
            UsageAndExit(dbmgr, argv[0]);
        }
    }
    if (target_chunks > 0)
    {
        bool canDoIt = wrtr->SetTargetChunks(target_chunks);
        if (!canDoIt)
        {
            if (PAR_Rank() == 0)
                cerr << "This writer does not support the \"-target_chunks\" "
                     << "option" << endl;
            UsageAndExit(dbmgr, argv[0]);
        }
    }
    if (outputZonal)
    {
        bool canDoIt = wrtr->SetOutputZonal(outputZonal);
        if (!canDoIt)
        {
            if (PAR_Rank() == 0)
                cerr << "This writer does not support the \"-output_zonal\" "
                     << "option" << endl;
            UsageAndExit(dbmgr, argv[0]);
        }
    }

    //
    // Figure out which mesh to operate on.
    // Assume MetaData for timestep 0 is sufficient for what we need here
    //
    const avtDatabaseMetaData *md = db->GetMetaData(0);
    const avtMeshMetaData *mmd = NULL;
    string meshname = "";
    if (md->GetNumMeshes() >= 1)
    {
        if (md->GetNumMeshes() > 1)
        {
            if (PAR_Rank() == 0)
            {
                cerr << "There are multiple meshes in the file.  This program can "
                     << "only\nhandle one mesh at a time.  I am using mesh: ";
                cerr << md->GetMesh(0)->name << endl;
            }
        }
        meshname = md->GetMesh(0)->name;
    }
    else if (md->GetNumMeshes() < 1)
    {
        if (md->GetNumCurves() > 0)
        {
            if (PAR_Rank() == 0)
                cerr << "Cannot find any meshes, converting curves." << endl;
            meshname = md->GetCurve(0)->name;
        }
        else
        {
            if (PAR_Rank() == 0)
                cerr << "Cannot find any valid meshes or curves to convert.\n"
                     << "Giving up." << endl;
            PAR_Exit();
            exit(EXIT_FAILURE);
        }
    }

#ifndef DISABLE_EXPRESSIONS
    //
    // Hook up the expressions we have associated with the database, so
    // we can get those as well.
    //
    Parser *p = new ExprParser(new avtExprNodeFactory());
    ParsingExprList *l = new ParsingExprList(p);
    ExpressionList *list = l->GetList();
    for (i = 0 ; i < md->GetNumberOfExpressions() ; i++)
    {
        const Expression *e = md->GetExpression(i);
        list->AddExpressions(*e);
    }
#endif

    if (PAR_Rank() == 0)
        cerr << "Operating on " << md->GetNumStates() << " timestep(s)." << endl;
    for (i = 0 ; i < md->GetNumStates() ; i++)
    {
         avtDataObject_p dob = db->GetOutput(meshname.c_str(), i);
#ifndef DISABLE_EXPRESSIONS
         avtExpressionEvaluatorFilter eef;
         eef.SetInput(dob);
         dob = eef.GetOutput();
#endif
         wrtr->SetInput(dob);

         char filename[1024];
         if (strstr(argv[2], "%") != NULL)
             sprintf(filename, argv[2], i);
         else if (md->GetNumStates() == 1)
             strcpy(filename, argv[2]);
         else
             sprintf(filename, "%04d.%s", i, argv[2]);
        
         TRY
         {
             if (vars.size())
             {
                 wrtr->Write(filename, md, vars);
             }
             else
             {
                 wrtr->Write(filename, md);
             } 
         }
         CATCH2(VisItException, e)
         {
             if (PAR_Rank() == 0)
             {
                 cerr << "Error encountered.  Unable to write files." << endl;
                 cerr << "Error was: " << endl;
                 cerr << e.Message() << endl;
             }
             break;
         }
         ENDTRY
    }

    delete dbmgr;

#ifdef PARALLEL
    PAR_Exit();
#endif

    return 0;
}


// ****************************************************************************
//  Function: UsageAndExit
//
//  Purpose:
//      Issues a usage statement and then exits.
//
//  Programmer: Hank Childs
//  Creation:   September 10, 2003
//
//  Modifications:
//
//    Hank Childs, Sat Sep 11 11:22:09 PDT 2004
//    Added "-target_chunks", "-target_zones", and "-variables".
//
//    Jeremy Meredith, Tue Feb 22 18:27:05 PST 2005
//    Changed it to loop over all the plugins, because the test for
//    having a writer is in the general plugin info.
//
//    Jeremy Meredith, Tue Jun  3 14:27:23 EDT 2008
//    Cleaned up greatly.  Added help for input formats and options.
//
//    Brad Whitlock, Tue Jun 24 16:57:45 PDT 2008
//    Pass in the DatabasePluginManager pointer.
//
//    Hank Childs, Fri Sep 11 17:53:31 CDT 2009
//    Only have proc 0 output error message.  Also call MPI_Finalize.
//
//    Hank Childs, Fri Sep  7 17:54:21 PDT 2012
//    Add -output_zonal.
//
// ****************************************************************************

static void
UsageAndExit(DatabasePluginManager *dbmgr, const char *argv0)
{
    const char *progname = argv0 + strlen(argv0)-1;
    while (progname > argv0 && *(progname-1) != '/')
        progname--;

    if (PAR_Rank() == 0)
    {
        cerr << "Usage: " << progname
             << " <input-file> <output-file> <output-type> [OPTIONS]\n";

        cerr << endl;
        cerr << "Available options:" << endl;
        cerr << "\t-clean" << endl;
        cerr << "\t-target_chunks   <number>" << endl;
        cerr << "\t-target_zones    <number>" << endl;
        cerr << "\t-output_zonal" << endl;
        cerr << "\t-variable        <variable>" << endl;
        cerr << "\t-no_options" << endl;
        cerr << "\t-assume_format   <input_format>" << endl;
        cerr << "\t-fallback_format <input_format>" << endl;
        cerr << endl;
        cerr << "Option descriptions:" << endl;
        cerr << "  -clean should be specified when all clean zones are desired." << endl;
        cerr << "   In this case material interface reconstruction will be performed." << endl;
        cerr << endl;
        cerr << "  -target_zones specifies what the total number of zones in the output" << endl;
        cerr << "   should be." << endl;
        cerr << endl;
        cerr << "  -target_chunks should be specified when the chunks should be" << endl;
        cerr << "   repartitioned.  This is often used in conjunction with -target_zones." << endl;
        cerr << endl;
        cerr << "  -output_zonal specifies the output should be zonal.  This option" << endl;
        cerr << "   only works in conjunction with -target_zones." << endl;
        cerr << endl;
        cerr << "  -variable specifies which variable should be processed." << endl;
        cerr << "  There can be multiple -variable options on the command line." << endl;
        cerr << "  Each one specifies the name of an additional variable to process." << endl;
        cerr << endl;
        cerr << "  -assume_format specifies the plugin format type to try first" << endl;
        cerr << endl;
        cerr << "  -fallback_format specifies a plugin format type to try if the" << endl;
        cerr << "   guessing based on file extension failes" << endl;
        cerr << endl;
        cerr << "  -no_options skips option entry, instead using only default values" << endl;
        cerr << "  where defaults are taken first from your config file unless -noconfig" << endl;
        cerr << "  is specified and then from defaults as specified by the plugin." << endl;
        cerr << endl;
        cerr << "Example (one timestep):" << endl;
        cerr << "  visitconvert_ser run1.exodus run1.silo Silo" << endl;
        cerr << "Example (multi-timestep):" << endl;
        cerr << "  visitconvert_ser run1.exodus run%04d.silo Silo" << endl;
        cerr << "Example (scaling study):" << endl;
        cerr << "  visitconvert_ser rect.silo rect BOV -target_chunks 512  \\" << endl;
        cerr << "                 -target_zones 512000000 -variable var1" << endl;
        cerr << endl;
        cerr << "Acceptable output types are: " << endl;
        for (int i = 0 ; i < dbmgr->GetNAllPlugins() ; i++)
        {
            string plugin = dbmgr->GetAllID(i);
            if (dbmgr->PluginHasWriter(plugin))
                cerr << "\t" << dbmgr->GetPluginName(plugin) << endl;
        }
        delete dbmgr;
    }

    PAR_Exit();
    exit(EXIT_FAILURE);
}


