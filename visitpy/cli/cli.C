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
#include <Python.h>

// get select
#if defined(_WIN32)
#include <winsock2.h>
#else
#include <strings.h>   // bzero by way of FD_ZERO
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#endif

// The following 2 include lines are only for the MIPSpro 7.41 compiler.
// There is some conflict between Python.h and Utility.h in including
// those 2 files.  Remove once support for MIPSpro is dropped.
#include <sys/types.h>
#include <sys/stat.h>

#include <ctype.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

#include <FileFunctions.h>
#include <Utility.h>
#include <InstallationFunctions.h>
#include <StringHelpers.h>
#include <VisItException.h>

#include <string>
#include <vector>

#ifdef WIN32
  #define VISITCLI_API __declspec(dllimport)

  #define BEGINSWITHQUOTE(A) (A[0] == '\'' || A[0] == '\"')

  #define ENDSWITHQUOTE(A) (A[strlen(A)-1] == '\'' || A[strlen(A)-1] == '\"')

  #define HASSPACE(A) (strstr(A, " ") != NULL)
#else
  #define VISITCLI_API
#endif
// For the VisIt module.
extern "C" void cli_initvisit(int, bool, int, char **, int, char **);
extern "C" void cli_runscript(const char *);
extern "C" VISITCLI_API int Py_Main(int, char **);

// ****************************************************************************
// Function: main
//
// Purpose:
//   This is the main function for the cli program.
//
// Notes:      Most of the program is in visitmodule.C which is also built as
//             a shared library.
//
// Programmer: Brad Whitlock
// Creation:   Fri Dec 7 15:34:11 PST 2001
//
// Modifications:
//   Brad Whitlock, Tue Dec 11 10:47:55 PDT 2001
//   I fixed the -debug, -s, -nowin arguments.
//
//   Brad Whitlock, Fri May 10 10:23:47 PDT 2002
//   I added the -o argument for opening databases.
//
//   Brad Whitlock, Mon Dec 16 13:43:20 PST 2002
//   I added the -verbose argument.
//
//   Brad Whitlock, Tue Jul 15 12:59:22 PDT 2003
//   I moved the code to run a script into the visit module.
//
//   Brad Whitlock, Mon Jul 28 16:51:22 PST 2003
//   Added code to prevent -sessionfile, -format, -framestep, and -output
//   from being passed on to the viewer.
//
//   Hank Childs, Thu Apr  1 08:57:11 PST 2004
//   Prevent -fps from being passed as well.
//
//   Brad Whitlock, Wed May 4 08:34:04 PDT 2005
//   I changed the code so argv[0] gets passed to the cli in argv2. I also
//   made Python initialize its threading.
//
//   Dave Bremer, Wed Mar 14 17:58:15 PDT 2007
//   Pass commandline arguments through to the main interpreter.
//
//   Jeremy Meredith, Wed Jun  6 16:41:45 EDT 2007
//   Add the -forceinteractivecli argument (equivalent to -i in python).
//
//   Brad Whitlock, Fri Jun 8 10:57:02 PDT 2007
//   Added support for saving the arguments after -s to their own tuple.
//
//   Brad Whitlock, Fri Jun 15 16:49:53 PST 2007
//   Load the visitrc file on startup.
//
//   Hank Childs, Thu Nov  8 15:49:38 PST 2007
//   Add support for ignoring nohups.
//
//   Gunther H. Weber, Thu Feb  7 14:44:20 PST 2008
//   Check for visitrc in golobal .visit directory to enable site-wide
//   macros.
//
//   Kathleen Bonnell, Thu Jun 26 17:22:55 PDT 2008 
//   Add special handling of '-s' and '-o' args on Windows, to ensure proper
//   parsing of paths-with-spaces. 
//
//    Jeremy Meredith, Thu Aug  7 15:01:14 EDT 2008
//    Assume Python won't modify argv, and cast a string literal to
//    a const.
//
//    Mark C. Miller, Tue Apr 21 14:24:18 PDT 2009
//    Added logic to manage buffering of debug logs; an extra 'b' after level.
//
//    Mark C. Miller, Wed Jun 17 14:27:08 PDT 2009
//    Replaced CATCHALL(...) with CATCHALL.
//
//    Cyrus Harrison, Mon Jun 29 15:58:02 PDT 2009
//    If a script file is passed - add its directory to the end of sys.path
//    to enable easy importing.
//
//    Cyrus Harrison, Wed Sep 30 07:53:17 PDT 2009
//    Added book keeping to track execution stack of source files and the
//    script file passed via the '-s' command line option.
//
//    Kathleen Bonnell, Wed Mar 24 08:20:01 MST 2010
//    Fix pointer overwrite problem when both "-s" and "-o" are used on Windows
//
//    Jeremy Meredith, Fri Mar 26 13:11:46 EDT 2010
//    Allow for the -o command line option to take an optional ,<pluginID>
//    suffix, e.g. "-o foobar,LAMMPS_1.0".
//
//    Hank Childs, Thu Oct 28 11:51:05 PDT 2010
//    If an exception is uncaught, then print out the information to the screen
//    and hang.  This hang will let folks using the CLI through GUI see the
//    failure.
//
//    Hank Childs, Fri Oct 29 12:32:26 PDT 2010
//    Change while (1) to select based on guidance from Sean Ahern.
//
//    Cyrus Harrison, Thu Sep 29 15:04:16 PDT 2011
//    Add PySide Support.
//
//    Cyrus Harrison, Thu Apr 12 17:33:16 PDT 2012
//    Update to reflect changes made in visit python module revamp.
//
//    Kathleen Biagas, Fri May 4 14:05:27 PDT 2012  
//    Use GetVisItLibraryDirectory to find lib location. 
//    SetIsDevelopmentVersion when -dv encountered.
//
//    Kathleen Biagas, Thu May 24 19:20:19 MST 2012  
//    Ensure visit's lib dir has path-separators properly escaped on Windows
//    before being passed to the pjoin command.
//
//    Brad Whitlock, Wed Jun 20 11:37:23 PDT 2012
//    Added -minimized argument to minimize the cli window on Windows.
//
//    Kathleen Biagas, Thu May 24 19:20:19 MST 2012  
//    Ensure runFile has path-separators properly escaped on Windows.
//
// ****************************************************************************

