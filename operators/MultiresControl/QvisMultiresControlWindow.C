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
#include <algorithm>
#include <cstdio>
#include <string>

#include <QtGui/QApplication>
#include <QtGui/QLabel>
#include <QtGui/QGridLayout>
#include <QtGui/QSlider>

#include "QvisMultiresControlWindow.h"
#include <DebugStream.h>
#include <FileServerList.h>
#include <MultiresControlAttributes.h>
#include <ViewerProxy.h>

using std::string;

// ****************************************************************************
// Method: QvisMultiresControlWindow::QvisMultiresControlWindow
//
// Purpose:
//   Constructor
//
// Note:       Autogenerated by xml2window.
//
// Programmer: xml2window
// Creation:   omitted
//
// Modifications:
//
// ****************************************************************************

QvisMultiresControlWindow::QvisMultiresControlWindow(const int type,
                         MultiresControlAttributes *subj,
                         const QString &caption,
                         const QString &shortName,
                         QvisNotepadArea *notepad)
    : QvisOperatorWindow(type,subj, caption, shortName, notepad),
      resolution(NULL),
      resolutionLevelLabel(NULL)
{
    atts = subj;
    fileServer->Attach(this);
}


// ****************************************************************************
// Method: QvisMultiresControlWindow::~QvisMultiresControlWindow
//
// Purpose:
//   Destructor
//
// Note:       Autogenerated by xml2window.
//
// Programmer: xml2window
// Creation:   omitted
//
// Modifications:
//
// ****************************************************************************

QvisMultiresControlWindow::~QvisMultiresControlWindow()
{
  if(fileServer) {
    fileServer->Detach(this);
  }
}


// ****************************************************************************
// Method: QvisMultiresControlWindow::CreateWindowContents
//
// Purpose:
//   Creates the widgets for the window.
//
// Note:       Autogenerated by xml2window.
//
// Programmer: xml2window
// Creation:   omitted
//
// Modifications:
//
//   Tom Fogal, Mon Aug 30 12:30:54 MDT 2010
//   Add resolution # to label.
//
// ****************************************************************************

void
QvisMultiresControlWindow::CreateWindowContents()
{
    QGridLayout *mainLayout = new QGridLayout(NULL);
    topLayout->addLayout(mainLayout);

    resolutionLevelLabel = new QLabel(this);
    resolutionLevelLabel->setText(tr("Resolution: %1").arg(0));
    mainLayout->addWidget(resolutionLevelLabel,0,0);

    this->resolution = new QSlider(Qt::Horizontal, this);
    this->resolution->setMaximum(32); // HACK, we don't know yet.
    this->resolution->setValue(0);
    mainLayout->addWidget(this->resolution, 0, 1);

    connect(this->resolution, SIGNAL(valueChanged(int)), this,
            SLOT(resolutionLevelChanged(int)));
}


// ****************************************************************************
// Method: QvisMultiresControlWindow::UpdateWindow
//
// Purpose:
//   Updates the widgets in the window when the subject changes.
//
// Note:       Autogenerated by xml2window.
//
// Programmer: xml2window
// Creation:   omitted
//
// Modifications:
//
//   Tom Fogal, Mon Aug 30 12:30:28 MDT 2010
//   Include resolution # in label.
//
// ****************************************************************************

void
QvisMultiresControlWindow::UpdateWindow(bool doAll)
{
    QString temp;

    if(fileServer)
    {
        const avtDatabaseMetaData *md = fileServer->GetMetaData(
          fileServer->GetOpenFile(),
          GetStateForSource(fileServer->GetOpenFile()), true, true
        );
        if(md && md->GetNumMeshes() != 0)
        {
            const avtMeshMetaData* mmd = md->GetMesh(0);
            debug3 << "Mesh says there are " << mmd->LODs
                   << " levels of detail.\n";
            atts->SetMaxResolution(mmd->LODs);
        }
    }
    else
    {
        debug1 << "No file server, bailing...\n";
    }
    debug1 << atts->GetMaxResolution() << " levels of detail available.\n";

    this->resolution->blockSignals(true);
    this->resolution->setValue(atts->GetResolution());
    this->resolution->setMaximum(std::max(0, atts->GetMaxResolution()));
    this->resolution->blockSignals(false);
    this->resolution->update();
    this->resolutionLevelLabel->setText(tr("Resolution: %1").
                                        arg(atts->GetResolution()));
    // set it back to its default background color.
    QPalette p = QApplication::palette();
    this->resolutionLevelLabel->setPalette(p);
}


// ****************************************************************************
// Method: QvisMultiresControlWindow::GetCurrentValues
//
// Purpose:
//   Gets values from certain widgets and stores them in the subject.
//
// Note:       Autogenerated by xml2window.
//
// Programmer: xml2window
// Creation:   omitted
//
// Modifications:
//
// ****************************************************************************

void
QvisMultiresControlWindow::GetCurrentValues(int which_widget)
{
    bool doAll = (which_widget == -1);
    QString msg, temp;

    // Do resolution
    if(which_widget == MultiresControlAttributes::ID_resolution || doAll)
    {
        atts->SetResolution(this->resolution->value());
    }

#if 0
    // Not yet ported to Qt4; we might get rid of the 'info' label.

    // Do info
    if(which_widget == MultiresControlAttributes::ID_info || doAll)
    {
        temp = info->displayText();
        okay = !temp.isEmpty();
        if(okay)
        {
            atts->SetInfo(temp.latin1());
        }

        if(!okay)
        {
            msg = tr("The value of info was invalid. "
                     "Resetting to the last good value of %1.").
                  arg(atts->GetInfo().c_str());
            Message(msg);
            atts->SetInfo(atts->GetInfo());
        }
    }
#endif
}


//
// Qt Slot functions
//

// Modificatons:
//
//   Tom Fogal, Mon Aug 30 12:52:36 MDT 2010
//   Change resolution label as well.
void
QvisMultiresControlWindow::resolutionLevelChanged(int val)
{
    if(val != atts->GetResolution())
    {
        atts->SetResolution(val);
        resolution->setValue(val);
        resolutionLevelLabel->setText(tr("Resolution: %1").arg(val));
        resolutionLevelLabel->setAutoFillBackground(true);
        // Change the text color to indicate there are unapplied changes.
        QPalette p;
        p.setColor(QPalette::WindowText, QColor(0,228,0));
        resolutionLevelLabel->setPalette(p);

        SetUpdate(false);
        Apply();
    }
}


void
QvisMultiresControlWindow::resolutionProcessText()
{
    GetCurrentValues(MultiresControlAttributes::ID_resolution);

    Apply();
}

void
QvisMultiresControlWindow::infoProcessText()
{
    GetCurrentValues(MultiresControlAttributes::ID_info);
    Apply();
}
