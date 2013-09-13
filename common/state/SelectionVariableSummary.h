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

#ifndef SELECTIONVARIABLESUMMARY_H
#define SELECTIONVARIABLESUMMARY_H
#include <state_exports.h>
#include <string>
#include <AttributeSubject.h>


// ****************************************************************************
// Class: SelectionVariableSummary
//
// Purpose:
//    Contains a summary of a variable used in a selection
//
// Notes:      Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

class STATE_API SelectionVariableSummary : public AttributeSubject
{
public:
    // These constructors are for objects of this class
    SelectionVariableSummary();
    SelectionVariableSummary(const SelectionVariableSummary &obj);
protected:
    // These constructors are for objects derived from this class
    SelectionVariableSummary(private_tmfs_t tmfs);
    SelectionVariableSummary(const SelectionVariableSummary &obj, private_tmfs_t tmfs);
public:
    virtual ~SelectionVariableSummary();

    virtual SelectionVariableSummary& operator = (const SelectionVariableSummary &obj);
    virtual bool operator == (const SelectionVariableSummary &obj) const;
    virtual bool operator != (const SelectionVariableSummary &obj) const;
private:
    void Init();
    void Copy(const SelectionVariableSummary &obj);
public:

    virtual const std::string TypeName() const;
    virtual bool CopyAttributes(const AttributeGroup *);
    virtual AttributeSubject *CreateCompatible(const std::string &) const;
    virtual AttributeSubject *NewInstance(bool) const;

    // Property selection methods
    virtual void SelectAll();
    void SelectName();
    void SelectHistogram();

    // Property setting methods
    void SetName(const std::string &name_);
    void SetMinimum(double minimum_);
    void SetMaximum(double maximum_);
    void SetHistogram(const double *histogram_);

    // Property getting methods
    const std::string &GetName() const;
          std::string &GetName();
    double            GetMinimum() const;
    double            GetMaximum() const;
    const double      *GetHistogram() const;
          double      *GetHistogram();

    // User-defined methods
    void SetHistogram(int index, double value);
    void NormalizedHistogram(float hist[256]) const;

    // IDs that can be used to identify fields in case statements
    enum {
        ID_name = 0,
        ID_minimum,
        ID_maximum,
        ID_histogram,
        ID__LAST
    };

private:
    std::string name;
    double      minimum;
    double      maximum;
    double      histogram[256];

    // Static class format string for type map.
    static const char *TypeMapFormatString;
    static const private_tmfs_t TmfsStruct;
};
#define SELECTIONVARIABLESUMMARY_TMFS "sddD"

#endif
