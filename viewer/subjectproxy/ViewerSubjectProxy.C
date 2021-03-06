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

#include <iostream>
#include <ViewerProxy.h>
#include <ViewerSubject.h>
#include <ViewerSubjectProxy.h>
#include <VisItInit.h>
#include <RuntimeSetting.h>


#include <ViewerState.h>
#include <ViewerMethods.h>
#include <PlotPluginManager.h>
#include <OperatorPluginManager.h>
#include <PluginManagerAttributes.h>
#include <PlotPluginInfo.h>
#include <OperatorPluginInfo.h>


#include <ClientInformation.h>
#include <ClientMethod.h>
#include <DebugStream.h>
#include <ImproperUseException.h>
#include <ParentProcess.h>
#include <PlotInfoAttributes.h>
#include <PluginManagerAttributes.h>
#include <RemoteProcess.h>
#include <SILRestrictionAttributes.h>
#include <SocketConnection.h>
#include <ViewerMethods.h>
#include <ViewerRPC.h>
#include <ViewerState.h>
#include <VisItException.h>
#include <Xfer.h>
#include <InitVTK.h>
#include <InitVTKRendering.h>

#ifndef _WIN32
#include <unistd.h> // for usleep
#endif


#undef Always
#include <QtVisWindow.h>
#include <ViewerWindow.h>
#include <ViewerWindowManager.h>
#include <ViewerPopupMenu.h>
#include <QMenu>
#include <ViewerToolbar.h>
#include <QMainWindow>

std::map<int,vtkQtRenderWindow*> ViewerSubjectProxy::viswindows;

bool 
ViewerSubjectProxy::TestConnection::NeedsRead(bool blocking) const
{
    //HACK: does this logic because visitModule calls NeedsRead once
    //before entering loop and expects to get false..
#ifdef _WIN32
    //Sleep(1);
#else
    usleep(1000);
#endif
    static int val = 0;
    if(val < 1)
    {
        val++;
        return false;
    }
    return true;
}

void ViewerSubjectProxy::InitializePlugins(PluginManager::PluginCategory t, const char *pluginDir)
{
    plotplugin->Initialize(t, false, pluginDir);
    operatorplugin->Initialize(t, false, pluginDir);
}

