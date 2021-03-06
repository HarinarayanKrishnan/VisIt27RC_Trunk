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
//                           avtPseudocolorPlot.C                            //
// ************************************************************************* //

#include <avtPseudocolorPlot.h>

#include <PseudocolorAttributes.h>

#include <avtColorTables.h>
#include <avtExtents.h>
#include <avtLookupTable.h>
#include <avtStaggeringFilter.h>
#include <avtPseudocolorFilter.h>
#include <avtShiftCenteringFilter.h>
#include <avtVariableLegend.h>
#include <avtVariablePointGlyphMapper.h>

#include <DebugStream.h>
#include <InvalidLimitsException.h>

#include <string>
#include <vector>

// ****************************************************************************
//  Method: avtPseudocolorPlot constructor
//
//  Programmer: Hank Childs
//  Creation:   December 28, 2000
//
//  Modifications:
//
//    Kathleen Bonnell, Tue Apr  3 08:56:47 PDT 2001
//    Initialize atts, set the title for the legend.
//
//    Kathleen Bonnell, Tue Apr 24 12:22:01 PDT 2001 
//    Initialize filter.
// 
//    Kathleen Bonnell, Thu Mar 28 14:03:19 PST 2002 
//    Removed previousMode.
// 
//    Hank Childs, Sun Jun 23 12:22:58 PDT 2002
//    Initialize glyphPoints.
//
//    Eric Brugger, Wed Jul 16 11:09:02 PDT 2003
//    Modified to work with the new way legends are managed.
//
//    Kathleen Bonnell, Thu Aug 19 15:29:46 PDT 2004 
//    Replaced varMapper and glyphPoints with glyphMapper. 
//
//    Kathleen Bonnell, Tue Nov  2 11:02:15 PST 2004 
//    Added pcfilter.
//
//    Kathleen Bonnell, Fri Nov 12 11:25:23 PST 2004
//    Replaced avtPointGlyphMapper with avtVariablePointGlyphMapper. 
//
//    Hank Childs, Tue Nov 18 11:58:46 PST 2008
//    Remove the "NoOp" filter, which was almost certainly checked in by
//    mistake.  (It was replacing the compact tree filter, and was causing
//    performance degradation.)
//
//    Jeremy Meredith, Fri Feb 20 15:09:22 EST 2009
//    Added support for using per-color alpha values from a color table
//    (instead of just a single global opacity for the whole plot).
//
// ****************************************************************************

avtPseudocolorPlot::avtPseudocolorPlot()
{
    varLegend = new avtVariableLegend;
    varLegend->SetTitle("Pseudocolor");
    glyphMapper = new avtVariablePointGlyphMapper;

    colorsInitialized = false;
    topoDim = 3;

    avtLUT  = new avtLookupTable;

    filter = NULL;
    pcfilter = NULL;
    staggeringFilter = NULL;
    colorTableIsFullyOpaque = true;

    //
    // This is to allow the legend to reference counted so the behavior can
    // still access it after the plot is deleted.  The legend cannot be
    // reference counted all of the time since we need to know that it is a 
    // VariableLegend.
    //
    varLegendRefPtr = varLegend;
}


// ****************************************************************************
//  Method: avtPseudocolorPlot destructor
//
//  Programmer: Hank Childs
//  Creation:   December 28, 2000
//
//  Modifications:
//
//    Kathleen Bonnell, Tue Apr 24 12:22:01 PDT 2001 
//    Destruct filter.
// 
//    Hank Childs, Sun Jun 23 12:22:58 PDT 2002
//    Destruct glyphPoints.
//
//    Kathleen Bonnell, Thu Aug 19 15:29:46 PDT 2004 
//    No longer using glyphPoints, replaced varMapper with glyphMapepr.
//
//    Kathleen Bonnell, Tue Nov  2 11:02:15 PST 2004 
//    Added pcfilter. 
//
// ****************************************************************************

avtPseudocolorPlot::~avtPseudocolorPlot()
{
    if (filter != NULL)
    {
        delete filter;
        filter = NULL;
    }
 
    if (staggeringFilter != NULL)
    {
        delete staggeringFilter;
        staggeringFilter = NULL;
    }

    if (pcfilter != NULL)
    {
        delete pcfilter;
        pcfilter = NULL;
    }

    if (avtLUT != NULL)
    {
        delete avtLUT;
        avtLUT = NULL;
    }
    if (glyphMapper != NULL)
    {
        delete glyphMapper;
        glyphMapper = NULL;
    }

    //
    // Do not delete the varLegend since it is being held by varLegendRefPtr.
    //
}


// ****************************************************************************
//  Method:  avtPseudocolorPlot::Create
//
//  Purpose:
//    Call the constructor.
//
//  Programmer:  Jeremy Meredith
//  Creation:    March  4, 2001
//
// ****************************************************************************

avtPlot*
avtPseudocolorPlot::Create()
{
    return new avtPseudocolorPlot;
}


