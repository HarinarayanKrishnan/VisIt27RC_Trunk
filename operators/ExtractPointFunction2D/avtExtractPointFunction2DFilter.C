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
//  File: avtExtractPointFunction2DFilter.C
// ************************************************************************* //

#include <avtExtractPointFunction2DFilter.h>

#include <float.h>

#include <vtkCellData.h>
#include <vtkDataArray.h>
#include <vtkDoubleArray.h>
#include <vtkFieldData.h>
#include <vtkIntArray.h>
#include <vtkRectilinearGrid.h>

// ****************************************************************************
//  Method: avtExtractPointFunction2DFilter constructor
//
//  Programmer: ghweber -- generated by xml2avt
//  Creation:   Tue Jun 4 10:46:56 PDT 2013
//
// ****************************************************************************

avtExtractPointFunction2DFilter::avtExtractPointFunction2DFilter()
{
}


// ****************************************************************************
//  Method: avtExtractPointFunction2DFilter destructor
//
//  Programmer: ghweber -- generated by xml2avt
//  Creation:   Tue Jun 4 10:46:56 PDT 2013
//
//  Modifications:
//
// ****************************************************************************

avtExtractPointFunction2DFilter::~avtExtractPointFunction2DFilter()
{
}


// ****************************************************************************
//  Method:  avtExtractPointFunction2DFilter::Create
//
//  Programmer: ghweber -- generated by xml2avt
//  Creation:   Tue Jun 4 10:46:56 PDT 2013
//
// ****************************************************************************

avtFilter *
avtExtractPointFunction2DFilter::Create()
{
    return new avtExtractPointFunction2DFilter();
}


// ****************************************************************************
//  Method:      avtExtractPointFunction2DFilter::SetAtts
//
//  Purpose:
//      Sets the state of the filter based on the attribute object.
//
//  Arguments:
//      a        The attributes to use.
//
//  Programmer: ghweber -- generated by xml2avt
//  Creation:   Tue Jun 4 10:46:56 PDT 2013
//
// ****************************************************************************

void
avtExtractPointFunction2DFilter::SetAtts(const AttributeGroup *a)
{
    atts = *(const ExtractPointFunction2DAttributes*)a;
}


// ****************************************************************************
//  Method: avtExtractPointFunction2DFilter::Equivalent
//
//  Purpose:
//      Returns true if creating a new avtExtractPointFunction2DFilter with the given
//      parameters would result in an equivalent avtExtractPointFunction2DFilter.
//
//  Programmer: ghweber -- generated by xml2avt
//  Creation:   Tue Jun 4 10:46:56 PDT 2013
//
// ****************************************************************************

bool
avtExtractPointFunction2DFilter::Equivalent(const AttributeGroup *a)
{
    return (atts == *(ExtractPointFunction2DAttributes*)a);
}


// ****************************************************************************
//  Method: avtExtractPointFunction2DFilter::PreExecute
//
//  Purpose:
//      Called before "Execute". Initialize variable we use to keep track of
//      value range.
//
//  Programmer: Gunther H. Weber 
//  Creation:   June 6, 2013
//
//  Modifications:
//
// ****************************************************************************

void
avtExtractPointFunction2DFilter::PreExecute(void)
{
    avtPluginDataTreeIterator::PreExecute();
    range[0] =  FLT_MAX;
    range[1] = -FLT_MAX;
}


// ****************************************************************************
//  Method: avtExtractPointFunction2DFilter::ExecuteData
//
//  Purpose:
//      Sends the specified input and output through the ExtractPointFunction2D filter.
//
//  Arguments:
//      in_ds      The input dataset.
//      <unused>   The domain number.
//      <unused>   The label.
//
//  Returns:       The output dataset.
//
//  Programmer: ghweber -- generated by xml2avt
//  Creation:   Tue Jun 4 10:46:56 PDT 2013
//
// ****************************************************************************

