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

#ifndef COLORATTRIBUTE_H
#define COLORATTRIBUTE_H
#include <state_exports.h>
#include <AttributeSubject.h>



// ****************************************************************************
// Class: ColorAttribute
//
// Purpose:
//    This class contains RGBA color information
//
// Notes:      Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

class STATE_API ColorAttribute : public AttributeSubject
{
public:

    // These constructors are for objects of this class
    ColorAttribute();
    ColorAttribute(const ColorAttribute &obj);
protected:
    // These constructors are for objects derived from this class
    ColorAttribute(private_tmfs_t tmfs);
    ColorAttribute(const ColorAttribute &obj, private_tmfs_t tmfs);
public:
    virtual ~ColorAttribute();

    virtual ColorAttribute& operator = (const ColorAttribute &obj);
    virtual bool operator == (const ColorAttribute &obj) const;
    virtual bool operator != (const ColorAttribute &obj) const;
private:
    void Init();
    void Copy(const ColorAttribute &obj);
public:

    virtual const std::string TypeName() const;
    virtual bool CopyAttributes(const AttributeGroup *);
    virtual AttributeSubject *CreateCompatible(const std::string &) const;
    virtual AttributeSubject *NewInstance(bool) const;

    // Property selection methods
    virtual void SelectAll();
    void SelectColor();

    // Property setting methods
    void SetColor(const unsigned char *color_);

    // Property getting methods
    const unsigned char *GetColor() const;
          unsigned char *GetColor();

    // Persistence methods
    virtual bool CreateNode(DataNode *node, bool completeSave, bool forceAdd);
    virtual void SetFromNode(DataNode *node);


    // Keyframing methods
    virtual std::string               GetFieldName(int index) const;
    virtual AttributeGroup::FieldType GetFieldType(int index) const;
    virtual std::string               GetFieldTypeName(int index) const;
    virtual bool                      FieldsEqual(int index, const AttributeGroup *rhs) const;

    // User-defined methods
    ColorAttribute(int red, int green, int blue, int alpha = 255);
    void SetRgb(int red, int green, int blue);
    void SetRgba(int red, int green, int blue, int alpha);
    void GetRgba(double c[4]) const;
    void SetRed(int red);
    void SetGreen(int green);
    void SetBlue(int blue);
    void SetAlpha(int alpha);
    int Red() const;
    int Green() const;
    int Blue() const;
    int Alpha() const;
    void SetRgba2(double c[4]);
    bool SetByName(const char *const colorName);
    const char *GetByName() const;
    static bool RgbStrToRgb(const char *const s, unsigned char rgb[3]);
    static void RgbToRgbStr(unsigned char rgb[3], char *s);
    static ColorAttribute Blend(const ColorAttribute &a, const ColorAttribute &b, double t);

    // IDs that can be used to identify fields in case statements
    enum {
        ID_color = 0,
        ID__LAST
    };

private:
    unsigned char color[4];

    // Static class format string for type map.
    static const char *TypeMapFormatString;
    static const private_tmfs_t TmfsStruct;
};
#define COLORATTRIBUTE_TMFS "U"

#endif
