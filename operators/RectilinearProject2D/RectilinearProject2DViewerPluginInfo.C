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
//  File: RectilinearProject2DViewerPluginInfo.C
// ************************************************************************* //

#include <RectilinearProject2DPluginInfo.h>
#include <QApplication>
#include <RectilinearProject2DAttributes.h>

// ****************************************************************************
//  Function:  GetViewerInfo
//
//  Purpose:
//    Return a new ViewerPluginInfo for the RectilinearProject2D operator.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************
extern "C" OP_EXPORT ViewerOperatorPluginInfo* RectilinearProject2D_GetViewerInfo()
{
    RectilinearProject2DViewerPluginInfo::InitializeGlobalObjects();
    return new RectilinearProject2DViewerPluginInfo;
}

//
// Storage for static data elements.
//
RectilinearProject2DAttributes *RectilinearProject2DViewerPluginInfo::clientAtts = NULL;
RectilinearProject2DAttributes *RectilinearProject2DViewerPluginInfo::defaultAtts = NULL;

// ****************************************************************************
//  Method:  RectilinearProject2DViewerPluginInfo::InitializeGlobalObjects
//
//  Purpose:
//    Initialize the operator atts.
//
//  Programmer: generated by xml2info
//  Creation:   omitted
//
// ****************************************************************************
void
RectilinearProject2DViewerPluginInfo::InitializeGlobalObjects()
{
    RectilinearProject2DViewerPluginInfo::clientAtts  = new RectilinearProject2DAttributes;
    RectilinearProject2DViewerPluginInfo::defaultAtts = new RectilinearProject2DAttributes;
}

// ****************************************************************************
//  Method: RectilinearProject2DViewerPluginInfo::GetClientAtts
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
RectilinearProject2DViewerPluginInfo::GetClientAtts()
{
    return clientAtts;
}

// ****************************************************************************
//  Method: RectilinearProject2DViewerPluginInfo::GetDefaultAtts
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
RectilinearProject2DViewerPluginInfo::GetDefaultAtts()
{
    return defaultAtts;
}

// ****************************************************************************
//  Method: RectilinearProject2DViewerPluginInfo::SetClientAtts
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
RectilinearProject2DViewerPluginInfo::SetClientAtts(AttributeSubject *atts)
{
    *clientAtts = *(RectilinearProject2DAttributes *)atts;
    clientAtts->Notify();
}

// ****************************************************************************
//  Method: RectilinearProject2DViewerPluginInfo::GetClientAtts
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
RectilinearProject2DViewerPluginInfo::GetClientAtts(AttributeSubject *atts)
{
    *(RectilinearProject2DAttributes *)atts = *clientAtts;
}

// ****************************************************************************
//  Method: RectilinearProject2DViewerPluginInfo::InitializeOperatorAtts
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
RectilinearProject2DViewerPluginInfo::InitializeOperatorAtts(AttributeSubject *atts,
                                              const ViewerPlot *plot,
                                              const bool fromDefault)
{
    if (fromDefault)
        *(RectilinearProject2DAttributes*)atts = *defaultAtts;
    else
        *(RectilinearProject2DAttributes*)atts = *clientAtts;

    UpdateOperatorAtts(atts, plot);
}

// ****************************************************************************
//  Method: RectilinearProject2DViewerPluginInfo::UpdateOperatorAtts
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
RectilinearProject2DViewerPluginInfo::UpdateOperatorAtts(AttributeSubject *atts,
                                              const ViewerPlot *plot)
{
}

// ****************************************************************************
//  Method: RectilinearProject2DViewerPluginInfo::GetOperatorVarDescription
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
RectilinearProject2DViewerPluginInfo::GetOperatorVarDescription(AttributeSubject *atts,
                                              const ViewerPlot *plot)
{
    return std::string("");
}

// ****************************************************************************
//  Method: RectilinearProject2DViewerPluginInfo::GetMenuName
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
RectilinearProject2DViewerPluginInfo::GetMenuName() const
{
    return new QString(qApp->translate("OperatorNames", "Rectilinear Project 2D"));
}