vtkDataSet *
avtExtractPointFunction2DFilter::ExecuteData(vtkDataSet *in_ds, int domain, std::string)
{
    // Ensure that we are working on rectilinear grid
    vtkRectilinearGrid *rgrid = dynamic_cast<vtkRectilinearGrid*>(in_ds);
    if (!rgrid)
        EXCEPTION1(ImproperUseException,
                "Can only extract point function for a rectilinear grid.");

    // Dimensions of data set
    int dims[3];
    rgrid->GetDimensions(dims);
    // We want number of cells as grid dimension, not number of samples
    for (int d=0; d<3; ++d)
        dims[d]--;


    vtkIntArray *arr = dynamic_cast<vtkIntArray*>(in_ds->GetFieldData()->GetArray("base_index"));
    int base_index[3] = { 0, 0, 0 };
    if (arr)
        for (int d = 0; d < 3; ++d)
            base_index[d] = arr->GetValue(d);

    arr = dynamic_cast<vtkIntArray*>(in_ds->GetFieldData()->GetArray("avtRealDims"));

    int iMin = base_index[0];
    int iMax = base_index[0] + dims[0] - 1;
    int jMin = base_index[1];
    int jMax = base_index[1] + dims[1] - 1;
    int kMin = base_index[2];
    int kMax = base_index[2] + dims[2] - 1;

    int avtRealDims[6] = { 0, 0, 0, 0, 0, 0 };
    if (arr)
        for (int d = 0; d < 6; ++d)
            avtRealDims[d] = arr->GetValue(d);

    // FIXME: Take avtRealDims into account
    if (iMin <= atts.GetI() && atts.GetI() <= iMax && jMin <= atts.GetJ() && atts.GetJ() <= jMax)
    {
        double dx = 1.0;
        vtkDoubleArray *dx_arr = dynamic_cast<vtkDoubleArray*>(in_ds->GetFieldData()->GetArray("dx_array"));
        if (dx_arr)
            dx = dx_arr->GetValue(0);

        const char *justTheVar = pipelineVariable + strlen("operators/ExtractPointFunction2D/");
        int nK = dims[2];
        vtkDataArray *data = in_ds->GetCellData()->GetArray(justTheVar);
        int nL = data->GetNumberOfComponents();

        vtkRectilinearGrid *ogrid = vtkRectilinearGrid::New();
        ogrid->SetDimensions(nK+1, nL+1, 1);
        ogrid->SetXCoordinates(rgrid->GetZCoordinates());
        vtkDataArray *yCoords = rgrid->GetZCoordinates()->NewInstance();
        yCoords->SetNumberOfTuples(nL+1);
        for (int l = 0; l < nL+1; ++l)
            yCoords->SetTuple1(l, l*dx);
        ogrid->SetYCoordinates(yCoords);
        yCoords->Delete();

        vtkDataArray *zCoords = rgrid->GetZCoordinates()->NewInstance();
        zCoords->SetNumberOfTuples(1);
        zCoords->SetTuple1(0,0.0);
        ogrid->SetZCoordinates(zCoords);
        zCoords->Delete();

        vtkDataArray *odata = data->NewInstance();
        odata->SetNumberOfComponents(1);
        odata->SetNumberOfTuples(nK*nL);
        for (int k = 0; k < nK; ++k)
        {
            for (int l = 0; l < nL; ++l)
            {
                int ijk_f[3] = { atts.GetI() - base_index[0], atts.GetJ() - base_index[1], k };
                vtkIdType id_f = rgrid->ComputeCellId(ijk_f);
                int ijk_t[3] = { k, l, 0 };
                vtkIdType id_t = ogrid->ComputeCellId(ijk_t);
                double val = data->GetComponent(id_f, l);
                //std::cout << val << " ";
                odata->SetTuple1(id_t, val);
                if (val < range[0]) range[0] = val;
                if (val > range[1]) range[1] = val;
            }
            //std::cout << std::endl;
        }
        ogrid->GetCellData()->SetScalars(odata);
        odata->Delete();
        return ogrid;
    }
    else
        return 0;
}


// ****************************************************************************
//  Method: avtExtractPointFunction2DFilter::PostExecute
//
//  Purpose:
//      Called after "Execute". Update value range.
//
//  Programmer: Gunther H. Weber 
//  Creation:   June 6, 2013
//
//  Modifications:
//
// ****************************************************************************

void
avtExtractPointFunction2DFilter::PostExecute(void)
{
    avtPluginDataTreeIterator::PostExecute();
    if (outVarName != "")
    {
        avtDataAttributes &atts = GetOutput()->GetInfo().GetAttributes();
        atts.GetThisProcsOriginalDataExtents(outVarName.c_str())->Set(range);
        atts.GetThisProcsActualDataExtents(outVarName.c_str())->Set(range);
    }
}


// ****************************************************************************
//  Method: avtExtractPointFunction2DFilter::ModifyContract
//
//  Purpose:
//      Creates a contract the removes the operator-created-expression.
//
//  Programmer: ghweber -- generated by xml2avt
//  Creation:   Tue Jun 4 10:46:56 PDT 2013
//
// ****************************************************************************

avtContract_p
avtExtractPointFunction2DFilter::ModifyContract(avtContract_p in_contract)
{
    avtContract_p rv;
    if (strncmp(pipelineVariable, "operators/ExtractPointFunction2D/", strlen("operators/ExtractPointFunction2D/")) == 0)
    {
        outVarName = pipelineVariable;
        const char *justTheVar = pipelineVariable + strlen("operators/ExtractPointFunction2D/");
        avtDataRequest_p dr = new avtDataRequest(in_contract->GetDataRequest(), justTheVar);
        rv = new avtContract(in_contract, dr);
    }
    else
    {
        rv = new avtContract(in_contract);
    }
    return rv;
}


// ****************************************************************************
//  Method: avtExtractPointFunction2DFilter::UpdateDataObjectInfo
//
//  Purpose:
//      Tells output that we have a new variable.
//
//  Programmer: ghweber -- generated by xml2avt
//  Creation:   Tue Jun 4 10:46:56 PDT 2013
//
// ****************************************************************************

void
avtExtractPointFunction2DFilter::UpdateDataObjectInfo(void)
{
    avtDataAttributes &atts = GetOutput()->GetInfo().GetAttributes();
    atts.SetSpatialDimension(2);
    atts.SetTopologicalDimension(2);
    GetOutput()->GetInfo().GetValidity().InvalidateSpatialMetaData();

   if (outVarName != "")
   {
       if (! atts.ValidVariable(outVarName))
       {
           atts.AddVariable(outVarName.c_str());
           atts.SetActiveVariable(outVarName.c_str());
           atts.SetVariableDimension(1);
           atts.SetVariableType(AVT_SCALAR_VAR);
           atts.SetCentering(AVT_ZONECENT);
       }
   }
}