void ViewerSubjectProxy::LoadPlugins()
{
    ViewerState* state = gstate;
    PlotPluginManager* plotPlugins = plotplugin;
    OperatorPluginManager* operatorPlugins = operatorplugin;

    int nPlots = state->GetNumPlotStateObjects();
    int nOperators = state->GetNumOperatorStateObjects();

    if(plotPlugins == 0 || operatorPlugins == 0)
    {
        EXCEPTION1(ImproperUseException, "ViewerProxy::InitializePlugins "
            "must be called before ViewerProxy::LoadPlugins")
    }

    //
    // Go through the plugin atts and en/disable the ones specified
    // by the plugin attributes
    //
    PluginManagerAttributes *pluginManagerAttributes = state->GetPluginManagerAttributes();
    for (size_t i=0; i<pluginManagerAttributes->GetId().size(); i++)
    {
        if (! pluginManagerAttributes->GetEnabled()[i]) // not enabled
        {
            std::string id = pluginManagerAttributes->GetId()[i];
            if (pluginManagerAttributes->GetType()[i] == "plot")
            {
                if (plotPlugins->PluginExists(id))
                    plotPlugins->DisablePlugin(id);
            }
            else if (pluginManagerAttributes->GetType()[i] == "operator")
            {
                if (operatorPlugins->PluginExists(id))
                    operatorPlugins->DisablePlugin(id);
            }
        }
        else // is enabled -- it had better be available
        {
            std::string id = pluginManagerAttributes->GetId()[i];
            if (pluginManagerAttributes->GetType()[i] == "plot")
            {
                if (plotPlugins->PluginExists(id))
                    plotPlugins->EnablePlugin(id);
                else
                {
                    std::string msg(std::string("The ") + id +
                        std::string(" plot plugin enabled by the viewer "
                            "was not available in the client."));
                    EXCEPTION1(VisItException, msg)
                }
            }
            else if (pluginManagerAttributes->GetType()[i] == "operator")
            {
                if (operatorPlugins->PluginExists(id))
                    operatorPlugins->EnablePlugin(id);
                else
                {
                    std::string msg(std::string("The ") + id +
                        std::string(" operator plugin enabled by the viewer "
                            "was not available in the client."));
                    EXCEPTION1(VisItException, msg)
                }
            }
        }
    }

    //
    // Now load dynamic libraries
    //
    plotPlugins->LoadPluginsNow();
    operatorPlugins->LoadPluginsNow();

    //
    // Initialize the plot attribute state objects.
    //
    nPlots = plotPlugins->GetNEnabledPlugins();
    for (int i = 0; i < nPlots; ++i)
    {
        CommonPlotPluginInfo *info =
            plotPlugins->GetCommonPluginInfo(plotPlugins->GetEnabledID(i));
        AttributeSubject *obj = info->AllocAttributes();
        //std::cout << obj->TypeName() << std::endl;
        state->RegisterPlotAttributes(obj);
    }

    //
    // Initialize the operator attribute state objects.
    //
    nOperators = operatorPlugins->GetNEnabledPlugins();
    for (int i = 0; i < nOperators; ++i)
    {
        CommonOperatorPluginInfo *info =
            operatorPlugins->GetCommonPluginInfo(operatorPlugins->GetEnabledID(i));
        AttributeSubject *obj = info->AllocAttributes();
        //std::cout << obj->TypeName() << std::endl;
        state->RegisterOperatorAttributes(obj);
    }
}

//return the new cli plot and operator manager so that the visit cli works like normal..
PlotPluginManager* ViewerSubjectProxy::GetPlotPluginManager() const
{
    return plotplugin;
}

OperatorPluginManager *ViewerSubjectProxy::GetOperatorPluginManager() const
{
    return operatorplugin;
}

ViewerState   *ViewerSubjectProxy::GetViewerState() const
{
   return gstate;
}

ViewerMethods *ViewerSubjectProxy::GetViewerMethods() const
{
    return gmethods;
}

void ViewerSubjectProxy::Initialize(int argc,char* argv[])
{
    vissubject = new ViewerSubject();
    testconn = new TestConnection();
    plotplugin = new PlotPluginManager();
    operatorplugin = new OperatorPluginManager();

    gstate = vissubject->GetViewerState();
    gmethods = vissubject->GetViewerMethods();

    try
    {
        vissubject->GetViewerProperties()->SetUseWindowMetrics(false);
        QtVisWindow::SetWindowCreationCallback(renderWindow, this);
        //let VisIt own all actions as if it was its
        QtVisWindow::SetOwnerShipOfAllWindows(true);
        //std::cout << "initializing" << std::endl;
        vissubject->ProcessCommandLine(argc,argv);
        vissubject->Initialize();
        viewerWindowCreated(ViewerWindowManager::Instance()->GetActiveWindow());
        connect(ViewerWindowManager::Instance(),SIGNAL(createWindow(ViewerWindow*)),
                this,SLOT(viewerWindowCreated(ViewerWindow*)));
    }
    catch(VisItException e)
    {
        std::cout << e.Message() << std::endl;
    }
}


vtkQtRenderWindow* ViewerSubjectProxy::GetRenderWindow(int i)
{
    //VisIt has not initialized any windows..
    if(viswindows.size() == 0)
        return NULL;

    if(viswindows.find(i) != viswindows.end())
        return viswindows[i];

    std::cerr << "Window " << i << " does not exist" << std::endl;
    return NULL;
}