// ****************************************************************************
//  Method: avtPseudocolorPlot::GetMapper
//
//  Purpose:
//      Gets a mapper for this plot, it is actually a variable mapper.
//
//  Returns:    The variable mapper typed as its base class mapper.
//
//  Programmer: Hank Childs
//  Creation:   December 28, 2000
//
//  Modifications:
//    Kathleen Bonnell, Thu Aug 19 15:29:46 PDT 2004
//    Replaced varMapper with glyphMapper.
//
// ****************************************************************************

avtMapper *
avtPseudocolorPlot::GetMapper(void)
{
    return glyphMapper;
}


// ****************************************************************************
//  Method: avtPseudocolorPlot::ApplyOperators
//
//  Purpose:
//      Applies the operators associated with a pseudo color plot -- that is
//      none.  It is still unclear to me if a facelist should occur here or
//      if a triangle stripper should occur.
//
//  Arguments:
//      input   The input data object.
//
//  Returns:    The data object after the pseudo color plot has been applied.
//
//  Programmer: Hank Childs
//  Creation:   December 28, 2000
//
//  Modifications:
//
//    Kathleen Bonnell, Tue Apr 24 12:22:01 PDT 2001
//    Added avtShiftCenteringFilter.
//
//    Hank Childs, Fri Jun 15 09:37:30 PDT 2001
//    Use more general data object instead of derived types.
//
//    Hank Childs, Fri Feb  8 18:33:39 PST 2002
//    Removed references to PC atts in the shift centering filter.
//
//    Hank Childs, Sun Jun 23 12:42:22 PDT 2002
//    Add the point to glyph filter if needed.
//
//    Kathleen Bonnell, Mon Jun 24 15:09:37 PDT 2002
//    Only use shift-centering filter if needed, fix potential
//    memory leak.
//
//    Kathleen Bonnell, Tue Oct 22 08:41:29 PDT 2002  
//    Moved the glyph filter to ApplyRenderingTransformtion, so that the 
//    output of this method (used as query input), would still have topo dim
//    of 0 for point meshes, and thus be possibly unquery-able. 
//
//    Hank Childs, Wed Feb 25 15:48:23 PST 2004
//    Do not consider the centering of the current dataset if we are asked to
//    shift the centering.
//
//    Kathleen Bonnell, Tue Nov  2 11:02:15 PST 2004 
//    Added pcfilter for point meshes.
//
//    Kathleen Bonnell, Fri Nov 12 11:25:23 PST 2004
//    Added pcfilter->SetPlotAtts. 
//
//    Cyrus Harrison, Fri Mar  7 11:37:07 PST 2008
//    Moved recentering code to the ApplyRenderingTransormation so centering
//    will not alter query results. (Resolving '8511)
//
// ****************************************************************************

avtDataObject_p
avtPseudocolorPlot::ApplyOperators(avtDataObject_p input)
{
    avtDataObject_p dob = input; 

    if (staggeringFilter != NULL) {
      delete staggeringFilter;
      staggeringFilter = NULL;
    }
    staggeringFilter = new avtStaggeringFilter();
    staggeringFilter->SetInput(dob);
    dob = staggeringFilter->GetOutput();
  
    if (input->GetInfo().GetAttributes().GetTopologicalDimension() == 0)
    {
        if (pcfilter != NULL)
        {
            delete pcfilter;
        }
        pcfilter = new avtPseudocolorFilter();
        pcfilter->SetInput(dob);
        pcfilter->SetPlotAtts(&atts);
        dob = pcfilter->GetOutput();
    }

    return dob;
}

// ****************************************************************************
//  Method: avtPseudocolorPlot::ApplyRenderingTransormation
//
//  Purpose:
//      Applies the rendering transformation associated with a pseudo color 
//      plot.  Namely, a glyph filter for point meshes. 
//
//  Arguments:
//      input   The input data object.
//
//  Returns:    The data object after the glyph points has (possibly) 
//              been applied.
//
//  Programmer: Kathleen Bonnell 
//  Creation:   October 22, 2002 
//
//  Modifications:
//
//    Jeremy Meredith, Fri Dec 20 11:36:03 PST 2002
//    Added scaling of point variables by a scalar field.
//
//    Hank Childs, Thu Aug 21 22:05:14 PDT 2003
//    Added support for different types of point glyphs.
//
//    Kathleen Bonnell, Thu Aug 19 15:29:46 PDT 2004
//    Removed glyph filter (replaced by glyphMapper).  Record topological 
//    dimension for use by EhanceSpecification. 
//
//    Cyrus Harrison, Fri Mar  7 11:37:07 PST 2008
//    Added recentering code previously in ApplyOperators so centering
//    will not alter query results. (Resolving '8511)
//
//    Sean Ahern, Wed Sep 10 13:25:54 EDT 2008
//    For ease of code reading and maintenance, I forced the
//    avtShiftCenteringFilter to take avtCentering type, rather than int.
//
// ****************************************************************************

