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
//                     avtLocalizedCompactnessExpression.C                   //
// ************************************************************************* //

#include <avtLocalizedCompactnessExpression.h>

#include <vtkDataArray.h>
#include <vtkDataSet.h>
#include <vtkPointData.h>
#include <vtkRectilinearGrid.h>

#include <ExpressionException.h>

// ****************************************************************************
//  Method: avtLocalizedCompactnessExpression constructor
//
//  Purpose:
//      Defines the constructor.  Note: this should not be inlined in the
//      header because it causes problems for certain compilers.
//
//  Programmer: Hank Childs
//  Creation:   April 29, 2006
//
// ****************************************************************************

avtLocalizedCompactnessExpression::avtLocalizedCompactnessExpression()
{
}


// ****************************************************************************
//  Method: avtLocalizedCompactnessExpression destructor
//
//  Purpose:
//      Defines the destructor.  Note: this should not be inlined in the header
//      because it causes problems for certain compilers.
//
//  Programmer: Hank Childs
//  Creation:   April 29, 2006
//
// ****************************************************************************

avtLocalizedCompactnessExpression::~avtLocalizedCompactnessExpression()
{
    ;
}


// ****************************************************************************
//  Method: avtLocalizedCompactnessExpression::DeriveVariable
//
//  Purpose:
//      Calculates the localized compactness at every point.
//
//  Arguments:
//      inDS      The input dataset.
//
//  Returns:      The derived variable.  The calling class must free this
//                memory.
//
//  Programmer:   Hank Childs
//  Creation:     April 29, 2006
//
//  Modifications:
//
//    Hank Childs, Fri Sep 15 16:55:45 PDT 2006
//    Add support for XY meshes (as opposed to RZ meshes).
//
//    Kathleen Biagas, Wed Apr 4 11:56:10 PDT 2012
//    Set output array's data type to same as input array. Use double for 
//    calculations throughout.
//
// ****************************************************************************

