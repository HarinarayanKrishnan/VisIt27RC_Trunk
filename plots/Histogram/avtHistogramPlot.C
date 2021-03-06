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
* CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  COQNTRACT,  STRICT
* LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
* OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*
*****************************************************************************/

// ************************************************************************* //
//                          avtHistogramPlot.C                               //
// ************************************************************************* //

#include <avtHistogramPlot.h>

#include <vtkCell.h>
#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkProperty.h>
#include <vtkUnsignedCharArray.h>

#include <LineAttributes.h>

#include <avtHistogramFilter.h>
#include <avtUserDefinedMapper.h>

#ifdef ENGINE
#include <avtExpressionFilter.h>
#include <avtRevolvedVolume.h>
#include <avtVMetricArea.h>
#include <avtVMetricVolume.h>
#endif


// ****************************************************************************
//  Method: avtHistogramPlot constructor
//
//  Programmer: childs -- generated by xml2info
//  Creation:   Thu Jun 26 10:33:56 PDT 2003
//
//  Modifications:
//    Cyrus Harrison, Mon Mar 12 11:08:37 PDT 2007
//    Made sure lighting is off
//
// ****************************************************************************

avtHistogramPlot::avtHistogramPlot()
{
    HistogramFilter = NULL;
    amountFilter = NULL;

    renderer = avtSurfaceAndWireframeRenderer::New();
    avtCustomRenderer_p ren;
    CopyTo(ren, renderer);
    mapper = new avtUserDefinedMapper(ren);

    property = vtkProperty::New();
    property->SetAmbient(1.0);
    property->SetDiffuse(0.0);
 
    renderer->ScalarVisibilityOff();
    renderer->ResolveTopologyOff();
 
    renderer->EdgeStripsOff();
    renderer->EdgePolysOff();

    renderer->SurfaceVertsOff();
    renderer->SurfaceLinesOn();
    renderer->SurfaceStripsOff();
    renderer->SurfacePolysOn();
    // make sure we do not use lighting
    renderer->IgnoreLighting(true);

    fgColor[0] = 0.;
    fgColor[1] = 0.;
    fgColor[2] = 0.;
}


// ****************************************************************************
//  Method: avtHistogramPlot destructor
//
//  Programmer: childs -- generated by xml2info
//  Creation:   Thu Jun 26 10:33:56 PDT 2003
//
// ****************************************************************************

avtHistogramPlot::~avtHistogramPlot()
{
    if (mapper != NULL)
    {
        delete mapper;
        mapper = NULL;
    }
    if (HistogramFilter != NULL)
    {
        delete HistogramFilter;
        HistogramFilter = NULL;
    }
    if (amountFilter != NULL)
    {
        delete amountFilter;
        amountFilter = NULL;
    }
    if (property != NULL)
    {
        property->Delete();
        property = NULL;
    }
}


// ****************************************************************************
//  Method:  avtHistogramPlot::Create
//
//  Purpose:
//    Call the constructor.
//
//  Programmer: childs -- generated by xml2info
//  Creation:   Thu Jun 26 10:33:56 PDT 2003
//
// ****************************************************************************

avtPlot*
avtHistogramPlot::Create()
{
    return new avtHistogramPlot;
}


// ****************************************************************************
//  Method: avtHistogramPlot::GetMapper
//
//  Purpose:
//      Gets a mapper for this plot, it is actually a variable mapper.
//
//  Returns:    The variable mapper typed as its base class mapper.
//
//  Programmer: childs -- generated by xml2info
//  Creation:   Thu Jun 26 10:33:56 PDT 2003
//
// ****************************************************************************

avtMapper *
avtHistogramPlot::GetMapper(void)
{
    return mapper;
}


// ****************************************************************************
//  Method: avtHistogramPlot::ApplyOperators
//
//  Purpose:
//      Applies the operators associated with a Histogram plot.  
//      The output from this method is a query-able object.
//
//  Arguments:
//      input   The input data object.
//
//  Returns:    The data object after the Histogram plot has been applied.
//
//  Programmer: childs -- generated by xml2info
//  Creation:   Thu Jun 26 10:33:56 PDT 2003
//
//  Modifications:
//    Cyrus Harrison, Wed Mar  7 14:12:55 PST 2007
//    Changed to use filters from the expressions library and the engine 
//    specific code mechanism
//
//    Kathleen Bonnell, Thu Mar 22 09:19:55 PDT 2007 
//    Added return of input if ENGINE not defined.
//
//    Hank Childs, Wed Dec 12 11:00:20 PST 2007
//    Retire 2D amount ... it is now captured by mesh coord type.
//
//    Hank Childs, Tue Mar  4 16:39:04 PST 2008
//    Do not insert a volume weighting for frequency counts.
//
// ****************************************************************************