avtDataObject_p
avtPseudocolorPlot::ApplyRenderingTransformation(avtDataObject_p input)
{
    avtDataObject_p dob = input; 
    topoDim = dob->GetInfo().GetAttributes().GetTopologicalDimension(); 
    if ((atts.GetCentering() == PseudocolorAttributes::Nodal) ||
        (atts.GetCentering() == PseudocolorAttributes::Zonal))
    {
        //
        // It was requested that we shift centering.  If we asked for zonal
        // data and the data is already zonal, then this will effectively
        // be a no-op.
        //
        if (filter != NULL)
        {
            delete filter;
        }
        PseudocolorAttributes::Centering c = atts.GetCentering();
        if (c == PseudocolorAttributes::Nodal)
            filter = new avtShiftCenteringFilter(AVT_NODECENT);
        if (c == PseudocolorAttributes::Zonal)
            filter = new avtShiftCenteringFilter(AVT_ZONECENT);
        filter->SetInput(dob);
        dob = filter->GetOutput();
    }
    return dob;
}


// ****************************************************************************
//  Method: avtPseudocolorPlot::CustomizeBehavior
//
//  Purpose:
//      Customizes the behavior as appropriate for a pseudo color plot.  Right
//      now this only means adding a legend.
//
//  Programmer: Hank Childs
//  Creation:   December 28, 2000
//
//  Modifications:
//
//    Hank Childs, Mon Mar 12 16:59:34 PST 2001
//    Add shift factor.
//
//    Kathleen Bonnell, Tue Apr  3 08:56:47 PDT 2001 
//    Add error checking for Log scale, added calls to retrieve the 
//    variable's range and set it in the legend. (Controls display of
//    limit text, even if artificial limits have been set.)
//
//    Kathleen Bonnell, Thu Apr  5 11:29:33 PDT 2001 
//    Set render order when opacity < 1. 
//    
//    Kathleen Bonnell, Wed Apr 25 14:28:22 PDT 2001 
//    Reflect change in InvalidLimitsException signature. 
//    
//    Jeremy Meredith, Tue Jun  5 20:45:02 PDT 2001
//    Allow storage of attributes as a class member.
//
//    Kathleen Bonnell, Thu Aug 30 16:45:39 PDT 2001 
//    Use avtLUT to set the legend's lut.  Added call to SetScaling
//    before throwing exception.
//
//    Kathleen Bonnell, Wed Mar 13 12:04:53 PST 2002
//    Move code that sets legend range to new method.
//
//    Kathleen Bonnell, Mon Sep 29 13:07:50 PDT 2003 
//    Set AntialiasedRenderOrder. 
//
//    Kathleen Bonnell, Wed Apr 28 17:37:47 PDT 2004 
//    Chagned 'SetLegendRanges' to 'SetLimitsMode', as the legend ranges are
//    dependent upon values set in SetLimitsMode. 
//
//    Jeremy Meredith, Fri Feb 20 15:09:22 EST 2009
//    Added support for using per-color alpha values from a color table
//    (instead of just a single global opacity for the whole plot).
//
// ****************************************************************************

void
avtPseudocolorPlot::CustomizeBehavior()
{
    SetLimitsMode(atts.GetLimitsMode());
    SetPointGlyphSize();

    if( (atts.GetOpacityType() == PseudocolorAttributes::FullyOpaque) ||
        (atts.GetOpacityType() == PseudocolorAttributes::ColorTable &&
         colorTableIsFullyOpaque) ||
        (atts.GetOpacityType() == PseudocolorAttributes::Constant &&
         atts.GetOpacity() == 1.) )
    {
       behavior->SetRenderOrder(DOES_NOT_MATTER);
       behavior->SetAntialiasedRenderOrder(DOES_NOT_MATTER);
    }
    else
    {
       behavior->SetRenderOrder(MUST_GO_LAST);
       behavior->SetAntialiasedRenderOrder(MUST_GO_LAST);
    }

    behavior->SetLegend(varLegendRefPtr);
    if (behavior->GetInfo().GetAttributes().GetTopologicalDimension() <= 1)
        behavior->SetShiftFactor(0.1);
    else
        behavior->SetShiftFactor(0.0);
}


// ****************************************************************************
//  Method: avtPseudocolorPlot::NeedZBufferToCompositeEvenIn2D
//
//  Purpose:
//      Tells the compositer that it needs zbuffer info to composite correctly,
//      in the case that the pseudocolor plot is bleeding over the domain 
//      boundary, which means it can spill into other processor's portion of 
//      image space.
//
//  Programmer: Hank Childs
//  Creation:   August 13, 2008
//
// ****************************************************************************

bool
avtPseudocolorPlot::NeedZBufferToCompositeEvenIn2D(void)
{
    if (behavior->GetInfo().GetAttributes().GetTopologicalDimension() <= 1)
    {
        if (atts.GetPointType() != PseudocolorAttributes::Point)
            return true;
        else if (atts.GetPointSizePixels() != 1)
            return true;
    }

    return false;
}


