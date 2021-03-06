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
//  File: AMRStitchCellViewerPluginInfo.C
// ************************************************************************* //

#include <AMRStitchCellPluginInfo.h>
#include <QApplication>
#include <AMRStitchCellAttributes.h>

// ****************************************************************************
//  Function:  GetViewerInfo
//
//  Purpose:
//    Return a new ViewerPluginInfo for the AMRStitchCell operator.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************
extern "C" OP_EXPORT ViewerOperatorPluginInfo* AMRStitchCell_GetViewerInfo()
{
    AMRStitchCellViewerPluginInfo::InitializeGlobalObjects();
    return new AMRStitchCellViewerPluginInfo;
}

//
// Storage for static data elements.
//
AMRStitchCellAttributes *AMRStitchCellViewerPluginInfo::clientAtts = NULL;
AMRStitchCellAttributes *AMRStitchCellViewerPluginInfo::defaultAtts = NULL;

// ****************************************************************************
//  Method:  AMRStitchCellViewerPluginInfo::InitializeGlobalObjects
//
//  Purpose:
//    Initialize the operator atts.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************
void
AMRStitchCellViewerPluginInfo::InitializeGlobalObjects()
{
    AMRStitchCellViewerPluginInfo::clientAtts  = new AMRStitchCellAttributes;
    AMRStitchCellViewerPluginInfo::defaultAtts = new AMRStitchCellAttributes;
}

// ****************************************************************************
//  Method: AMRStitchCellViewerPluginInfo::GetClientAtts
//
//  Purpose:
//    Return a pointer to the viewer client attributes.
//
//  Returns:    A pointer to the viewer client attributes.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************

AttributeSubject *
AMRStitchCellViewerPluginInfo::GetClientAtts()
{
    return clientAtts;
}

// ****************************************************************************
//  Method: AMRStitchCellViewerPluginInfo::GetDefaultAtts
//
//  Purpose:
//    Return a pointer to the viewer default attributes.
//
//  Returns:    A pointer to the viewer default attributes.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************

AttributeSubject *
AMRStitchCellViewerPluginInfo::GetDefaultAtts()
{
    return defaultAtts;
}

// ****************************************************************************
//  Method: AMRStitchCellViewerPluginInfo::SetClientAtts
//
//  Purpose:
//    Set the viewer client attributes.
//
//  Arguments:
//    atts      A pointer to the new client attributes.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************

void
AMRStitchCellViewerPluginInfo::SetClientAtts(AttributeSubject *atts)
{
    *clientAtts = *(AMRStitchCellAttributes *)atts;
    clientAtts->Notify();
}

// ****************************************************************************
//  Method: AMRStitchCellViewerPluginInfo::GetClientAtts
//
//  Purpose:
//    Get the viewer client attributes.
//
//  Arguments:
//    atts      A pointer to return the client default attributes in.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************

void
AMRStitchCellViewerPluginInfo::GetClientAtts(AttributeSubject *atts)
{
    *(AMRStitchCellAttributes *)atts = *clientAtts;
}

// ****************************************************************************
//  Method: AMRStitchCellViewerPluginInfo::InitializeOperatorAtts
//
//  Purpose:
//    Initialize the operator attributes to the default attributes.
//
//  Arguments:
//    atts        The attribute subject to initialize.
//    plot        The viewer plot that owns the operator.
//    fromDefault True to initialize the attributes from the defaults. False
//                to initialize from the current attributes.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************

void
AMRStitchCellViewerPluginInfo::InitializeOperatorAtts(AttributeSubject *atts,
                                              const ViewerPlot *plot,
                                              const bool fromDefault)
{
    if (fromDefault)
        *(AMRStitchCellAttributes*)atts = *defaultAtts;
    else
        *(AMRStitchCellAttributes*)atts = *clientAtts;

    UpdateOperatorAtts(atts, plot);
}

// ****************************************************************************
//  Method: AMRStitchCellViewerPluginInfo::UpdateOperatorAtts
//
//  Purpose:
//    Update the operator attributes when using operator expressions.
//
//  Arguments:
//    atts        The attribute subject to update.
//    plot        The viewer plot that owns the operator.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************

void
AMRStitchCellViewerPluginInfo::UpdateOperatorAtts(AttributeSubject *atts,
                                              const ViewerPlot *plot)
{
}

// ****************************************************************************
//  Method: AMRStitchCellViewerPluginInfo::GetOperatorVarDescription
//
//  Purpose:
//    Return the operator  variable description.
//
//  Arguments:
//    atts        The current attributes.
//    plot        The viewer plot that owns the operator.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************

std::string
AMRStitchCellViewerPluginInfo::GetOperatorVarDescription(AttributeSubject *atts,
                                              const ViewerPlot *plot)
{
    return std::string("");
}

// ****************************************************************************
//  Method: AMRStitchCellViewerPluginInfo::GetMenuName
//
//  Purpose:
//    Return a pointer to the name to use in the viewer menus.
//
//  Returns:    A pointer to the name to use in the viewer menus.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************

QString *
AMRStitchCellViewerPluginInfo::GetMenuName() const
{
    return new QString(qApp->translate("OperatorNames", "AMR Dual Grid and Stitch Cells"));
}

// ****************************************************************************
//  Method: AMRStitchCellViewerPluginInfo::XPMIconData
//
//  Purpose:
//    Return a pointer to the icon data.
//
//  Returns:    A pointer to the icon data.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************

#include <AMRStitchCell.xpm>
const char **
AMRStitchCellViewerPluginInfo::XPMIconData() const
{
    return AMRStitchCell_xpm;
}