vtkQtRenderWindow* ViewerSubjectProxy::renderWindow(void* data)
{
    vtkQtRenderWindow* win = new ViewerSubjectProxy::NonClosableQtRenderWindow();

    //add signal to let us know when windowDeleted.
    win->connect(win,SIGNAL(destroyed(QObject*)),(QObject*)data,SLOT(windowDeleted(QObject*)));

    //find next free id (okay to do this loop since VisIt
    //has very limited opengl windows it can support

    int id = 1;
    while(viswindows.count(id) > 0) id++;

    viswindows[id] = win;

    return viswindows[id];
}

void ViewerSubjectProxy::windowDeleted(QObject * obj)
{
    std::map<int,vtkQtRenderWindow*>::iterator itr = viswindows.begin();

    for(itr = viswindows.begin(); itr != viswindows.end(); ++itr)
    {
        if(itr->second == obj)
        {
            std::cerr << "Deleting window: " << itr->first
                    << ". References to Rendering Window are now invalid. "
                    << std::endl;
            viswindows.erase(itr);
        }
    }
}

QList<int> ViewerSubjectProxy::GetRenderWindowIDs()
{
    QList<int> windowIDs;

    std::map<int,vtkQtRenderWindow*>::iterator itr = viswindows.begin();

    for(itr = viswindows.begin(); itr != viswindows.end(); ++itr)
    {
        windowIDs.append(itr->first);
    }

    return windowIDs;
}

// Constructor
ViewerSubjectProxy::ViewerSubjectProxy(int argc, char *argv[]):ViewerProxy()
{
    //reset VTK logs..
    InitVTK::Initialize();
    InitVTKRendering::Initialize();

    initialize = true;
    //setting to ViewerBase works because they are static
    gstate = ViewerBase::GetViewerState();
    gmethods = ViewerBase::GetViewerMethods();
    vissubject = 0;
    plotplugin = 0;
    operatorplugin = 0;
    m_proxy = 0;
    internalSILRestriction = new avtSILRestriction(
        *gstate->GetSILRestrictionAttributes());
    gstate->GetSILRestrictionAttributes()->Attach(this);

    Initialize(argc,argv);
}

// This constructor emulates as if it has already been started..
ViewerSubjectProxy::ViewerSubjectProxy(ViewerSubjectProxy* proxy):ViewerProxy()
{
    initialize = false;
    vissubject = 0;
    this->gstate = proxy->GetViewerState();
    this->gmethods = proxy->GetViewerMethods();

    testconn = new TestConnection();
    plotplugin = new PlotPluginManager();
    operatorplugin = new OperatorPluginManager();
    m_proxy = proxy;
    internalSILRestriction = new avtSILRestriction(
        *gstate->GetSILRestrictionAttributes());
    gstate->GetSILRestrictionAttributes()->Attach(this);
}

// Destructor (free allocated memory)
ViewerSubjectProxy::~ViewerSubjectProxy()
{
    delete testconn;
    delete plotplugin;
    delete operatorplugin;
}


//SIL functions
avtSILRestriction_p 
ViewerSubjectProxy::GetPlotSILRestriction()
{
    return internalSILRestriction;
}

avtSILRestriction_p 
ViewerSubjectProxy::GetPlotSILRestriction() const
{
    return new avtSILRestriction(internalSILRestriction);
}

void 
ViewerSubjectProxy::SetPlotSILRestriction(avtSILRestriction_p newRestriction)
{
    // Copy the new SIL restriction into the internal SIL restriction object.
    internalSILRestriction = newRestriction;

    SILRestrictionAttributes *newSRA =internalSILRestriction->MakeAttributes();
    if(newSRA)
    {
        // Copy the attribute representation of the SIL restriction into the
        // silRestrictionAtts object and send notify observers. Note that
        // SetUpdate is set to false so the proxy's Update method is NOT called
        SILRestrictionAttributes *silRestrictionAtts =
            gstate->GetSILRestrictionAttributes();
        *silRestrictionAtts = *newSRA;
        SetUpdate(false);
        silRestrictionAtts->Notify();

        // Now that the new SIL restriction attributes have been sent to the
        // viewer, send the RPC that tells the viewer to apply them.
        gmethods->SetPlotSILRestriction();

        // Delete the new SRA since we're done with it.
        delete newSRA;
    }
}