int
main(int argc, char *argv[])
{
    int  retval = 0;
    int  debugLevel = 0;
    bool bufferDebug = false;
    bool verbose = false, s_found = false;
    bool pyside = false;
    bool pyside_gui = false,pyside_viewer = false;
    char *runFile = 0, *loadFile = 0, tmpArg[512];
    char **argv2 = new char *[argc];
    char **argv_after_s = new char *[argc];
    int  argc2 = 0, argc_after_s = 0; 
    char* uifile = 0;

#ifdef IGNORE_HUPS
    signal(SIGHUP, SIG_IGN);
#endif

    // Parse the arguments
    for(int i = 0; i < argc; ++i)
    {
        if(strcmp(argv[i], "-debug") == 0)
        {
            debugLevel = 1;
            if (i+1 < argc && isdigit(*(argv[i+1])))
               debugLevel = atoi(argv[i+1]);
            else
               fprintf(stderr,"Warning: debug level not specified, "
                              "assuming 1\n");
            if (i+1 < argc && *(argv[i+1]+1) == 'b')
                bufferDebug = true;
            i++;
            if (debugLevel < 0)
            {
                debugLevel = 0;
                fprintf(stderr,"Warning: clamping debug level to 0\n");
            }
            if (debugLevel > 5)
            {
                debugLevel = 5;
                fprintf(stderr,"Warning: clamping debug level to 5\n");
            }
        }
#ifdef WIN32
        else if((strcmp(argv[i], "-s") == 0 && (i+1 < argc)) ||
                (strcmp(argv[i], "-o") == 0 && (i+1 < argc)))
        {
            bool runF = (strcmp(argv[i], "-s") == 0);
            ++i;
            // append all parts of this arg back into one string
            if (BEGINSWITHQUOTE(argv[i]) && !ENDSWITHQUOTE(argv[i]))
            {
                strcpy(tmpArg, argv[i]);
                int nArgsSkip = 1;
                size_t tmplen = strlen(argv[i]);
                for (int j = i+1; j < argc; j++)
                {
                    nArgsSkip++;
                    strcat(tmpArg, " ");
                    strcat(tmpArg, argv[j]);
                    tmplen += (strlen(argv[j]) +1);
                    if (ENDSWITHQUOTE(argv[j]))
                        break;
                }
                i += (nArgsSkip -1);
                // We want to remove the beginning and ending quotes, to 
                // ensure proper operation further on.
                strncpy(tmpArg, tmpArg+1, tmplen-2);
                tmpArg[tmplen-2] = '\0';
            }
            else 
            {
                sprintf(tmpArg, "%s", argv[i]);
            }
            if (runF)
            {
                runFile = new char [strlen(tmpArg)+1];
                sprintf(runFile, "%s", tmpArg);
                s_found = true;
            }
            else
            {
                loadFile = new char [strlen(tmpArg)+1];
                sprintf(loadFile, "%s", tmpArg);
            }
        }
#else
        else if(strcmp(argv[i], "-s") == 0 && (i+1 < argc))
        {
            runFile = argv[i+1];
            ++i;

            s_found = true;
        }
        else if(strcmp(argv[i], "-o") == 0 && (i+1 < argc))
        {
            loadFile = argv[i+1];
            ++i;
        }
#endif
        else if(strcmp(argv[i], "-verbose") == 0)
        {
            verbose = true;
        }
        else if(strcmp(argv[i], "-output") == 0)
        {
            // Skip the argument that comes along with -output. Note that
            // the -output argument will be accessible in sys.argv but it
            // is not passed along in the args that are sent to the viewer.
            ++i;
        }
        else if(strcmp(argv[i], "-forceinteractivecli") == 0)
        {
            // Force the python interpreter to behave interactively
            // even if there is no TTY.  This lets clients hook
            // into visit's cli using pipes, and is the equivalent
            // of the "-i" flag in python.
            Py_InteractiveFlag++;
        }
        else if(strcmp(argv[i], "-format") == 0)
        {
            // Skip the argument that comes along with -format.
            ++i;
        }
        else if(strcmp(argv[i], "-framestep") == 0)
        {
            // Skip the argument that comes along with -framestep.
            ++i;
        }
        else if(strcmp(argv[i], "-sessionfile") == 0)
        {
            // Skip the argument that comes along with -sessionfile.
            ++i;
        }
        else if(strcmp(argv[i], "-fps") == 0)
        {
            // Skip the rate that comes along with -fps.
            ++i;
        }
        else if(strcmp(argv[i], "-pyside") == 0)
        {
            pyside = true;
        }
        else if(strcmp(argv[i], "-pysideviewer") == 0)
        {
            pyside_gui = true;
            pyside_viewer = true;
            argv2[argc2++] = "-pyuiembedded"; //pass it along to client
        }
        else if(strcmp(argv[i], "-pysideclient") == 0)
        {
            pyside_gui = true;
            argv2[argc2++] = "-pyuiembedded"; //pass it along to client
        }
        else if(strcmp(argv[i], "-uifile") == 0)
        {
            pyside_gui = true;
            uifile = argv[i+1];
            ++i;
            argv2[argc2++] = "-pyuiembedded"; //pass it along to client
        }
        else if(strcmp(argv[i], "-dv") == 0)
        {
            SetIsDevelopmentVersion(true);
        }

        else if(strcmp(argv[i], "-minimized") == 0)
        {
#ifdef WIN32
            HWND console = GetConsoleWindow();
            if(console != NULL)
                ShowWindow(console, SW_MINIMIZE);
#endif
        }
        else
        {
            // Pass the array along to the visitmodule.
            argv2[argc2++] = argv[i];

            // This argument is after -s file.py so count it unless it's
            // the -dv argument.
            if(s_found && 
               strcmp(argv[i], "-dv") != 0)
            {
                argv_after_s[argc_after_s++] = argv[i];
            }
        }
    }

    //for some reason if the viewer requests the pysideviewer
    //for now this does not make sense
    for(int i = 0; i < argc; ++i)
    {
        std::string tmp = argv[i];
        if(pyside_gui && tmp == "-reverse_launch")
        {
            std::cerr << "Warning: Cannot enable pysideviewer from remote process,"
                    << " disabling and reverting functionality to pyside"
                    << std::endl;
            pyside_gui = false;
            pyside = true;
        }
        if(pyside_gui && tmp == "-nowin")
        {
            std::cerr << "Error: Cannot create pyside client and nowin at the same time" 
                      << std::endl;
            return (0);
        }

        if(pyside_gui && tmp == "-defer")
        {
            std::cerr << "Warning: Cannot use pyside client and defer at the same time"
                      << std::endl;
            return (0);
        }
    }

    TRY
    {
        // Initialize python
        Py_Initialize();
        PyEval_InitThreads();
        Py_SetProgramName(argv[0]);
        PySys_SetArgv(argc, argv);

        PyRun_SimpleString((char*)"import sys");
        PyRun_SimpleString((char*)"import os");
        PyRun_SimpleString((char*)"from os.path import join as pjoin");

        // add lib to sys.path to pickup various dylibs.
        std::string vlibdir  = GetVisItLibraryDirectory(); 
        std::ostringstream oss;

        oss << "sys.path.append(pjoin(r'" << vlibdir  <<"','site-packages'))";
        PyRun_SimpleString(oss.str().c_str());

        PyRun_SimpleString((char*)"import visit");

        // Initialize the VisIt module.
        cli_initvisit(bufferDebug ? -debugLevel : debugLevel, verbose, argc2, argv2,
                      argc_after_s, argv_after_s);

        if(pyside || pyside_gui)
        {
            int error = 0;
            if(!error) error |= PyRun_SimpleString((char*)"from PySide.QtCore import *");
            if(!error) error |= PyRun_SimpleString((char*)"from PySide.QtGui import *");
            if(!error) error |= PyRun_SimpleString((char*)"from PySide.QtOpenGL import *");
            if(!error) error |= PyRun_SimpleString((char*)"from PySide.QtUiTools import *");
            if(!error) error |= PyRun_SimpleString((char*)"import visit.pyside_support");
            if(!error) error |= PyRun_SimpleString((char*)"import visit.pyside_hook");
            
            if(error) 
            {
                std::cerr   << "Error: Unable to initialize PySide components" 
                            << std::endl;
                return (0); 
            }
            else
            {
                PyRun_SimpleString((char*)"visit.pyside_support.SetupTimer()");
                PyRun_SimpleString((char*)"visit.pyside_hook.SetHook()");
            }
        }

        if(pyside_gui)
        {
            //pysideviewer needs to be executed before visit import
            //so that visit will use the window..
            // we will only have one instance, init it
            int error = PyRun_SimpleString((char*)"import visit.pyside_gui");

            if(error)
            {
                std::cerr   << "Error: Unable to initialize PySide GUI components" 
                            << std::endl;
                return (0);
            }

            PyRun_SimpleString((char*)"args = sys.argv");
            if(uifile) //if external file then start VisIt in embedded mode
                PyRun_SimpleString((char*)"args.append('-pyuiembedded')"); //default to embedded
            PyRun_SimpleString((char*)"tmp = visit.pyside_gui.PySideGUI.instance(args)");
            PyRun_SimpleString((char*)"visit.InitializeViewerProxy(tmp.GetViewerProxyPtr())");
            PyRun_SimpleString((char*)"from visit.pyside_support import GetRenderWindow");
            PyRun_SimpleString((char*)"from visit.pyside_support import GetRenderWindowIds");
            PyRun_SimpleString((char*)"from visit.pyside_support import GetUIWindow");
            PyRun_SimpleString((char*)"from visit.pyside_support import GetPlotWindow");
            PyRun_SimpleString((char*)"from visit.pyside_support import GetOperatorWindow");
            PyRun_SimpleString((char*)"from visit.pyside_support import GetOtherWindow");
            PyRun_SimpleString((char*)"from visit.pyside_support import GetOtherWindowNames");

            if(!uifile && !pyside_viewer)
                PyRun_SimpleString((char*)"GetUIWindow().show()");
        }

        // Initialize the VisIt module.
        //cli_initvisit(bufferDebug ? -debugLevel : debugLevel, verbose, argc2, argv2,
        //              argc_after_s, argv_after_s);

        // setup source file and source stack variables
        PyRun_SimpleString((char*)"import os\n"
                                  "__visit_script_file__  = '<None>'\n"
                                  "__visit_source_file__  = None\n"
                                  "__visit_source_stack__ = [] \n");


        PyRun_SimpleString((char*)"visit.Launch()");

        // reload symbols from visit, since they may have changed
        PyRun_SimpleString((char*)"from visit import *");

        // If a visitrc file exists, execute it.
        std::string visitSystemRc(GetSystemVisItRCFile());
        std::string visitUserRc(GetUserVisItRCFile());

        VisItStat_t s;
        std::string visitrc;
        if(VisItStat(visitUserRc.c_str(), &s) == 0)
        {
            visitrc = visitUserRc;
        }
        else if (VisItStat(visitSystemRc.c_str(), &s) == 0)
        {
            visitrc = visitSystemRc;
        }

        if(!uifile && (pyside_viewer || pyside_gui))
            PyRun_SimpleString((char*)"visit.ShowAllWindows()");

        if(uifile && pyside_gui)
        {
            std::ostringstream pycmd;
            pycmd << "visit.Source(\"" << uifile << "\")";
            PyRun_SimpleString(pycmd.str().c_str());
        }

        if (visitrc.size())
        {
            PyRun_SimpleString("ClearMacros()");
            cli_runscript(visitrc.c_str());
        }

        // If a database was specified, load it.
        if(loadFile != 0)
        {
             char *command = new char[strlen(loadFile) + 10 + 16];
             std::vector<std::string> split = SplitValues(loadFile, ',');
             if (split.size() == 2)
             {
                 sprintf(command, "OpenDatabase(\"%s\", 0, \"%s\")",
                         split[0].c_str(), split[1].c_str());
             }  
             else
             {
                 sprintf(command, "OpenDatabase(\"%s\")", loadFile);
             }
             PyRun_SimpleString(command);
             delete [] command;
#ifdef WIN32
             delete [] loadFile;
#endif
        }


        // If there was a file to execute, do it.
        if(runFile !=0)
        {
            // add the script file's dir to the cli's sys.path
            std::string pycmd  = "import sys\n";
            pycmd += "__visit_script_file__ = os.path.abspath(r'";
            pycmd +=  std::string(runFile) + "')\n";
            pycmd += "__visit_script_path__ = ";
            pycmd += "os.path.split(__visit_script_file__)[0]\n";
            pycmd += "sys.path.append(__visit_script_path__)\n";
            PyRun_SimpleString(pycmd.c_str());

            cli_runscript(runFile);
#ifdef WIN32
             delete [] runFile;
#endif
        }

        // Enter the python interpreter loop.
        //int argc3 = 1;
        //char *argv3 = argv[0];
        //retval = Py_Main(argc3, &argv3);

        char **argv3 = new char*[argc+1];
        int ii;
        for (ii = 1 ; ii < argc ; ii++)
        {
            argv3[ii+1] = argv[ii];
        }
        argv3[0] = argv[0];
        argv3[1] = (char*)"-";
        retval = Py_Main(argc+1, argv3);
        delete[] argv3;

    }
    CATCH(VisItException &e)
    {
        cerr << "A fatal exception occurred when trying to launch the CLI." << endl;
        cerr << "The following information may help a VisIt developer debug the problem: " << endl;
        cerr << "Message: " << e.Message() << endl;
        cerr << "Type: " << e.GetExceptionType() << endl;
        cerr << "Location: " << e.GetFilename() << ", " << e.GetLine() << endl;
        cerr << "(You must press Ctrl-C to get the program to stop)" << endl;
        // If the GUI launched the CLI, the user won't have a chance to see
        // message ... the terminal will disappear when the program finishes.
        //  So go in an infinite loop to make sure they see the message.
        select(0,NULL,NULL,NULL,NULL);
        retval = -1;
    }
    ENDTRY

    // Delete the argv2 array.
    delete [] argv2;
    delete [] argv_after_s;

    return retval;
}