// ****************************************************************************
//  Method: avtPseudocolorPlot::SetAtts
//
//  Purpose:
//      Sets the atts for the pseudocolor plot.
//
//  Arguments:
//      atts    The attributes for this pseudocolor plot.
//
//  Programmer: Hank Childs
//  Creation:   December 29, 2000
//
//  Modifications:
//    Jeremy Meredith, Fri Mar  2 13:10:02 PST 2001
//    Made this method take a generic AttributeGroup since it is now virtual.
//
//    Kathleen Bonnell, Tue Apr  3 08:56:47 PDT 2001 
//    Made atts a member. Added call to SetScaling method.
//
//    Jeremy Meredith, Tue Jun  5 20:33:13 PDT 2001
//    Added code to set a flag if the plot needs recalculation.  Also
//    store the attributes as a class member.
//
//    Brad Whitlock, Thu Jun 14 16:58:48 PST 2001
//    I added code to update the color tables if necessary.
//
//    Jeremy Meredith, Thu Aug 23 14:11:40 PDT 2001
//    Made it use the color table name "Default" instead of the boolean flag.
//
//    Kathleen Bonnell, Thu Oct  4 16:28:16 PDT 2001 
//    Moved Min/Max code to new method SetLimitsMode. 
//
//    Kathleen Bonnell, Wed Mar 13 12:04:53 PST 2002 
//    varLegends requires SetScaling come before SetLimitsMode. 
//
//    Kathleen Bonnell, Thu Mar 28 14:03:19 PST 2002 
//    Removed previousMode.
// 
//    Kathleen Bonnell, Thu Aug 19 15:29:46 PDT 2004
//    Replaced varMapper with glyphMapper. Added glyphMapper specific code.
//
//    Kathleen Bonnell, Fri Nov 12 11:25:23 PST 2004 
//    Replaced glyphMapper methods with new names. 
//
//    Brad Whitlock, Wed Jul 20 13:26:13 PST 2005
//    I made the pointSize in the atts be used for to set the point size for
//    points, which is not the same as what's used for Box, Axis, Icosahedra.
//
//    Jeremy Meredith, Wed Nov 26 11:28:24 EST 2008
//    Added line style/width control.
//
//    Jeremy Meredith, Fri Feb 20 15:09:22 EST 2009
//    Added support for using per-color alpha values from a color table
//    (instead of just a single global opacity for the whole plot).
//    Changing color can now change opacity, and we must check the
//    color table opacity before we call SetOpacity (now SetOpacityFromAtts)
//    to set the rendering order.  Renamed SetOpacity because it was
//    too complex to take just a single argument.
//
//    Kathleen Bonnell, Mon Jan 17 18:00:41 MST 2011
//    Consider InvertColorTable toggle when setting updateColors.
//
//    Brad Whitlock, Mon Jan  7 17:00:39 PST 2013
//    I added some new glyph types.
//
// ****************************************************************************

void
avtPseudocolorPlot::SetAtts(const AttributeGroup *a)
{
    const PseudocolorAttributes *newAtts = (const PseudocolorAttributes *)a;

    // See if the colors will need to be updated.
    bool updateColors = (!colorsInitialized) ||
      (atts.GetColorTableName() != newAtts->GetColorTableName()) ||
      (atts.GetInvertColorTable() != newAtts->GetInvertColorTable()) ||
      (atts.GetOpacityType() != newAtts->GetOpacityType()) ||
      (atts.GetOpacityType() == PseudocolorAttributes::Ramp &&
       atts.GetOpacity() != newAtts->GetOpacity());

    // See if any attributes that require the plot to be regenerated were
    // changed and copy the state object.
    needsRecalculation = atts.ChangesRequireRecalculation(*newAtts);
    atts = *newAtts;

    // Update the plot's colors if needed.
    if(updateColors || atts.GetColorTableName() == "Default")
    {
        colorsInitialized = true;
        SetColorTable(atts.GetColorTableName().c_str());
    }
    else
      SetOpacityFromAtts();

    SetLighting(atts.GetLightingFlag());
    SetLegend(atts.GetLegendFlag());


    glyphMapper->SetLineWidth(Int2LineWidth(atts.GetLineWidth()));
    glyphMapper->SetLineStyle(Int2LineStyle(atts.GetLineStyle()));
    glyphMapper->SetScale(atts.GetPointSize());
    if (atts.GetPointSizeVarEnabled() &&
        atts.GetPointSizeVar() != "" &&
        atts.GetPointSizeVar() != "\0")
    {
        if (atts.GetPointSizeVar() == "default")
        { 
            if (varname != NULL)
                glyphMapper->ScaleByVar(varname);
        } 
        else
        { 
            glyphMapper->ScaleByVar(atts.GetPointSizeVar());
        } 
    }
    else 
    {
        glyphMapper->DataScalingOff();
    }
    if (atts.GetPointType() == PseudocolorAttributes::Box)
        glyphMapper->SetGlyphType(avtPointGlypher::Box);
    else if (atts.GetPointType() == PseudocolorAttributes::Axis)
        glyphMapper->SetGlyphType(avtPointGlypher::Axis);
    else if (atts.GetPointType() == PseudocolorAttributes::Icosahedron)
        glyphMapper->SetGlyphType(avtPointGlypher::Icosahedron);
    else if (atts.GetPointType() == PseudocolorAttributes::Octahedron)
        glyphMapper->SetGlyphType(avtPointGlypher::Octahedron);
    else if (atts.GetPointType() == PseudocolorAttributes::Tetrahedron)
        glyphMapper->SetGlyphType(avtPointGlypher::Tetrahedron);
    else if (atts.GetPointType() == PseudocolorAttributes::SphereGeometry)
        glyphMapper->SetGlyphType(avtPointGlypher::SphereGeometry);
    else if (atts.GetPointType() == PseudocolorAttributes::Point)
        glyphMapper->SetGlyphType(avtPointGlypher::Point);
    else if (atts.GetPointType() == PseudocolorAttributes::Sphere)
        glyphMapper->SetGlyphType(avtPointGlypher::Sphere);

    SetPointGlyphSize();

    if (varname != NULL)
    {
        glyphMapper->ColorByScalarOn(std::string(varname));
    }

    SetScaling(atts.GetScaling(), atts.GetSkewFactor());
    SetLimitsMode(atts.GetLimitsMode());
}