void 
ViewerSubjectProxy::SetPlotSILRestriction()
{
    SILRestrictionAttributes *newSRA =internalSILRestriction->MakeAttributes();
    if(newSRA)
    {
        // Copy the attribute representation of the SIL restriction into the
        // silRestrictionAtts object and send notify observers. Note that
        // SetUpdate is set to false so the proxy's Update method is NOT called
        SILRestrictionAttributes *silRestrictionAtts =
            gstate->GetSILRestrictionAttributes();
        *silRestrictionAtts = *newSRA;
        SetUpdate(false);
        silRestrictionAtts->Notify();

        // Now that the new SIL restriction attributes have been sent to the
        // viewer, send the RPC that tells the viewer to apply them.
        gmethods->SetPlotSILRestriction();

        // Delete the new SRA since we're done with it.
        delete newSRA;
    }
}

void
ViewerSubjectProxy::Update(Subject *subj)
{
    // If the SIL restriction attributes have changed, update the internal
    // SIL restriction object internalSILRestriction. This is done so the
    // internalSILRestriction is available to the client and we have to
    // make the conversion far less often.
    if(subj == gstate->GetSILRestrictionAttributes())
    {
        internalSILRestriction = new avtSILRestriction(
            *gstate->GetSILRestrictionAttributes());
        if(m_proxy) m_proxy->Update(subj);
    }
}

void
ViewerSubjectProxy::viewerWindowCreated(ViewerWindow *window)
{
    QMenu* menu = window->GetPopupMenu()->GetPopup();
    QList<QAction*> actions = menu->actions();
    for(int i = 0; i < actions.size(); ++i)
    {
        QAction* action = actions[i];

        if(     action->text() == tr("Quit") ||
                action->text() == tr("Open CLI") ||
                action->text() == tr("Open GUI") ||
                action->text() == tr("Customize"))
            menu->removeAction(action);
        else if(action->text() == tr("Window"))
        {
            QList<QAction*> winactions = action->menu()->actions();
            for(int i = 0; i < winactions.size(); ++i)
            {
                if(winactions[i]->text() == tr("New") ||
                        winactions[i]->text() == tr("Clone") ||
                        winactions[i]->text() == tr("Delete") ||
                        winactions[i]->text() == tr("Layout"))
                    action->menu()->removeAction(winactions[i]);
            }
        }
    }

    ViewerToolbar* toolbar = window->GetToolbar();
    toolbar->HideAll();

    int visid = window->GetWindowId()+1; //+1 Gets vtkQtRenderWindow id..
    QMainWindow* renwin = dynamic_cast<QMainWindow*>(viswindows[visid]);
    renwin->setProperty("id",visid);
    renwin->installEventFilter(this);
}

bool
ViewerSubjectProxy::eventFilter(QObject *o, QEvent *e)
{
    if(e->type() == QEvent::Show && !e->spontaneous())
    {
        QMainWindow* renwin = dynamic_cast<QMainWindow*>(o);
        if(renwin)
        {
            int id = renwin->property("id").toInt();

            ViewerWindow* vw = ViewerWindowManager::Instance()->GetWindow(id-1);
            if(vw)
            {
                if (!vw->GetRealized())
                    vw->Realize();
                //else
                //    vw->Show();
            }
            return true;
        }
    }
/*    else if(e->type() == QEvent::Hide && !e->spontaneous())
    {
        QMainWindow* renwin = dynamic_cast<QMainWindow*>(o);
        if(renwin)
        {
            int id = renwin->property("id").toInt();
            ViewerWindow* vw = ViewerWindowManager::Instance()->GetWindow(id-1);
            if(vw)
                vw->Hide();
            return true;
        }
    }
*/
    return false;
}

