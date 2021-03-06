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
//  File: avtCylinderFilter.C
// ************************************************************************* //

#include <avtCylinderFilter.h>

#include <vtkVisItClipper.h>
#include <vtkCylinder.h>
#include <vtkImplicitBoolean.h>
#include <vtkMath.h>
#include <vtkPlane.h>
#include <vtkPolyData.h>
#include <vtkUnstructuredGrid.h>
#include <vtkTransform.h>


// ****************************************************************************
//  Method: avtCylinderFilter constructor
//
//  Programmer: Hank Childs
//  Creation:   Tue Oct 21 13:17:14 PST 2003
//
// ****************************************************************************

avtCylinderFilter::avtCylinderFilter()
{
    cylinderSlice = NULL;
}


// ****************************************************************************
//  Method: avtCylinderFilter destructor
//
//  Programmer: Hank Childs
//  Creation:   Tue Oct 21 13:17:14 PST 2003
//
//  Modifications:
//
// ****************************************************************************

avtCylinderFilter::~avtCylinderFilter()
{
    if (cylinderSlice != NULL)
    {
        cylinderSlice->Delete();
        cylinderSlice = NULL;
    }
}


// ****************************************************************************
//  Method:  avtCylinderFilter::Create
//
//  Programmer: Hank Childs
//  Creation:   Tue Oct 21 13:17:14 PST 2003
//
// ****************************************************************************

avtFilter *
avtCylinderFilter::Create()
{
    return new avtCylinderFilter();
}


// ****************************************************************************
//  Method:      avtCylinderFilter::SetAtts
//
//  Purpose:
//      Sets the state of the filter based on the attribute object.
//
//  Arguments:
//      a        The attributes to use.
//
//  Programmer: Hank Childs
//  Creation:   Tue Oct 21 13:17:14 PST 2003
//
//  Modifications:
//    Brad Whitlock, Wed Feb 22 10:05:13 PST 2012
//    I fixed a deg->rad transformation.
//
// ****************************************************************************

void
avtCylinderFilter::SetAtts(const AttributeGroup *a)
{
    atts = *(const CylinderAttributes*)a;
    const double *pt1 = atts.GetPoint1();
    const double *pt2 = atts.GetPoint2();
    double radius = atts.GetRadius();
    vtkCylinder *c = vtkCylinder::New();
    c->SetRadius(radius);

    double normal[3];
    normal[0] = pt2[0] - pt1[0];
    normal[1] = pt2[1] - pt1[1];
    normal[2] = pt2[2] - pt1[2];
   
    //
    // We are going to rotate the cylinder's true direction of project to be
    // along the y-axis.  Formula for doing this: calculate the cross product
    // and then rotate along the axis of the vector formed by the cross
    // product.  The amount to rotate is the angle between cylinder's true
    // direction of projection and the y-axis -- that is their dot product.
    //
    vtkTransform *trans = vtkTransform::New();
    double normal_normalized[3];
    double mag = sqrt(normal[0]*normal[0] + normal[1]*normal[1]
                      + normal[2]*normal[2]);
    normal_normalized[0] = normal[0] / mag;
    normal_normalized[1] = normal[1] / mag;
    normal_normalized[2] = normal[2] / mag;
    double cross[3];
    cross[0] = normal_normalized[2];
    cross[1] = 0.;
    cross[2] = -normal_normalized[0];
    if (cross[0] != 0. || cross[1] != 0. || cross[2] != 0.)
    {
        double dot = normal_normalized[1];
        double angle = -acos(dot);
        angle = vtkMath::DegreesFromRadians(angle);
        trans->RotateWXYZ(angle, cross[0], cross[1], cross[2]);
    }
    c->SetTransform(trans);
    trans->Translate(-pt1[0], -pt1[1], -pt1[2]);
    trans->Delete();

    //
    // Now add two planes to make the top part and the bottom part of the
    // cylinder.
    //
    vtkPlane *top_part = vtkPlane::New();
    top_part->SetOrigin(pt2[0], pt2[1], pt2[2]);
    top_part->SetNormal(normal[0], normal[1], normal[2]);
    vtkPlane *bottom_part = vtkPlane::New();
    bottom_part->SetOrigin(pt1[0], pt1[1], pt1[2]);
    bottom_part->SetNormal(-normal_normalized[0], -normal_normalized[1], 
                           -normal_normalized[2]);

    //
    // Make a VTK function that will intersect the cylinder and its top and
    // bottom.
    //
    vtkImplicitBoolean *allFunctions = vtkImplicitBoolean::New();
    allFunctions->AddFunction(c);
    allFunctions->AddFunction(top_part);
    allFunctions->AddFunction(bottom_part);
    allFunctions->SetOperationTypeToIntersection();

    c->Delete();
    top_part->Delete();
    bottom_part->Delete();
    
    cylinderSlice = allFunctions;
}