// ****************************************************************************
//  Method: avtPseudocolorPlot::GetDataExtents
//
//  Purpose:
//      Gets the data extents used by the plot.
//
//  Arguments:
//      extents The extents used by the plot.
//
//  Programmer: Eric Brugger
//  Creation:   March 25, 2004
//
// ****************************************************************************

void
avtPseudocolorPlot::GetDataExtents(std::vector<double> &extents)
{
    double min, max;

    varLegend->GetRange(min, max);

    extents.push_back(min);
    extents.push_back(max);
}


// ****************************************************************************
// Method: avtPseudocolorPlot::SetColorTable
//
// Purpose: 
//   Sets the plot's color table if the color table is the same as that of
//   the plot or we are using the default color table for the plot.
//
// Arguments:
//   ctName : The name of the color table to use.
//
// Returns:    Returns true if the color table is updated.
//
// Programmer: Brad Whitlock
// Creation:   Thu Jun 14 16:52:49 PST 2001
//
// Modifications:
//
//    Jeremy Meredith, Thu Aug 23 14:11:40 PDT 2001
//    Made it use the color table name "Default" instead of the boolean flag.
//
//    Kathleen Bonnell, Thu Aug 30 16:10:15 PDT 2001 
//    Main functionality now resides in avtLookupTable::SetColorTable. 
//
//    Kathleen Bonnell, Fri Aug 31 15:21:45 PDT 2001 
//    Added logic (that existed here before I moved functionality to
//    avtLookupTable) to ensure that changes to active color in color table
//    window will have effect if this plot's color table is "Default". 
//
//    Jeremy Meredith, Fri Feb 20 15:09:22 EST 2009
//    Added support for using per-color alpha values from a color table
//    (instead of just a single global opacity for the whole plot).
//    We have to be careful to update the mapper opacity, render order,
//    and return value here.
//
//    Kathleen Bonnell, Mon Jan 17 18:00:41 MST 2011
//    Retrieve invertColorTableToggle and send to avtLUT.
//
// ****************************************************************************

bool
avtPseudocolorPlot::SetColorTable(const char *ctName)
{
    bool oldColorTableIsFullyOpaque = colorTableIsFullyOpaque;

    colorTableIsFullyOpaque =
        avtColorTables::Instance()->ColorTableIsFullyOpaque(ctName);
    
    bool namesMatch = (atts.GetColorTableName() == std::string(ctName));
    bool useOpacities = SetOpacityFromAtts();

    double rampOpacity;

    if( atts.GetOpacityType() == PseudocolorAttributes::Ramp )
    {
      useOpacities = false;
      rampOpacity = atts.GetOpacity();
    }
    else
      rampOpacity = -1;

    bool retval = (namesMatch &&
                   (oldColorTableIsFullyOpaque != colorTableIsFullyOpaque));

    if (atts.GetColorTableName() == "Default")
        retval |= avtLUT->SetColorTable(NULL, namesMatch,
                                        useOpacities, 
                                        atts.GetInvertColorTable(),
                                        rampOpacity );
    else
        retval |= avtLUT->SetColorTable(ctName, namesMatch,
                                        useOpacities, 
                                        atts.GetInvertColorTable(),
                                        rampOpacity ); 
    return retval;
}


// ****************************************************************************
//  Method: avtPseudocolorPlot::SetLegend
//
//  Purpose:
//      Turns the legend on or off.
//
//  Arguments:
//      legendOn     true if the legend should be turned on, false otherwise.
//
//  Programmer: Hank Childs
//  Creation:   December 28, 2000
//
// ****************************************************************************

