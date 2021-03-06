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
//                              avtSpreadsheetFilter.C                       //
// ************************************************************************* //

#include <avtSpreadsheetFilter.h>

#include <vtkDataSet.h>
#include <InvalidSetException.h>
#include <InvalidVariableException.h>

#include <avtExtents.h>
#include <avtSILRestrictionTraverser.h>
#include <DebugStream.h>

#include <string>
#include <vector>

// ****************************************************************************
//  Method: avtSpreadsheetFilter constructor
//
//  Programmer: Brad Whitlock
//  Creation:   Tue Feb 6 12:08:42 PDT 2007
//
// ****************************************************************************

avtSpreadsheetFilter::avtSpreadsheetFilter()
{
}

// ****************************************************************************
//  Method: avtSpreadsheetFilter destructor
//
//  Programmer: Brad Whitlock
//  Creation:   Tue Feb 6 12:08:42 PDT 2007
//
// ****************************************************************************

avtSpreadsheetFilter::~avtSpreadsheetFilter()
{
}

// ****************************************************************************
// Method: avtSpreadsheetFilter::SetAtts
//
// Purpose: 
//   Set the attributes used for the filter.
//
// Programmer: Brad Whitlock
// Creation:   Wed Feb 21 17:17:33 PST 2007
//
// Modifications:
//   
// ****************************************************************************

void
avtSpreadsheetFilter::SetAtts(const SpreadsheetAttributes &a)
{
    atts = a;
}

// ****************************************************************************
// Method: avtSpreadsheetFilter::ModifyContract
//
// Purpose: 
//   Turns off all domains except for the specified subset.
//
// Arguments:
//   spec : The pipeline specification.
//
// Returns:    A new pipeline specification.
//
// Note:       This filter overrides any SIL selection that has taken place.
//
// Programmer: Brad Whitlock
// Creation:   Wed Feb 21 17:16:40 PST 2007
//
// Modifications:
//   Brad Whitlock, Wed Mar 28 18:25:33 PST 2007
//   Force MIR to be off.
//
//   Gunther H. Weber, Mon Feb  4 14:41:06 PST 2008
//   Use doubles if they are a file's native precision
//
//   Hank Childs, Mon Dec 14 16:04:01 PST 2009
//   Use new interface for SILs.
//
//   Brad Whitlock, Wed Jan  4 17:08:35 PST 2012
//   Identify missing data instead of remove it.
//
// ****************************************************************************

