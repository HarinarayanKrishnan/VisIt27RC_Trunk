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
//                         avtZoneTypeExpression.C                           //
// ************************************************************************* //

#include <avtZoneTypeExpression.h>

#include <vtkCellData.h>
#include <vtkCellType.h>
#include <vtkDataSet.h>
#include <vtkIntArray.h>

// ****************************************************************************
//  Method: avtZoneTypeExpression constructor
//
//  Purpose:
//      Defines the constructor.  Note: this should not be inlined in the
//      header because it causes problems for certain compilers.
//
//  Programmer: Mark C. Miller 
//  Creation:   November 15, 2006 
//
// ****************************************************************************

avtZoneTypeExpression::avtZoneTypeExpression()
{
    ;
}


// ****************************************************************************
//  Method: avtZoneTypeExpression destructor
//
//  Purpose:
//      Defines the destructor.  Note: this should not be inlined in the header
//      because it causes problems for certain compilers.
//
//  Programmer: Mark C. Miller
//  Creation:   November 15, 2006 
//
// ****************************************************************************

avtZoneTypeExpression::~avtZoneTypeExpression()
{
    ;
}


// ****************************************************************************
//  Method: avtZoneTypeExpression::DeriveVariable
//
//  Purpose:
//      Determines the zon type of a cell.
//
//  Arguments:
//      inDS      The input dataset.
//
//  Returns:      The derived variable.  The calling class must free this
//                memory.
//
//  Programmer:   Mark C. Miller 
//  Creation:     November 15, 2006 
//
//  Modifications:
//    Mark C. Miller, Wed Apr  2 09:46:47 PDT 2008
//    Added case statement to set character for zone type.
//
// ****************************************************************************

vtkDataArray *
avtZoneTypeExpression::DeriveVariable(vtkDataSet *in_ds, int currentDomainsIndex)
{
    vtkIdType ncells = in_ds->GetNumberOfCells();

    vtkIntArray *rv = vtkIntArray::New();
    rv->SetNumberOfTuples(ncells);
    for (vtkIdType i = 0 ; i < ncells ; i++)
    {
        int val = (int) '?';
        switch (in_ds->GetCellType(i))
        {
            // 2D cell types, lower case letters
            case VTK_EMPTY_CELL:     val = (int) 'e'; break;
            case VTK_VERTEX:         val = (int) 'v'; break;
            case VTK_POLY_VERTEX:    val = (int) 'w'; break;
            case VTK_LINE:           val = (int) 'l'; break;
            case VTK_POLY_LINE:      val = (int) 'm'; break;
            case VTK_TRIANGLE:       val = (int) 't'; break;
            case VTK_TRIANGLE_STRIP: val = (int) 's'; break;
            case VTK_POLYGON:        val = (int) 'p'; break;
            case VTK_PIXEL:          val = (int) 'x'; break;
            case VTK_QUAD:           val = (int) 'q'; break;

            // 3D cell types, upper case letters
            case VTK_TETRA:          val = (int) 'T'; break;
            case VTK_VOXEL:          val = (int) 'V'; break;
            case VTK_HEXAHEDRON:     val = (int) 'H'; break;
            case VTK_WEDGE:          val = (int) 'W'; break;
            case VTK_PYRAMID:        val = (int) 'P'; break;
        }            

        rv->SetTuple1(i, val);
    }

    return rv;
}

// ****************************************************************************
//  Method: avtZoneTypeExpression::UpdateDataObjectInfo
//
//  Purpose: Tell the output this is a treat as ascii variable
//
//  Programmer:   Mark C. Miller 
//  Creation:     April 2, 2008 
//
// ****************************************************************************

void
avtZoneTypeExpression::UpdateDataObjectInfo(void)
{
    avtSingleInputExpressionFilter::UpdateDataObjectInfo();

    avtDataAttributes &outAtts = GetOutput()->GetInfo().GetAttributes();
    outAtts.SetTreatAsASCII(true);
}