// ****************************************************************************
//  Method: avtCylinderFilter::Equivalent
//
//  Purpose:
//      Returns true if creating a new avtCylinderFilter with the given
//      parameters would result in an equivalent avtCylinderFilter.
//
//  Programmer: Hank Childs
//  Creation:   Tue Oct 21 13:17:14 PST 2003
//
// ****************************************************************************

bool
avtCylinderFilter::Equivalent(const AttributeGroup *a)
{
    return (atts == *(CylinderAttributes*)a);
}


// ****************************************************************************
//  Method: avtCylinderFilter::ExecuteData
//
//  Purpose:
//      Sends the specified input and output through the Cylinder filter.
//
//  Arguments:
//      in_ds      The input dataset.
//      <unused>   The domain number.
//      <unused>   The label.
//
//  Returns:       The output dataset.
//
//  Programmer: Hank Childs
//  Creation:   Tue Oct 21 13:17:14 PST 2003
//
//  Modifications:
//    Kathleen Bonnell, Thu Apr 28 10:28:22 PDT 2005
//    Use vtkVisItCutter instead of vtkCutter since it has logic to handle
//    CellData correctly.
//
//    Brad Whitlock, Fri May  6 13:41:40 PDT 2011
//    Do clipping instead of cutting so we can leave the dataset interior.
//
//   Dave Pugmire, Fri Aug 30 14:43:48 EDT 2013
//   Add Inverse clipping option.
//
// ****************************************************************************

vtkDataSet *
avtCylinderFilter::ExecuteData(vtkDataSet *in_ds, int, std::string)
{
    vtkVisItClipper *clipper = vtkVisItClipper::New();
    if (atts.GetInverse())
        clipper->SetInsideOut(false);
    else
        clipper->SetInsideOut(true);
    
    clipper->SetClipFunction(cylinderSlice);
    clipper->SetInputData(in_ds);
    clipper->Update();
    vtkDataSet *rv = clipper->GetOutput();

    ManageMemory(rv);
    clipper->Delete();

    return rv;
}


// ****************************************************************************
//  Method: avtCylinderFilter::UpdateDataObjectInfo
//
//  Purpose:
//      Changes to topological dimension of the output to be one less that the
//      input.
//
//  Programmer: Hank Childs
//  Creation:   October 21, 2003
//
//  Modifications:
//    Kathleen Bonnell, Thu Mar  2 14:26:06 PST 2006 
//    Set ZonesSplit.
//
//    Brad Whitlock, Fri May  6 13:41:20 PDT 2011
//    Do not reduce the topological dimension anymore.
//
// ****************************************************************************

void
avtCylinderFilter::UpdateDataObjectInfo(void)
{
    avtDataAttributes &inAtts      = GetInput()->GetInfo().GetAttributes();
    avtDataAttributes &outAtts     = GetOutput()->GetInfo().GetAttributes();
    avtDataValidity   &outValidity = GetOutput()->GetInfo().GetValidity();

    outValidity.InvalidateZones();
    outValidity.ZonesSplit();
}