vtkDataArray *
avtLocalizedCompactnessExpression::DeriveVariable(vtkDataSet *in_ds, int currentDomainsIndex)
{
    int   i, j, k, ii, jj, kk;

    if (in_ds->GetDataObjectType() != VTK_RECTILINEAR_GRID)
    {
        EXCEPTION2(ExpressionException, outputVariableName,
                   "The localized compactness expression "
                   "can only be calculated on rectilinear grids.  Use the "
                   "resample operator to generate a rectilinear grid of "
                   "this object.  You must also use the DeferExpression "
                   "operator to defer the evaluation of this expression until "
                   "after the resample operator.  The resample"
                   " and defer expression operators are available through "
                   "the plugin manager located under the Options menu");
    }

    vtkRectilinearGrid *rgrid = (vtkRectilinearGrid *) in_ds;
    int dims[3];
    rgrid->GetDimensions(dims);
    
    bool isRZ = (GetInput()->GetInfo().GetAttributes().GetMeshCoordType()
                 == AVT_RZ);

    const char *varname = activeVariable;
    vtkDataArray *var = in_ds->GetPointData()->GetArray(varname);
    if (var == NULL)
    {
        EXCEPTION2(ExpressionException, outputVariableName,
                   "The variable of which you take the localized"
                  " compactness must be nodal.  Recenter your data and try"
                  " again.");
    }

    int nOut = dims[0]*dims[1]*dims[2];
    vtkDataArray *arr = var->NewInstance();
    arr->SetNumberOfTuples(nOut);

    vtkDataArray *xd = rgrid->GetXCoordinates();
    vtkDataArray *yd = rgrid->GetYCoordinates();
    vtkDataArray *zd = rgrid->GetZCoordinates();

    double radius = 0.1;
    if (dims[2] <= 1)
    {
        for (i = 0 ; i < dims[0] ; i++)
        {
            for (j = 0 ; j < dims[1] ; j++)
            {
                // For each point (i, j), find the bounding box in terms of
                // logical dimensions for the radius.
                int iMin = -1, iMax = -1;
                double x = xd->GetTuple1(i);
                for (ii = 0 ; ii < dims[0]; ii++)
                {
                    if (iMin < 0)
                    {
                        double x2 = xd->GetTuple1(ii);
                        if (x2 > x-radius)
                            iMin = ii;
                    }
                    if (iMax < 0)
                    {
                        double x2 = xd->GetTuple1(ii);
                        if (x2 > x+radius)
                            iMax = ii;
                    }
                }
                iMin = (iMin < 0 ? 0 : iMin);
                iMax = (iMax < 0 ? dims[0] : iMax);
                int jMin = -1, jMax = -1;
                double y = yd->GetTuple1(j);
                for (jj = 0 ; jj < dims[1]; jj++)
                {
                    if (jMin < 0)
                    {
                        double y2 = yd->GetTuple1(jj);
                        if (y2 > y-radius)
                            jMin = jj;
                    }
                    if (jMax < 0)
                    {
                        double y2 = yd->GetTuple1(jj);
                        if (y2 > y+radius)
                            jMax = jj;
                    }
                }
                jMin = (jMin < 0 ? 0 : jMin);
                jMax = (jMax < 0 ? dims[1] : jMax);

                // Now that we have found the neighborhood, iterate over
                // that neighborhood.
                double totalWeight = 0.;
                double weightInside = 0.;
                for (ii = iMin ; ii < iMax ; ii++)
                    for (jj = jMin ; jj < jMax ; jj++)
                    {
                        double x2 = xd->GetTuple1(ii);
                        double y2 = yd->GetTuple1(jj);
                        double rad_squared = (x2-x)*(x2-x) + (y2-y)*(y2-y);
                        if (rad_squared > radius*radius)
                            continue;
                        totalWeight += (isRZ ? y2 : 1.);
                        int idx = jj*dims[0] + ii;
                        if (var->GetTuple1(idx) != 0.)
                            weightInside += (isRZ ? y2 : 1.);
                    }
                int idx = j*dims[0] + i;
                double compactness = weightInside / totalWeight;
                arr->SetTuple1(idx, compactness);
            }
        }
    }
    else
    {
        for (i = 0 ; i < dims[0] ; i++)
        {
            for (j = 0 ; j < dims[1] ; j++)
            {
                for (k = 0 ; k < dims[2] ; k++)
                {
                    // For each point (i,j,k),find the bounding box in terms of
                    // logical dimensions for the radius.
                    int iMin = -1, iMax = -1;
                    double x = xd->GetTuple1(i);
                    for (ii = 0 ; ii < dims[0]; ii++)
                    {
                        if (iMin < 0)
                        {
                            double x2 = xd->GetTuple1(ii);
                            if (x2 > x-radius)
                                iMin = ii;
                        }
                        if (iMax < 0)
                        {
                            double x2 = xd->GetTuple1(ii);
                            if (x2 > x+radius)
                                iMax = ii;
                        }
                    }
                    iMin = (iMin < 0 ? 0 : iMin);
                    iMax = (iMax < 0 ? dims[0] : iMax);
                    int jMin = -1, jMax = -1;
                    double y = yd->GetTuple1(j);
                    for (jj = 0 ; jj < dims[1]; jj++)
                    {
                        if (jMin < 0)
                        {
                            double y2 = yd->GetTuple1(jj);
                            if (y2 > y-radius)
                                jMin = jj;
                        }
                        if (jMax < 0)
                        {
                            double y2 = yd->GetTuple1(jj);
                            if (y2 > y+radius)
                                jMax = jj;
                        }
                    }
                    jMin = (jMin < 0 ? 0 : jMin);
                    jMax = (jMax < 0 ? dims[1] : jMax);
                    int kMin = -1, kMax = -1;
                    double z = zd->GetTuple1(k);
                    for (kk = 0 ; kk < dims[2]; kk++)
                    {
                        if (kMin < 0)
                        {
                            double z2 = zd->GetTuple1(kk);
                            if (z2 > z-radius)
                                kMin = kk;
                        }
                        if (kMax < 0)
                        {
                            double z2 = zd->GetTuple1(kk);
                            if (z2 > z+radius)
                                kMax = kk;
                        }
                    }
                    kMin = (kMin < 0 ? 0 : kMin);
                    kMax = (kMax < 0 ? dims[2] : kMax);

                    // Now that we have found the neighborhood, iterate over
                    // that neighborhood.
                    double totalWeight = 0.;
                    double weightInside = 0.;
                    for (ii = iMin ; ii < iMax ; ii++)
                        for (jj = jMin ; jj < jMax ; jj++)
                            for (kk = kMin ; kk < kMax ; kk++)
                            {
                                double x2 = xd->GetTuple1(ii);
                                double y2 = yd->GetTuple1(jj);
                                double z2 = zd->GetTuple1(kk);
                                double rad_squared = (x2-x)*(x2-x) 
                                               + (y2-y)*(y2-y) + (z2-z)*(z2-z);
                                if (rad_squared > radius*radius)
                                    continue;
                                totalWeight += 1;
                                int idx = jj*dims[0] + ii;
                                if (var->GetTuple1(idx) != 0.)
                                    weightInside += 1;
                            }
                    int idx = k*dims[0]*dims[1] + j*dims[0] + i;
                    double compactness = weightInside / totalWeight;
                    arr->SetTuple1(idx, compactness);
                }
            }
        }
    }

    return arr;
}


