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
//                      avtVariableSkewExpression.C                          //
// ************************************************************************* //

#include <avtVariableSkewExpression.h>

#include <vtkDataArray.h>
#include <ExpressionException.h>
#include <vtkSkew.h>

// ****************************************************************************
//  Method: avtVariableSkewExpression constructor
//
//  Purpose:
//    Defines the constructor.  Note: this should not be inlined in the
//    header because it causes problems for certain compilers.
//
//  Programmer: Kathleen Bonnell 
//  Creation:   March 2, 2050 
//
// ****************************************************************************

avtVariableSkewExpression::avtVariableSkewExpression()
{
    ;
}


// ****************************************************************************
//  Method: avtVariableSkewExpression destructor
//
//  Purpose:
//    Defines the destructor.  Note: this should not be inlined in the header
//    because it causes problems for certain compilers.
//
//  Programmer: Kathleen Bonnell 
//  Creation:   March 2, 2005 
//
// ****************************************************************************

avtVariableSkewExpression::~avtVariableSkewExpression()
{
    ;
}


// ****************************************************************************
//  Method: avtVariableSkewExpression::DoOperation
//
//  Purpose:
//    Performs the skew operation on each component,tuple of a data array.
//
//  Arguments:
//    in1           The first input data array.
//    in2           The second data array.
//    out           The output data array.
//    ncomponents   The number of components ('1' for scalar, '2' or '3' for
//                  vectors, etc.)
//    ntuples       The number of tuples (ie 'npoints' or 'ncells')
//
//  Programmer: Kathleen Bonnell 
//  Creation:   March 5, 2005 
//
//  Modifications:
//    Brad Whitlock, Fri Dec 19 15:54:36 PST 2008
//    I made it use vtkSkewValue so we have all of the skew definitions in
//    one place.
//
// ****************************************************************************

void
avtVariableSkewExpression::DoOperation(vtkDataArray *in1, vtkDataArray *in2,
                                   vtkDataArray *out, int ncomponents, 
                                   int ntuples)
{
    vtkIdType in1ncomps = in1->GetNumberOfComponents();
    vtkIdType in2ncomps = in2->GetNumberOfComponents();
    if (in1ncomps == 1 && in2ncomps == 1)
    {
        double *r = in1->GetRange();
        for (int i = 0 ; i < ntuples ; i++)
        {
            double val1 = in1->GetComponent(i, 0);
            double val2 = in2->GetComponent(i, 0);
            double f = vtkSkewValue(val1, r[0], r[1], val2);
            out->SetComponent(i, 0, f);
        }
    }
    else
    {
        EXCEPTION2(ExpressionException, outputVariableName, "Skew can only be "
                   "used on scalar variables.");
    }
}