void
avtPseudocolorPlot::SetLegend(bool legendOn)
{
    if (legendOn)
    {
        varLegend->LegendOn();
    }
    else
    {
        varLegend->LegendOff();
    }
}

// ****************************************************************************
//  Method: avtPseudocolorPlot::SetScaling
//
//  Purpose:
//    Set up mapper and legend to perform needed scaling 
//
//  Arguments:
//    mode   Which scaling mode to use: 0 = Linear, 1 = Log, 2 = Skew
//    skew   The skew factor to use in the case of skew mode.
//
//  Programmer: Kathleen Bonnell 
//  Creation:   March 30, 2001 
//
//  Modifications:
//
//    Kathleen Bonnell, Thu Aug 30 14:35:32 PDT 2001
//    Set mapper's lut via one retrieved from avtLUT.
//
//    Kathleen Bonnell, Wed Mar 13 12:04:53 PST 2002    
//    Set legend's lut and scaling. 
//
//    Kathleen Bonnell, Thu Aug 19 15:29:46 PDT 2004
//    Replaced varMapper with glyphMapper.
//
// ****************************************************************************

void
avtPseudocolorPlot::SetScaling(int mode, double skew)
{
    varLegend->SetLookupTable(avtLUT->GetLookupTable());
    varLegend->SetScaling(mode, skew);

    if (mode == 1)
    {
       glyphMapper->SetLookupTable(avtLUT->GetLogLookupTable());
    }
    else if (mode == 2)
    {
       avtLUT->SetSkewFactor(skew);
       glyphMapper->SetLookupTable(avtLUT->GetSkewLookupTable());
    }
    else 
    {
       glyphMapper->SetLookupTable(avtLUT->GetLookupTable());
    }
}



// ****************************************************************************
//  Method: avtPseudocolorPlot::SetLighting
//
//  Purpose:
//      Turns the lighting on or off.
//
//  Arguments:
//      lightingOn   true if the lighting should be turned on, false otherwise.
//
//  Programmer: Hank Childs
//  Creation:   December 28, 2000
//
//  Modifications:
//    Kathleen Bonnell, Thu Aug 19 15:29:46 PDT 2004
//    Replaced varMapper with glyphMapper.
//
//    Hank Childs, Fri Oct 29 10:00:15 PDT 2004
//    Turn off specular lighting as well.
//
// ****************************************************************************

void
avtPseudocolorPlot::SetLighting(bool lightingOn)
{
    if (lightingOn)
    {
        glyphMapper->TurnLightingOn();
        glyphMapper->SetSpecularIsInappropriate(false);
    }
    else
    {
        glyphMapper->TurnLightingOff();
        glyphMapper->SetSpecularIsInappropriate(true);
    }
}


// ****************************************************************************
//  Method: avtPseudocolorPlot::SetLimitsMode
//
//  Purpose:  To determine the proper limits the mapper should be using.
//
//  Arguments:
//    limitsMode  Specifies which type of limits.
//
//  Programmer:   Kathleen Bonnell
//  Creation:     October 4, 2001
//
//  Modifications:
//    Brad Whitlock, Wed Nov 7 16:36:37 PST 2001
//    Made it use the generated name for the user-specified limits mode.
//
//    Kathleen Bonnell, Tue Dec 18 08:58:09 PST 2001
//    Added code to set the legend range from the mapper's range. 
//
//    Kathleen Bonnell, Wed Mar 13 12:04:53 PST 2002    
//    Use new method to set legend range. 
//
//    Kathleen Bonnell, Thu Mar 28 08:21:21 PST 2002 
//    Handle cases where min > max. 
//
//    Kathleen Bonnell, Wed May 29 13:40:22 PDT 2002  
//    Always allow user to specify Min/Max. 
// 
//    Eric Brugger, Thu Mar 25 17:12:04 PST 2004
//    I added code to use the data extents from the base class if set.
//
//    Kathleen Bonnell, Thu Aug 19 15:29:46 PDT 2004
//    Replaced varMapper with glyphMapper.
//
// ****************************************************************************

void
avtPseudocolorPlot::SetLimitsMode(int limitsMode)
{
    double min, max;
    //
    //  Retrieve the actual range of the data
    //
    glyphMapper->GetVarRange(min, max);

    double userMin = atts.GetMinFlag() ? atts.GetMin() : min;
    double userMax = atts.GetMaxFlag() ? atts.GetMax() : max;
      
    if (dataExtents.size() == 2)
    {
        glyphMapper->SetMin(dataExtents[0]);
        glyphMapper->SetMax(dataExtents[1]);
    }
    else if (atts.GetMinFlag() && atts.GetMaxFlag())
    {
        if (userMin >= userMax)
        {
            EXCEPTION1(InvalidLimitsException, false); 
        }
        else
        {
            glyphMapper->SetMin(userMin);
            glyphMapper->SetMax(userMax);
        }
    } 
    else if (atts.GetMinFlag())
    {
        glyphMapper->SetMin(userMin);
        if (userMin > userMax)
        {
            glyphMapper->SetMax(userMin);
        }
        else
        {
            glyphMapper->SetMaxOff();
        }
    }
    else if (atts.GetMaxFlag())
    {
        glyphMapper->SetMax(userMax);
        if (userMin > userMax)
        {
            glyphMapper->SetMin(userMax);
        }
        else
        {
            glyphMapper->SetMinOff();
        }
    }
    else
    {
        glyphMapper->SetMinOff();
        glyphMapper->SetMaxOff();
    }
    glyphMapper->SetLimitsMode(limitsMode);

    SetLegendRanges();
}


