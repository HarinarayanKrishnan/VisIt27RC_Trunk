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

#include <Netnodes.h>
#include <DebugStream.h>
#include <ImproperUseException.h>

// ****************************************************************************
//  Method: NetnodeDB constructor
//
//  Arguments:
//      db_in   A pointer to a database.
//
//  Programmer: Hank Childs
//  Creation:   July 28, 2003
//
// ****************************************************************************

NetnodeDB::NetnodeDB(avtDatabase *db_in)
{
    db = db_in;
    filename = "<unknown>";
    var = "<unknown>";
    time = -1;
    current_var = "<unknown>";
    current_time = -1;
}


// ****************************************************************************
//  Method: NetnodeDB constructor
//
//  Arguments:
//      db_in   A pointer to a database.
//
//  Programmer: Hank Childs
//  Creation:   July 28, 2003
//
// ****************************************************************************

NetnodeDB::NetnodeDB(ref_ptr<avtDatabase> db_in)
{
    db = db_in;
    filename = "<unknown>";
    var = "<unknown>";
    time = -1;
    current_var = "<unknown>";
    current_time = -1;
}


// ****************************************************************************
//  Method: NetnodeDB::SetDBInfo
//
//  Purpose:
//      Sets the information about the database.
//
//  Notes:      This was previously an inlined function.  Creation date is
//              for when the function was defined as a non-inlined function.
//
//  Programmer: Hank Childs
//  Creation:   November 18, 2003
//
// ****************************************************************************

void
NetnodeDB::SetDBInfo(std::string _filename, std::string _var, int _time)
{
    if ((time != _time) || (filename != _filename) || (var != _var))
    {
        if (*output != NULL)
        {
            output->ReleaseData();
        }
    }

    time = _time;
    filename = _filename;
    var = _var;
}


// ****************************************************************************
//  Method: NetnodeDB::GetOutput
//
//  Purpose:
//      Gets the output from the database.
//
//  Programmer: Hank Childs
//  Creation:   September 10, 2002
//
//  Modificiations:
//
//    Hank Childs, Mon Jul 28 13:49:58 PDT 2003
//    Returned the cached output if possible.
//
// ****************************************************************************

avtDataObject_p
NetnodeDB::GetOutput(void)
{
    if ((*output != NULL) &&
        (var == current_var) &&
        (time == current_time))
    {
        return output;
    }

    output = db->GetOutput(var.c_str(), time);
    current_var = var;
    current_time = time;

    return output;
}


// ****************************************************************************
//  Method: NetnodeDB::ReleaseData
//
//  Purpose:
//      Tells the database to release the data associated with this output.
//
//  Programmer: Hank Childs
//  Creation:   September 10, 2002
//
//  Modifications:
//
//    Hank Childs, Thu Apr  1 07:33:42 PST 2004
//    Don't try to release the data if we don't have a variable -- this can
//    happen if we never used the database, or if we are doing a re-open.
//
//    Mark C. Miller, Fri Apr  2 10:18:55 PDT 2010
//    Wrapped on try/catch block. If for any reason an attempt to release
//    data throws an exception, we want to catch here on the engine as early
//    as possible so that other release-data-like operations can still proceed
//    and the engine remains in a decent state.
// ****************************************************************************

void
NetnodeDB::ReleaseData(void)
{
    if (var != "" && var != "<unknown>")
    {
        TRY
        {
            GetOutput()->ReleaseData();
        }
        CATCHALL
        {
            debug1 << "An attempt to release data associated with \""
                   << var << "\" has thrown an exception. This may result in "
                   << "some leaked memory." << endl;
        }
        ENDTRY
    }
}


// ****************************************************************************
//  Method:  NetnodeFilter::GetOutput
//
//  Purpose:
//    Get the final output of a network.  This has the effect of walking up
//    the line, hooking everything up.
//
//  Programmer: Sean Ahern
//  Creation:   June 26, 2002
//
//  Modifications:
//
//    Hank Childs, Fri Jan 16 14:34:40 PST 2009
//    If this method has already been called once, then don't set up the
//    network again.  We probably had it right the first time and subsequent
//    "settings up" will just get things wrong.  Case in point: ray-traced
//    volume rendering.  The first execution calculates a gradient.  The 
//    second (if another plot has been created in between) can wipe out the
//    data attributes.  So the volume filter will complain that it doesn't
//    have the gradient, even though it is actually there ... the problem
//    was that the data atts were overwritten ... which is avoided by not
//    overwriting them on subsequent times (i.e. this change).
//
// ****************************************************************************

avtDataObject_p
NetnodeFilter::GetOutput(void)
{
    // This is mostly a hack.  Filters that have more than one "input"
    // actually ignore all inputs other than the first.
    // Ahern - Wed Jun 26 14:27:42 PDT 2002
#if 0
    // We don't yet support more than one input
    if (inputNodes.size() != 1)
    {
        debug1 << "NetnodeFilter::GetOutput: Unable to add function with more than one argument." << endl;
        EXCEPTION0(ImproperUseException);
    }
#endif

    if (*(filter->GetInput()) == NULL)
        filter->SetInput(inputNodes[0]->GetOutput());

    return filter->GetOutput();
}

// ****************************************************************************
//  Method:  NetnodeFilter::ReleaseData
//
//  Purpose:
//    Releases the data associated with the filter.
//
// ****************************************************************************
void
NetnodeFilter::ReleaseData(void)
{
    // Release the data for my filter (and its inputs).
    filter->ReleaseData();

    // Release the data for my inputs.
    for (int i = 0; i < inputNodes.size(); i++)
        inputNodes[i]->ReleaseData();
}

// ****************************************************************************
//  Method:  NetnodeTransition::Constructor
//
//  Purpose:
//    Acts as a transition node between one network and its clone.
//
//  Programmer:  Kathleen Bonnell
//  Creation:    March 18, 2004
//
// ****************************************************************************

NetnodeTransition::NetnodeTransition(avtDataObject_p input) :
    NetnodeFilter(NULL, "")
{
    output = input;
}
