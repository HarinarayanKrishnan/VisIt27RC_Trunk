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

#ifndef PSEUDOCOLORATTRIBUTES_H
#define PSEUDOCOLORATTRIBUTES_H
#include <string>
#include <AttributeSubject.h>

#include <visitstream.h>

// ****************************************************************************
// Class: PseudocolorAttributes
//
// Purpose:
//    Attributes for the pseudocolor plot
//
// Notes:      Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

class PseudocolorAttributes : public AttributeSubject
{
public:
    enum Centering
    {
        Natural,
        Nodal,
        Zonal
    };
    enum Scaling
    {
        Linear,
        Log,
        Skew
    };
    enum LimitsMode
    {
        OriginalData,
        CurrentPlot
    };
    enum PointType
    {
        Box,
        Axis,
        Icosahedron,
        Octahedron,
        Tetrahedron,
        SphereGeometry,
        Point,
        Sphere
    };
    enum Opacity
    {
        Explicit,
        ColorTable
    };

    // These constructors are for objects of this class
    PseudocolorAttributes();
    PseudocolorAttributes(const PseudocolorAttributes &obj);
protected:
    // These constructors are for objects derived from this class
    PseudocolorAttributes(private_tmfs_t tmfs);
    PseudocolorAttributes(const PseudocolorAttributes &obj, private_tmfs_t tmfs);
public:
    virtual ~PseudocolorAttributes();

    virtual PseudocolorAttributes& operator = (const PseudocolorAttributes &obj);
    virtual bool operator == (const PseudocolorAttributes &obj) const;
    virtual bool operator != (const PseudocolorAttributes &obj) const;
private:
    void Init();
    void Copy(const PseudocolorAttributes &obj);
public:

    virtual const std::string TypeName() const;
    virtual bool CopyAttributes(const AttributeGroup *);
    virtual AttributeSubject *CreateCompatible(const std::string &) const;
    virtual AttributeSubject *NewInstance(bool) const;

    // Property selection methods
    virtual void SelectAll();
    void SelectColorTableName();
    void SelectPointSizeVar();

    // Property setting methods
    void SetLegendFlag(bool legendFlag_);
    void SetLightingFlag(bool lightingFlag_);
    void SetMinFlag(bool minFlag_);
    void SetMaxFlag(bool maxFlag_);
    void SetCentering(Centering centering_);
    void SetScaling(Scaling scaling_);
    void SetLimitsMode(LimitsMode limitsMode_);
    void SetMin(double min_);
    void SetMax(double max_);
    void SetPointSize(double pointSize_);
    void SetPointType(PointType pointType_);
    void SetSkewFactor(double skewFactor_);
    void SetOpacity(double opacity_);
    void SetColorTableName(const std::string &colorTableName_);
    void SetInvertColorTable(bool invertColorTable_);
    void SetSmoothingLevel(int smoothingLevel_);
    void SetPointSizeVarEnabled(bool pointSizeVarEnabled_);
    void SetPointSizeVar(const std::string &pointSizeVar_);
    void SetPointSizePixels(int pointSizePixels_);
    void SetLineStyle(int lineStyle_);
    void SetLineWidth(int lineWidth_);
    void SetOpacityType(Opacity opacityType_);

    // Property getting methods
    bool              GetLegendFlag() const;
    bool              GetLightingFlag() const;
    bool              GetMinFlag() const;
    bool              GetMaxFlag() const;
    Centering         GetCentering() const;
    Scaling           GetScaling() const;
    LimitsMode        GetLimitsMode() const;
    double            GetMin() const;
    double            GetMax() const;
    double            GetPointSize() const;
    PointType         GetPointType() const;
    double            GetSkewFactor() const;
    double            GetOpacity() const;
    const std::string &GetColorTableName() const;
          std::string &GetColorTableName();
    bool              GetInvertColorTable() const;
    int               GetSmoothingLevel() const;
    bool              GetPointSizeVarEnabled() const;
    const std::string &GetPointSizeVar() const;
          std::string &GetPointSizeVar();
    int               GetPointSizePixels() const;
    int               GetLineStyle() const;
    int               GetLineWidth() const;
    Opacity           GetOpacityType() const;

    // Persistence methods
    virtual bool CreateNode(DataNode *node, bool completeSave, bool forceAdd);
    virtual void SetFromNode(DataNode *node);

    // Enum conversion functions
    static std::string Centering_ToString(Centering);
    static bool Centering_FromString(const std::string &, Centering &);
protected:
    static std::string Centering_ToString(int);
public:
    static std::string Scaling_ToString(Scaling);
    static bool Scaling_FromString(const std::string &, Scaling &);
protected:
    static std::string Scaling_ToString(int);
public:
    static std::string LimitsMode_ToString(LimitsMode);
    static bool LimitsMode_FromString(const std::string &, LimitsMode &);
protected:
    static std::string LimitsMode_ToString(int);
public:
    static std::string PointType_ToString(PointType);
    static bool PointType_FromString(const std::string &, PointType &);
protected:
    static std::string PointType_ToString(int);
public:
    static std::string Opacity_ToString(Opacity);
    static bool Opacity_FromString(const std::string &, Opacity &);
protected:
    static std::string Opacity_ToString(int);
public:

    // Keyframing methods
    virtual std::string               GetFieldName(int index) const;
    virtual AttributeGroup::FieldType GetFieldType(int index) const;
    virtual std::string               GetFieldTypeName(int index) const;
    virtual bool                      FieldsEqual(int index, const AttributeGroup *rhs) const;

    // User-defined methods
    bool ChangesRequireRecalculation(const PseudocolorAttributes &) const;
    void Print(ostream &, bool) const;
    virtual void ProcessOldVersions(DataNode *parentNode, const char *configVersion);

    // IDs that can be used to identify fields in case statements
    enum {
        ID_legendFlag = 0,
        ID_lightingFlag,
        ID_minFlag,
        ID_maxFlag,
        ID_centering,
        ID_scaling,
        ID_limitsMode,
        ID_min,
        ID_max,
        ID_pointSize,
        ID_pointType,
        ID_skewFactor,
        ID_opacity,
        ID_colorTableName,
        ID_invertColorTable,
        ID_smoothingLevel,
        ID_pointSizeVarEnabled,
        ID_pointSizeVar,
        ID_pointSizePixels,
        ID_lineStyle,
        ID_lineWidth,
        ID_opacityType,
        ID__LAST
    };

private:
    bool        legendFlag;
    bool        lightingFlag;
    bool        minFlag;
    bool        maxFlag;
    int         centering;
    int         scaling;
    int         limitsMode;
    double      min;
    double      max;
    double      pointSize;
    int         pointType;
    double      skewFactor;
    double      opacity;
    std::string colorTableName;
    bool        invertColorTable;
    int         smoothingLevel;
    bool        pointSizeVarEnabled;
    std::string pointSizeVar;
    int         pointSizePixels;
    int         lineStyle;
    int         lineWidth;
    int         opacityType;

    // Static class format string for type map.
    static const char *TypeMapFormatString;
    static const private_tmfs_t TmfsStruct;
};
#define PSEUDOCOLORATTRIBUTES_TMFS "bbbbiiidddiddsbibsiiii"

#endif