// ****************************************************************************
//  Method: avtPseudocolorPlot::SetOpacity
//
//  Purpose:
//      Allows for plots to be non-opaque.
//
//  Arguments:
//      opacity     The new opacity.
//
//  Programmer: Hank Childs
//  Creation:   December 28, 2000
//
//  Modifications:
//    Kathleen Bonnell, Mon Sep 29 13:07:50 PDT 2003 
//    Set AntialiasedRenderOrder. 
//
//    Kathleen Bonnell, Thu Aug 19 15:29:46 PDT 2004
//    Replaced varMapper with glyphMapper.
//
//    Jeremy Meredith, Fri Feb 20 15:12:26 EST 2009
//    Added support for using per-color alpha values from a color table
//    (instead of just a single global opacity for the whole plot).
//    This function now needs to set a real opacity less than 1.0
//    if the we have non-1.0 alphas set from the color table (since
//    the value in the mapper gets propagated to the actor prop which is
//    then used to move the geometry into the sorted transparency actor).
//
// ****************************************************************************

bool
avtPseudocolorPlot::SetOpacityFromAtts()
{
    double realOpacity;

    if( atts.GetOpacityType() == PseudocolorAttributes::FullyOpaque ) 
      realOpacity = 1.0;
    else if (atts.GetOpacityType() == PseudocolorAttributes::ColorTable )
    {
      if( colorTableIsFullyOpaque )
        realOpacity = 1.0;
      else
        realOpacity = 0.99;
    }
    else if( atts.GetOpacityType() == PseudocolorAttributes::Constant )
      realOpacity = atts.GetOpacity();
    else if( atts.GetOpacityType() == PseudocolorAttributes::Ramp )
      realOpacity = 0.99;
    else if( atts.GetOpacityType() == PseudocolorAttributes::VariableRange )
      realOpacity = 0.99;

    // double origOpacity = atts.GetOpacity();
    // double realOpacity = atts.GetOpacityType() ?
    //     (colorTableIsFullyOpaque ? 1.0 : 0.99) : origOpacity;

    glyphMapper->SetOpacity(realOpacity);

    if (realOpacity < 1.0)
    {
       behavior->SetRenderOrder(MUST_GO_LAST);
       behavior->SetAntialiasedRenderOrder(MUST_GO_LAST);

       if (atts.GetOpacityType() == PseudocolorAttributes::ColorTable )
         return true;
       else
         return false;
    }
    else
    {
       behavior->SetRenderOrder(DOES_NOT_MATTER);
       behavior->SetAntialiasedRenderOrder(DOES_NOT_MATTER);

       return false;
    }
}


// ****************************************************************************
//  Method: avtPseudocolorPlot::SetLegendRanges
//
//  Purpose:
//    Sets the legend ranges (color bar labels and limits text). 
//
//  Programmer: Kathleen Bonnell 
//  Creation:   March 13, 2002 
//
//  Modifications
//    Kathleen Bonnell, Thu Mar 28 14:03:19 PST 2002 
//    Removed call to SetScaling before throwing exception.
// 
//    Kathleen Bonnell, Wed May 29 13:40:22 PDT 2002  
//    Always allow user to specify Min/Max. 
// 
//    Kathleen Bonnell, Thu Aug 19 15:29:46 PDT 2004
//    Replaced varMapper with glyphMapper.
//
// ****************************************************************************

void
avtPseudocolorPlot::SetLegendRanges()
{
    double min, max;

    //
    // set and get the range for the legend's color bar labels 
    //
    bool validRange = false;
    if (atts.GetLimitsMode() == PseudocolorAttributes::OriginalData)
    {
        validRange = glyphMapper->GetRange(min, max);
    }
    else
    {
        validRange = glyphMapper->GetCurrentRange(min, max);
    }

    varLegend->SetRange(min, max);

    // 
    // Perform error checking if log scaling is to be used.
    // 
    if (atts.GetScaling() == PseudocolorAttributes::Log &&
       ( min <= 0. || max <= 0. ) && validRange == true)
    {
        EXCEPTION1(InvalidLimitsException, true); 
    }
 
    varLegend->SetScaling(atts.GetScaling(), atts.GetSkewFactor());

    //
    // set and get the range for the legend's limits text
    //
    glyphMapper->GetVarRange(min, max);
    varLegend->SetVarRange(min, max);
}