avtContract_p
avtSpreadsheetFilter::ModifyContract(avtContract_p spec)
{
    const char *mName = "avtSpreadsheetFilter::ModifyContract: ";

    avtContract_p rv = new avtContract(spec);
    std::string firstName;
    avtSILRestriction_p silr = spec->GetDataRequest()->GetRestriction();
    avtSILSet_p current = silr->GetSILSet(silr->GetTopSet());
    const std::vector<int> &mapsOut = current->GetMapsOut();
    int nSets = 1;
    int setId = -1, firstSetId = -1;
    for(int j = 0; j < mapsOut.size() && setId==-1; ++j)
    {
        int cIndex = mapsOut[j];
        avtSILCollection_p collection = silr->GetSILCollection(cIndex);
        if(*collection != NULL && collection->GetRole() == SIL_DOMAIN) 
        {
            int nSets = collection->GetNumberOfSubsets();
            for(int si = 0; si < nSets && setId==-1; ++si)
            {
                if(firstSetId == -1)
                    firstSetId = collection->GetSubset(si);
                if(silr->GetSILSet(collection->GetSubset(si))->GetName() == 
                   atts.GetSubsetName())
                {
                    setId = collection->GetSubset(si);
                }
            }
            break;
        }
    }

    //
    // If we've identified which set to use, do something with it now.
    //
    silr = rv->GetDataRequest()->GetRestriction();
    if(setId != -1)
    {
        debug1 << mName << "The set " << atts.GetSubsetName()
               << " was okay. Its setid = " << setId << endl;

        // The set name that was provided is okay.
        silr->TurnOffAll();
        silr->TurnOnSet(setId);
    }
    else
    {
        debug1 << mName << "The set " << atts.GetSubsetName()
               << " was not okay. ";

        // The set name that was provided is not okay.
        if(nSets > 1)
        {
            // Use the first set.
            if(firstSetId != -1)
            {
                debug1 << "One domain so turn on all sets" << endl;
                silr->TurnOffAll();
                silr->TurnOnSet(firstSetId);
            }
            else
            {
                EXCEPTION1(InvalidSetException, atts.GetSubsetName().c_str());
            }
        }
        else
        {
            debug1 << "One domain so turn on all sets" << endl;

            // Turn on all sets.
            silr->TurnOnAll();
        }
    }

    // Force material interface reconstruction to be off.
    rv->GetDataRequest()->ForceMaterialInterfaceReconstructionOff();
    rv->GetDataRequest()->SetNeedMixedVariableReconstruction(false);

    // Force native precision
    rv->GetDataRequest()->SetNeedNativePrecision(true);

    // Add double to the permissible types
    std::vector<int> adTypes;
    adTypes.push_back(VTK_FLOAT);
    adTypes.push_back(VTK_DOUBLE);
    rv->GetDataRequest()->UpdateAdmissibleDataTypes(adTypes);

    rv->SetCalculateMeshExtents(true);
    rv->SetCalculateVariableExtents(rv->GetDataRequest()->GetVariable(), true);

    // Indicate that we want to only identify missing data instead of removing it.
    rv->GetDataRequest()->IdentifyMissingData();

    return rv;
}

// ****************************************************************************
//  Method: avtSpreadsheetFilter::ExecuteData
//
//  Purpose:
//      Does the actual VTK code to modify the dataset.
//
//  Arguments:
//      inDS      The input dataset.
//      <unused>  The domain number.
//      <unused>  The label.
//
//  Returns:      The output dataset.
//
//  Programmer: Brad Whitlock
//  Creation:   Tue Feb 6 12:08:42 PDT 2007
//
// ****************************************************************************

void
avtSpreadsheetFilter::Execute(void)
{
    avtDataObject_p input = GetInput();
    GetOutput()->Copy(*input);
}

// ****************************************************************************
//  Method: avtCurveFilter::ModifyContract
//
//  Purpose:
//      Allows the filter to change its output's data object information, which
//      is a description of the data object.
//
//  Notes: We use this routine to force the curve extents to be present in
//         the data attributes when we have curve data. I tried doing this
//         as UpdateDataObjectInfo but it never was honored.
//
//  Programmer: Brad Whitlock
//  Creation:   Thu Dec 16 10:13:12 PST 2010
//
// ****************************************************************************

void
avtSpreadsheetFilter::PostExecute(void)
{
    if(GetInput()->GetInfo().GetAttributes().GetTopologicalDimension() == 1)
    {
        // Set the mesh's extents to 2D
        double extents[6] = {0.,0.,0.,0.,0.,0.};
        GetOutput()->GetInfo().GetAttributes().GetThisProcsOriginalSpatialExtents()->CopyTo(extents);
        TRY
        {
            const std::string &v = GetOutput()->GetInfo().GetAttributes().GetVariableName();
            GetOutput()->GetInfo().GetAttributes().GetOriginalDataExtents(v.c_str())->
                CopyTo(extents+2);
        }
        CATCH(VisItException)
        {
            extents[3] = 1.;
        }
        ENDTRY
        GetOutput()->GetInfo().GetAttributes().SetSpatialDimension(2);
        GetOutput()->GetInfo().GetAttributes().GetThisProcsOriginalSpatialExtents()->Set(extents);
        GetOutput()->GetInfo().GetAttributes().SetCanUseThisProcsAsOriginalOrActual(true);
   }
}

