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
//  File: avtDisplaceFilter.h
// ************************************************************************* //

#ifndef AVT_Displace_FILTER_H
#define AVT_Displace_FILTER_H

#include <filters_exports.h>

#include <avtDataTreeIterator.h>

class vtkDataSet;


// ****************************************************************************
//  Class: avtDisplaceFilter
//
//  Purpose:
//      A plugin operator for Displace.
//
//  Programmer: childs -- generated by xml2info
//  Creation:   Mon Nov 5 15:35:46 PST 2001
//
//  Modifications:
//    Kathleen Bonnell, Wed Nov 28 16:59:53 PST 2001
//    Added UpdateDataObjectInfo, ModifyContract.
//
//    Hank Childs, Mon May 24 16:16:05 PDT 2004
//    Added PostExecute to manage extents.
//
//    Hank Childs, Tue Jun 29 07:21:32 PDT 2004
//    Removed data member "issuedWarning", since we no longer issue warnings.
//
//    Hank Childs, Tue Sep  5 16:07:45 PDT 2006
//    Added PreExecute method, so we could check validity of "default".
//
//    Hank Childs, Fri May 18 15:59:03 PDT 2007
//    Changed the inheritance, since this is no longer a plugin filter.
//    (This filter was moved from /operators/Displace.)
//
// ****************************************************************************

class AVTFILTERS_API avtDisplaceFilter : public avtDataTreeIterator
{
  public:
                         avtDisplaceFilter();
    virtual             ~avtDisplaceFilter();

    virtual const char  *GetType(void)  { return "avtDisplaceFilter"; };
    virtual const char  *GetDescription(void)
                             { return "Displacing by a vector"; };

    void                 SetFactor(double f) { factor = f; };
    void                 SetVariable(const std::string &v);

  protected:
    double                factor;
    std::string           variable;

    virtual vtkDataSet   *ExecuteData(vtkDataSet *, int, std::string);
    virtual void          PreExecute(void);
    virtual void          PostExecute(void);

    virtual void          UpdateDataObjectInfo(void);
    virtual avtContract_p
                          ModifyContract(avtContract_p);

};


#endif