// ****************************************************************************
// Method: avtPseudocolorPlot::SetPointGlyphSize
//
// Purpose: 
//   Sets the point glyph size into the mapper.
//
// Programmer: Brad Whitlock
// Creation:   Thu Jul 21 15:24:25 PST 2005
//
// Modifications:
//   Brad Whitlock, Thu Aug 25 10:15:01 PDT 2005
//   Added sphere points.
//
// ****************************************************************************

void
avtPseudocolorPlot::SetPointGlyphSize()
{
    // Size used for points when using a point glyph.
    if(atts.GetPointType() == PseudocolorAttributes::Point ||
       atts.GetPointType() == PseudocolorAttributes::Sphere)
        glyphMapper->SetPointSize(atts.GetPointSizePixels());
}

// ****************************************************************************
//  Method: avtPseudocolorPlot::ReleaseData
//
//  Purpose:
//      Release the problem sized data associated with this plot.
//
//  Programmer: Hank Childs
//  Creation:   September 12, 2002
//
//  Modifications:
//    Kathleen Bonnell, Thu Aug 19 15:29:46 PDT 2004
//    Removed glyphPoints.
//
//    Kathleen Bonnell, Tue Nov  2 11:02:15 PST 2004 
//    Added pcfilter. 
//
// ****************************************************************************
 
void
avtPseudocolorPlot::ReleaseData(void)
{
    avtSurfaceDataPlot::ReleaseData();
    if (staggeringFilter != NULL) {
        staggeringFilter->ReleaseData();
    }
    if (filter != NULL)
    {
        filter->ReleaseData();
    }
    if (pcfilter != NULL)
    {
        pcfilter->ReleaseData();
    }
}


// ****************************************************************************
//  Method:  avtPseudocolorPlot::GetSmoothingLevel
//
//  Purpose:
//    Returns the amount of smoothing needed inside the avtPlot.
//
//  Programmer:  Jeremy Meredith
//  Creation:    December 10, 2002
//
// ****************************************************************************

int
avtPseudocolorPlot::GetSmoothingLevel()
{
    return atts.GetSmoothingLevel();
}


// ****************************************************************************
//  Method: avtPseudocolorPlot::EnhanceSpecification
//
//  Purpose:
//    Add secondary variable to pipeline if needed. 
//
//  Programmer: Kathleen Bonnell 
//  Creation:   August 19, 2004 
//
//  Modifications:
//
//    Hank Childs, Thu Aug 26 22:23:26 PDT 2010
//    Calculate the extents of the scaling variable.
//
// ****************************************************************************

avtContract_p
avtPseudocolorPlot::EnhanceSpecification(avtContract_p spec)
{
    avtContract_p rv = spec;
    if (topoDim == 0)
    {
        std::string pointVar = atts.GetPointSizeVar();
        avtDataRequest_p dataRequest = spec->GetDataRequest();

        //
        // Find out if we REALLY need to add the secondary variable.
        //
        if (atts.GetPointSizeVarEnabled() && 
            pointVar != "default" &&
            pointVar != dataRequest->GetVariable() &&
            !dataRequest->HasSecondaryVariable(pointVar.c_str()))
        {
            rv = new avtContract(spec);
            rv->GetDataRequest()->AddSecondaryVariable(pointVar.c_str());
            rv->SetCalculateVariableExtents(pointVar, true);
        }
    }
    return rv;
}

// ****************************************************************************
//  Method: avtPlot::SetCellCountMultiplierForSRThreshold
//
//  Purpose: 
//    Set this plot's cell count multiplier for SR threshold. 
//    If not a point-var (topoDim == 0), returns 1.0 because no glyphing will
//    take place.  Otherwise uses the maximum polys that could be created
//    based on spatial dimension.
// 
//  Arguments:
//    dob       The current input, used to determine correct topological
//              and spatial dimenions. 
//
//  Programmer: Kathleen Bonnell 
//  Creation:   August 24, 2004 
//
// ****************************************************************************

void
avtPseudocolorPlot::SetCellCountMultiplierForSRThreshold(
                                   const avtDataObject_p dob)
{
    if (dob->GetInfo().GetAttributes().GetTopologicalDimension() > 0)
    {
        // Not a point var, no multiplier.
        cellCountMultiplierForSRThreshold = 1.0; 
    }
    else 
    {
        //
        // Actual polygon count can change on-the-fly based on point type,
        // and does not require re-execution of the engine (where the
        // cell count is important, and queried from.) 
        // Using the current polgyon count doesn't make sense,
        // the default point-type is 'Point' which does not do any
        // glyhing.  If we return 1, then we may never switch into SR
        // mode even when necessary (if user changes type to icosahedron
        // in 3D, which adds 20 polys per point). 
        // Always using the maximum may be overkill, but it seems best
        // under the circumstances.
        //
        if (dob->GetInfo().GetAttributes().GetSpatialDimension() == 3)
            cellCountMultiplierForSRThreshold = 20.0;
        else 
            cellCountMultiplierForSRThreshold = 12.0;
    }
}
