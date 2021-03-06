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

#include <QvisPostableWindowObserver.h>
#include <QPushButton>
#include <QLayout>
#include <QFileDialog>
#include <SingleAttributeConfigManager.h>
#ifdef _WIN32
#include <QTemporaryFile>
#include <InstallationFunctions.h>
#endif

#include <AttributeSubject.h>

// ****************************************************************************
// Method: QvisPostableWindowObserver::QvisPostableWindowObserver
//
// Purpose: 
//   Constructor for the QvisPostableWindowObserver class. It
//   passes construction responsibility to the superclasses.
//
// Arguments:
//   caption : A string containing the name of the window.
//   notepad : The notepad that the window will post to.
//   subj    : The subject that the window is observing.
//
// Programmer: Brad Whitlock
// Creation:   Fri Jul 28 17:10:59 PST 2000
//
// Modifications:
//    Brad Whitlock, Fri Nov 7 16:23:53 PST 2003
//    I made it inherit from QvisPostableWindowSimpleObserver.
//
//    Brad Whitlock, Wed Apr  9 10:48:14 PDT 2008
//    QString for caption and shortName.
//
// ****************************************************************************

QvisPostableWindowObserver::QvisPostableWindowObserver(
    Subject *subj, const QString &caption, const QString &shortName,
    QvisNotepadArea *notepad, int buttonCombo, bool stretch) : 
    QvisPostableWindowSimpleObserver(caption, shortName, notepad, buttonCombo,
        stretch)
{
    subject = subj;
    subject->Attach(this);
}

// ****************************************************************************
// Method: QvisPostableWindowObserver::~QvisPostableWindowObserver
//
// Purpose: 
//   Destructor for the QvisPostableWindowObserver class.
//
// Programmer: Brad Whitlock
// Creation:   Fri Jul 28 17:11:41 PST 2000
//
// Modifications:
//   
// ****************************************************************************

QvisPostableWindowObserver::~QvisPostableWindowObserver()
{
    if(subject != 0)
        subject->Detach(this);
}

// ****************************************************************************
// Method: QvisPostableWindowObserver::SubjectRemoved
//
// Purpose: 
//   This method is called when the subject is deleted so we can set the 
//   selectedSubject pointer to zero so we don't try and detach from the
//   subject in this window's destructor.
//
// Arguments:
//   TheRemovedSubject : A pointer to the subject being removed.
//
// Programmer: Brad Whitlock
// Creation:   Fri Nov 7 16:25:49 PST 2003
//
// Modifications:
//   
// ****************************************************************************

void
QvisPostableWindowObserver::SubjectRemoved(Subject *TheRemovedSubject)
{
    if(subject == TheRemovedSubject)
        subject = 0;
}

// ****************************************************************************
// Method: QvisPostableWindowObserver::apply
//
// Purpose: 
//   This is a Qt slot function that forces the subject that is being
//   observed to notify its observers if it has changed.
//
// Programmer: Brad Whitlock
// Creation:   Fri Jul 28 18:28:49 PST 2000
//
// Modifications:
//   Brad Whitlock, Fri Nov 7 16:53:29 PST 2003
//   I made it use the selectedSubject pointer.
//
// ****************************************************************************

void
QvisPostableWindowObserver::apply()
{
    AttributeSubject *as = (AttributeSubject *)selectedSubject;

    // If the AttributeSubject that we're observing has fields
    // that are selected, make it notify all its observers.
    if(as->NumAttributesSelected() > 0)
    {
        as->Notify();
    }
}

// ****************************************************************************
//  Method:  QvisPostableWindowObserver::loadSubject
//
//  Purpose:
//    Loads the window's subject from an XML file.
//
//  Arguments:
//    none
//
//  Programmer:  Jeremy Meredith
//  Creation:    January  2, 2009
//
// ****************************************************************************

void
QvisPostableWindowObserver::loadSubject()
{
    if (!subject)
        return;

    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open Attribute XML"),
                                                    NULL,
                                                    tr("XML Files (*.xml);;"
                                                       "All files (*)"));
    if (filename.isNull())
        return;
    
    AttributeSubject *as = (AttributeSubject *)subject;
    SingleAttributeConfigManager mgr(as);
    mgr.Import(filename.toStdString());
    as->SelectAll();
    as->Notify();
}

// ****************************************************************************
//  Method:  QvisPostableWindowObserver::saveSubject
//
//  Purpose:
//    Save the window's subject to an XML file.
//
//  Arguments:
//    none
//
//  Programmer:  Jeremy Meredith
//  Creation:    January  2, 2009
//
//  Modifications:
//    Kathleen Bonnell, Fri May 13 13:28:45 PDT 2011
//    Explicitly set the path in call to getSaveFileName, so the writeability
//    of the 'cwd' can be tested (on Windows) before the call (eg don't 
//    present user with a place to save a file if they cannot save there!).
//
// ****************************************************************************

void
QvisPostableWindowObserver::saveSubject()
{
    if (!subject)
        return;

    QString useDir;
    useDir = QDir::current().path();

#ifdef _WIN32
    { // new scope
        // force a temporary file creation in cwd to see if user can
        // write a file there, if not change useDir to users' VisIt Dir.
        QTemporaryFile tf("mytemp");
        if (!tf.open())
        {
            useDir = GetUserVisItDirectory().c_str();
        }
    }
#endif

    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Save Attribute XML"),
                                                    useDir,
                                                    tr("XML Files (*.xml)"));
    if (filename.isNull())
        return;

    if (filename.indexOf('.') == -1)
        filename += ".xml";

    AttributeSubject *as = (AttributeSubject *)subject;
    SingleAttributeConfigManager mgr(as);
    mgr.Export(filename.toStdString());
}