avtDataObject_p
avtHistogramPlot::ApplyOperators(avtDataObject_p input)
{
#ifdef ENGINE
    if (HistogramFilter != NULL)
        delete HistogramFilter;
    if (amountFilter != NULL)
        delete amountFilter;

    avtDataAttributes &in_atts = input->GetInfo().GetAttributes();
    avtExpressionFilter *af = NULL;
    if (in_atts.GetTopologicalDimension() == 3)
    {
        af = new avtVMetricVolume;
    }
    else
    {
        if (in_atts.GetSpatialDimension() == 2)
        {
            if (input->GetInfo().GetAttributes().GetMeshCoordType() == AVT_XY)
                af = new avtVMetricArea;
            else
                af = new avtRevolvedVolume;
        }
        else
            af = new  avtVMetricArea;
    }
    
    af->SetOutputVariableName("_amounts");
    af->SetInput(input);
    amountFilter = af;

    HistogramFilter = new avtHistogramFilter;
    HistogramFilter->SetAttributes(atts);
    if (atts.GetHistogramType() != HistogramAttributes::Frequency)
        HistogramFilter->SetInput(af->GetOutput());
    else
        HistogramFilter->SetInput(input);
    return HistogramFilter->GetOutput();
#endif
    return input;
}


// ****************************************************************************
//  Method: avtHistogramPlot::ApplyRenderingTransformation
//
//  Purpose:
//      Applies the rendering transformation associated with a Histogram plot.  
//
//  Arguments:
//      input   The input data object.
//
//  Returns:    The data object after the Histogram plot has been applied.
//
//  Programmer: childs -- generated by xml2info
//  Creation:   Thu Jun 26 10:33:56 PDT 2003
//
// ****************************************************************************

avtDataObject_p
avtHistogramPlot::ApplyRenderingTransformation(avtDataObject_p input)
{
    return input;
}


// ****************************************************************************
//  Method: avtHistogramPlot::CustomizeBehavior
//
//  Purpose:
//      Customizes the behavior as appropriate for a Histogram plot.  This
//      includes behavior like shifting towards or away from the screen.
//
//  Programmer: childs -- generated by xml2info
//  Creation:   Thu Jun 26 10:33:56 PDT 2003
//
//  Modifications:
//    Eric Brugger, Wed Aug 20 10:25:51 PDT 2003
//    I added code to set the window mode to curve.
//
// ****************************************************************************

void
avtHistogramPlot::CustomizeBehavior(void)
{
    behavior->GetInfo().GetAttributes().SetWindowMode(WINMODE_CURVE);

    renderer->SetProperty(property); 
}


// ****************************************************************************
//  Method: avtHistogramPlot::SetAtts
//
//  Purpose:
//      Sets the atts for the Histogram plot.
//
//  Arguments:
//      atts    The attributes for this Histogram plot.
//
//  Programmer: childs -- generated by xml2info
//  Creation:   Thu Jun 26 10:33:56 PDT 2003
//
//  Modifications:
//    Kathleen Bonnell, Thu May  7 17:56:28 PDT 2009
//    Advertise window-mode here, since it never changes and may need
//    to be known before plot executes.
//
// ****************************************************************************

void
avtHistogramPlot::SetAtts(const AttributeGroup *a)
{
    const HistogramAttributes *newAtts = (const HistogramAttributes *)a;

    needsRecalculation = atts.ChangesRequireRecalculation(*newAtts);

    atts = *newAtts;

    if (HistogramFilter != NULL)
        HistogramFilter->SetAttributes(*newAtts);

    double rgba[4];
    atts.GetColor().GetRgba(rgba);
    property->SetColor((float)rgba[0], (float)rgba[1], (float)rgba[2]);
    if (atts.GetOutputType() == HistogramAttributes::Block)
    {
        property->SetEdgeColor(fgColor[0], fgColor[1], fgColor[2]);
        property->EdgeVisibilityOn();
        renderer->EdgeLinesOn();
    }
    else
    {
        property->SetEdgeColor((float)rgba[0], (float)rgba[1], (float)rgba[2]);
        property->EdgeVisibilityOff();
        renderer->EdgeLinesOff();
    }
    property->SetLineWidth(LineWidth2Int(Int2LineWidth(atts.GetLineWidth())));
    property->SetLineStipplePattern(
                 LineStyle2StipplePattern(Int2LineStyle(atts.GetLineStyle())));

    behavior->GetInfo().GetAttributes().SetWindowMode(WINMODE_CURVE);
}


// ****************************************************************************
//  Method: avtHistogramPlot::ReleaseData
//
//  Purpose:
//      Release the problem sized data associated with this plot.
//
//  Programmer: Hank Childs
//  Creation:   June 26, 2002
//
// ****************************************************************************
 
void
avtHistogramPlot::ReleaseData(void)
{
    avtLineDataPlot::ReleaseData();
 
    if (HistogramFilter != NULL)
    {
        HistogramFilter->ReleaseData();
    }
    if (amountFilter != NULL)
    {
        amountFilter->ReleaseData();
    }
}


// ****************************************************************************
//  Method: avtHistogramPlot::SetForegroundColor
//
//  Purpose:
//      Sets the foreground color of the plot.
//
//  Returns:    true if the plot should be redrawn after the color is set.
//
//  Programmer: Hank Childs
//  Creation:   June 26, 2003
//
// ****************************************************************************

bool
avtHistogramPlot::SetForegroundColor(const double *fg)
{
    fgColor[0] = fg[0];
    fgColor[1] = fg[1];
    fgColor[2] = fg[2];

    if (atts.GetOutputType() == HistogramAttributes::Block)
        property->SetEdgeColor(fgColor[0], fgColor[1], fgColor[2]);

    return (atts.GetOutputType() == HistogramAttributes::Block);
}


