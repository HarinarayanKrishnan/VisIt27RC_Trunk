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
//                             avtCMFEExpression.h                           //
// ************************************************************************* //

#ifndef AVT_CMFE_EXPRESSION_H
#define AVT_CMFE_EXPRESSION_H

#include <avtExpressionFilter.h>
#include <avtSILRestriction.h>
#include <ExprNode.h>

class     vtkDataArray;
class     ArgsExpr;
class     ExprPipelineState;
class     ConstExpr;
class     avtDatabase;

// ****************************************************************************
//  Class: avtCMFEExpression
//
//  Purpose:
//      The base class for doing CMFE expressions.
//          
//  Programmer: Hank Childs
//  Creation:   August 26, 2005
//
//  Modifications:
//
//    Hank Childs, Fri Sep  9 09:38:08 PDT 2005
//    Add support for expressions.  Also add GetVariableDimension, which is
//    necessary for non-scalars.
//
//    Hank Childs, Thu Jan  5 15:34:17 PST 2006
//    Do a better job of getting centering of input variable.  Also add 
//    support for a "default" variable for when we are sampling onto 
//    non-overlapping meshes.
//
//    Hank Childs, Thu Jan 12 10:13:17 PST 2006
//    Change the number of variable arguments when we have a default
//    argument.
//
//    Hank Childs, Thu Apr 10 16:10:33 PDT 2008
//    Add a data member to manage making consistent ghost zone requests.
//
//    Hank Childs, Mon Mar 23 11:02:55 CDT 2009
//    Add data member for on demand streaming.
//
//    Hank Childs, Tue Mar 24 13:18:10 CDT 2009
//    Add data member for data selections.
//
//    Hank Childs, Thu Aug 26 23:36:03 PDT 2010
//    Add data members for extents calculations.
//
//    Hank Chids, Tue Mar 13 08:26:09 PDT 2012
//    Add method "OnlyRequiresSpatiallyOverlappingData".
//
//    Hank Childs, Thu Aug  1 09:33:48 PDT 2013
//    Increase the number of variable arguments, to make CMFE names more
//    distinct and less likely to collide.
//
// ****************************************************************************

class EXPRESSION_API avtCMFEExpression : public avtExpressionFilter
{
  public:
                              avtCMFEExpression();
    virtual                  ~avtCMFEExpression();

    virtual const char       *GetType(void){ return "avtCMFEExpression"; };
    virtual const char       *GetDescription(void)
                                           {return "Evaluating field";};
    virtual void              ProcessArguments(ArgsExpr*, ExprPipelineState *);
    virtual int               NumVariableArguments()
                                    { return (HasDefaultVariable() ? 3 : 2); };
    virtual int               GetVariableDimension() { return varDim; };
    virtual bool              IsPointVariable(void) { return isNodal; };
    virtual void              AddInputVariableName(const char *);

  protected:
    bool                      issuedWarning;
    std::string               db;
    std::string               var;
    TimeExpr::Type            timeType;
    int                       timeIndex;
    int                       cycle;
    double                    dtime;
    bool                      isDelta;
    int                       firstDBTime;
    avtSILRestriction_p       firstDBSIL;
    avtGhostDataType          ghostNeeds;
    bool                      onDemandProcessing;
    bool                      replicateSingleDomainOnAllProcessors;
    std::vector<avtDataSelection_p> dataSels;
    std::string               argument_expression;
    int                       varDim;
    bool                      isNodal;
    std::vector<std::string>  varnames;
    std::vector<std::string>  varExtentsList;
    bool                      calculateMeshExtents;
    bool                      initialSILHasData;

    virtual void              PreExecute(void);
    virtual void              Execute(void);

    virtual avtDataTree_p     PerformCMFE(avtDataTree_p, avtDataTree_p,
                                          const std::string &, 
                                          const std::string &) = 0;
    virtual void              ExamineContract(avtContract_p);
    virtual bool              UseIdenticalSIL(void) { return false; };
    virtual bool              HasDefaultVariable(void) { return false; };
    int                       GetTimestate(ref_ptr<avtDatabase>);

    // We can reduce how much data is loaded if we know that we only
    // need the spatially overlapping data.
    virtual bool              OnlyRequiresSpatiallyOverlappingData()
                                            { return false; };
};


#endif


