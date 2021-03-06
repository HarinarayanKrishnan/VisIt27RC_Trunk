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
//                             ZoomInteractor.C                              //
// ************************************************************************* //

#include <vtkActor2D.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkProperty2D.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

#include <VisWindow.h>
#include <VisWindowInteractorProxy.h>
#include <ZoomInteractor.h>


// ****************************************************************************
//  Method:  ZoomInteractor constructor
//
//  Programmer: Hank Childs
//  Creation:   May 22, 2000
//
//  Modifications:
//
//    Hank Childs, Wed Jul 26 11:42:39 PDT 2000
//    Removed call to SetCanvasViewport, since that is now done every time
//    rubber band mode is started so re-sizes can be picked up.
//
//    Kathleen Bonnell, Thu Nov 15 12:24:45 PST 2001 
//    Change ids from int to vtkIdType to match VTK 4.0 API. 
//
//    Akira Haddox, Thu Jul  3 14:15:48 PDT 2003
//    Added initialization of shift and controlKeyDown.
//    
//    Kathleen Bonnell, Wed Aug 18 10:10:35 PDT 2004 
//    Set shouldClampSquare and shouldDrawGuides based on InteractorAtts.
//
//    Brad Whitlock, Fri Mar 10 17:43:07 PST 2006
//    Added APPLE-specific code where we create polydata that can contain
//    4 points instead of 2. This lets us draw the lines at the same time
//    into an overlay window.
//
//    Brad Whitlock, Fri Oct 14 16:27:15 PDT 2011
//    Create mapper via proxy.
//
//    Kathleen Biagas, Mon Jun 11 17:15:39 MST 2012 
//    Have windows follow the APPLE path. 
//
// ****************************************************************************

ZoomInteractor::ZoomInteractor(VisWindowInteractorProxy &vw) 
    : VisitInteractor(vw)
{
    rubberBandMode  = false;
    shiftKeyDown = controlKeyDown = false;

    VisWindow *win = vw;
    shouldClampSquare = win->GetInteractorAtts()->GetClampSquare();
    shouldDrawGuides = win->GetInteractorAtts()->GetShowGuidelines();

    //
    // Create the poly data that will map the rubber band onto the screen.
    //
    rubberBand       = vtkPolyData::New();

    vtkPoints *pts = vtkPoints::New();
#if defined(__APPLE__) || defined(_WIN32)
    pts->SetNumberOfPoints(4);
    rubberBand->SetPoints(pts);
    pts->Delete();

    vtkCellArray *lines  = vtkCellArray::New();
    vtkIdType  ids[] = { 0, 1, 2, 3, 0};
    lines->InsertNextCell(5, ids);
    rubberBand->SetLines(lines);
    lines->Delete();
#else
    pts->SetNumberOfPoints(2);
    rubberBand->SetPoints(pts);
    pts->Delete();

    vtkCellArray *lines  = vtkCellArray::New();
    vtkIdType  ids[2] = { 0, 1 };
    lines->InsertNextCell(2, ids);
    rubberBand->SetLines(lines);
    lines->Delete();
#endif

    rubberBandMapper = proxy.CreateRubberbandMapper();
    rubberBandMapper->SetInputData(rubberBand);

    rubberBandActor  = vtkActor2D::New();
    rubberBandActor->SetMapper(rubberBandMapper);
    rubberBandActor->GetProperty()->SetColor(0., 0., 0.);
}


// ****************************************************************************
//  Method: ZoomInteractor destructor
//
//  Programmer: Hank Childs
//  Creation:   May 24, 2000
//
// ****************************************************************************

ZoomInteractor::~ZoomInteractor()
{
    rubberBand->Delete();
    rubberBandMapper->Delete();
    rubberBandActor->Delete();
}


// ****************************************************************************
//  Method: ZoomInteractor::SetCanvasViewport
//
//  Purpose:
//      Sets where the canvas is in display coordinates so that rubber bands
//      outside the canvas can be forced inside the canvas.
//
//  Programmer: Hank Childs
//  Creation:   May 24, 2000
//
// ****************************************************************************

void
ZoomInteractor::SetCanvasViewport(void)
{
    vtkRenderer *ren = proxy.GetCanvas();

    canvasDeviceMinX = 0.;
    canvasDeviceMinY = 0.;
    ren->NormalizedViewportToViewport(canvasDeviceMinX, canvasDeviceMinY);
    ren->ViewportToNormalizedDisplay(canvasDeviceMinX, canvasDeviceMinY);
    ren->NormalizedDisplayToDisplay(canvasDeviceMinX, canvasDeviceMinY);

    canvasDeviceMaxX = 1.;
    canvasDeviceMaxY = 1.;
    ren->NormalizedViewportToViewport(canvasDeviceMaxX, canvasDeviceMaxY);
    ren->ViewportToNormalizedDisplay(canvasDeviceMaxX, canvasDeviceMaxY);
    ren->NormalizedDisplayToDisplay(canvasDeviceMaxX, canvasDeviceMaxY);
}


// ****************************************************************************
//  Method: ZoomInteractor::StartRubberBand
//
//  Purpose:
//      Tells the interactor to draw a rubber band and update it whenever the
//      mouse moves.
//
//  Arguments:
//      x       The x-coordinate of the pointer in display coordinates when
//              the button was pressed.
//      y       The y-coordinate of the pointer in display coordinates when
//              the button was pressed.
//
//  Programmer: Hank Childs
//  Creation:   May 22, 2000
//
//  Modifications:
//
//    Hank Childs, Wed Jul  5 11:32:18 PDT 2000
//    Removed call to StartTimer since pointer motions are now picked up
//    by the interactor and the timer is incompatible (and redundant) with it.
//
//    Hank Childs, Wed Jul 26 11:42:39 PDT 2000
//    Added call to SetCanvasViewport.  Done here to pick up resizes between
//    rubber band modes (as compared to constructor).
//
//    Hank Childs, Tue Aug  1 15:37:11 PDT 2000
//    Moved SetCanvasViewport to be before ForceCoordsToViewport.
//
//    Akira Haddox, Thu Jul  3 14:17:48 PDT 2003
//    Set shift and controlKeyDown based on interactor information.
//
//    Kathleen Bonnell, Wed Aug 18 10:10:35 PDT 2004 
//    Set shouldClampSquare and shouldDrawGuides based on InteractorAtts.
//
//    Brad Whitlock, Mon Mar 13 11:08:22 PDT 2006
//    I added code to set the actor's color to the current foreground
//    color so it uses the right color on MacOS X. Other platforms use the
//    XOR drawing mode so the color will vary there.
//
// ****************************************************************************

void
ZoomInteractor::StartRubberBand(int x, int y)
{
    rubberBandMode = true;

    shiftKeyDown = Interactor->GetShiftKey();
    controlKeyDown = Interactor->GetControlKey();

    VisWindow *win = proxy;
    shouldClampSquare = win->GetInteractorAtts()->GetClampSquare();
    shouldDrawGuides = win->GetInteractorAtts()->GetShowGuidelines();

    // Set the actor's color.
    double fg[3];
    proxy.GetForegroundColor(fg);
    rubberBandActor->GetProperty()->SetColor(fg[0], fg[1], fg[2]);

    //
    // Add the rubber band actors to the background.  We do this since the
    // background is in the same display coordinates that the rubber band will
    // be.  From an appearance standpoint it should be in the canvas, which
    // the routine ForceCoordsToViewport ensures.
    //
    vtkRenderer *ren = proxy.GetBackground();
    ren->AddActor2D(rubberBandActor);

    //
    // The anchor of the rubber band will be where the button press was.
    //
    anchorX = x;
    anchorY = y;

    //
    // Determine what to clamp the rubber band to.
    //
    SetCanvasViewport();

    //
    // If the user has clicked outside the viewport, force the back inside.
    //
    ForceCoordsToViewport(anchorX, anchorY);

    //
    // Must update bookkeeping so that OnMouseMove works correctly.
    //
    lastX   = anchorX;
    lastY   = anchorY;

    rubberBandDrawn = false;
}


// ****************************************************************************
//  Method: ZoomInteractor::OnMouseMove
//
//  Purpose:
//      Receives a mouse movement and updates the rubber band.
//
//  Arguments:
//      ctrl          non-zero if the ctrl button was pushed.
//      shift         non-zero if the shift button was pushed.
//      x             The x-coordinate on the mouse pointer in display coords.
//      y             The y-coordinate on the mouse pointer in display coords.
//     
//  Programmer: Hank Childs
//  Creation:   May 22, 2000
//
//  Modifications:
//    Kathleen Bonnell, Fri Dec 13 16:41:12 PST 2002 
//    Removed arguments to comply with vtks' new interactor interface.
//    Values are now accessed via the RenderWindowInteractor.
//
//    Akira Haddox, Thu Jul  3 14:17:48 PDT 2003
//    Force a square rubber band when shift key is down.
//
//    Kathleen Bonnell, Wed Aug 18 10:10:35 PDT 2004 
//    Force a square rubber band when the global flag is turned on, or
//    the shift key is down. 
//
// ****************************************************************************

void
ZoomInteractor::OnMouseMove()
{
    int x, y;
    if (rubberBandMode)
    {
        Interactor->GetEventPosition(x, y);
        ForceCoordsToViewport(x, y);

        //
        // If the shift key is down, lock the coordinates so that we form
        // a square.
        //
        if (shouldClampSquare || shiftKeyDown)
        {
            int deltaX = x - anchorX;
            int deltaY = y - anchorY;
            
            int absDeltaX = abs(deltaX);
            int absDeltaY = abs(deltaY);
            
            if (absDeltaX > absDeltaY)
            {
                // x stays the same, adjust y
                if (deltaY < 0)
                    y = anchorY - absDeltaX;
                else
                    y = anchorY + absDeltaX;
            }
            else
            {
                // y stays the same, adjust x
                if (deltaX < 0)
                    x = anchorX - absDeltaY;
                else
                    x = anchorX + absDeltaY;
            }

            // We now have a square, but it might be outside our bounds.
            // We force the coordinates again, since we take the larger
            // dimension to expand the square on, and we might have
            // expanded outside the box.
            // If these coordinates change, then we need to make it a box
            // again.
            //
            int oldX = x;
            int oldY = y;
            ForceCoordsToViewport(x, y);
            oldX = abs(x-oldX);
            oldY = abs(y-oldY);

            // if x was forced, adjust y
            if (oldX)
            {
                if (deltaY < 0)
                    y += oldX;
                else
                    y -= oldX;
            }

            // if y was forced, adjust x
            if (oldY)
            {
                if (deltaX < 0)
                    x += oldY;
                else
                    x -= oldY;
            }
        }

        UpdateRubberBand(anchorX, anchorY, lastX, lastY, x, y);
        lastX = x;
        lastY = y;
    }
    else
    {
        //
        // We are not in rubber band mode, so we don't want to override
        // the interactor.  Call the base function.
        //
        VisitInteractor::OnMouseMove();
    }
}


// ****************************************************************************
//  Method: ZoomInteractor::EndRubberBand
//
//  Purpose:
//      Tells the interactor to remove the rubber band and no longer update it 
//      whenever the mouse moves.
//
//  Programmer: Hank Childs
//  Creation:   May 22, 2000
//
//  Modifications:
//
//    Hank Childs, Mon Jun 26 17:09:25 PDT 2000
//    Removed unused arguments for the location of the mouse pointer.
//
//    Hank Childs, Wed Jul  5 11:32:18 PDT 2000
//    Removed call to StartTimer since pointer motions are now picked up
//    by the interactor and the timer is incompatible (and redundant) with it.
//
// ****************************************************************************

void
ZoomInteractor::EndRubberBand()
{
    rubberBandMode = false;

    //
    // Remove the rubber band actors from the renderer.
    // We should use the background as the renderer because our rubber band
    // is in display coordinates.
    //
    vtkRenderer *ren = proxy.GetBackground();
    ren->RemoveActor2D(rubberBandActor);
}


// ****************************************************************************
//  Method: ZoomInteractor::ForceCoordsToViewport
//
//  Purpose:
//      Takes coordinates and makes sure they are in the viewport.  If not,
//      it forces them inside the viewport.
//
//  Arguments:
//      x      X coordinate in display coordinates.
//      y      Y coordinate in display coordinates.
//
//  Note:    This routine assumes the canvasDevice[Min|Max][X|Y] has
//           already been set.
//
//  Programmer: Hank Childs
//  Creation:   May 22, 2000
//
//  Modifications:
//
//    Hank Childs, Tue Sep 18 12:02:29 PDT 2001
//    Cast return value of ceil/floor to get rid of compiler warnings.
//
//    Kathleen Bonnell, Wed Jul  2 09:12:52 PDT 2003  
//    Switch ceil to floor and vice-versa, so that rubber ban can extend
//    all the way to the xmin, xmax, ymin, ymax. 
//
// ****************************************************************************

void
ZoomInteractor::ForceCoordsToViewport(int &x, int &y)
{
    if (x < canvasDeviceMinX)
    {
        x = (int)floor(canvasDeviceMinX);
    }
    if (x > canvasDeviceMaxX)
    {
        x = (int)ceil(canvasDeviceMaxX);
    }
    if (y < canvasDeviceMinY)
    {
        y = (int)floor(canvasDeviceMinY);
    }
    if (y > canvasDeviceMaxY)
    {
        y = (int)ceil(canvasDeviceMaxY);
    }
}


// ****************************************************************************
//  Function: GetSegment
//
//  Purpose:
//      Determines what pixels should be drawn based on the anchor location,
//      the location of the old pixel and the location of the new pixel.
//
//  Note:       This is meant for extending or retracting a segment.  If
//              both dimensions vary for the new pixel, this should not be 
//              used.
//
//  Arguments:
//      a       The location of the anchor in one dimension in display coords.
//      l       The location of the last coord in one dim in display coords.
//      n       The location of the new coord in one dim in display coords.
//      outl    Where the last coord portion of the segment should be placed.
//      newl    Where the new coord portion of the segment should be placed.
//
//  Programmer: Hank Childs
//  Creation:   May 30, 2000
//
// ****************************************************************************

static inline void
GetSegment(int a, int l, int n, int &outl, int &newl)
{
    outl = l;
    newl = n;
    if (abs(l-a) > abs(n-a))
    {
        //
        // We are moving towards the anchor.
        //
        int offset = (l > a ? 1 : -1);
        newl += offset;
    }
    else
    {
        //
        // We are moving away from the anchor.
        //
        int offset = (n > a ? 1 : -1);
        outl += offset;
    }
}


// ****************************************************************************
//  Method: ZoomInteractor::UpdateRubberBand
//
//  Purpose:
//      The real rubber band function.  This takes six arguments specifying
//      the anchor of the rubber band, what the last opposite corner was,
//      and what the new opposite corner is.  It implements the rubber band
//      through an XOR found in any text book.  To prevent blinking effects,
//      if extends the old rubber band lines instead of wiping them out and
//      redrawing.
//
//  Arguments:
//      aX      The x-coordinate of the anchor in display coordinates.
//      aY      The y-coordinate of the anchor in display coordinates.
//      lX      The x-coordinate of the last corner in display coordinates.
//      lY      The y-coordinate of the last corner in display coordinates.
//      nX      The x-coordinate of the new corner in display coordinates.
//      nY      The y-coordinate of the new corner in display coordinates.
//
//  Programmer: Hank Childs
//  Creation:   May 22, 2000
//
//  Modifications:
//    Brad Whitlock, Fri Mar 10 17:45:22 PST 2006
//    I added code for APPLE that lets us draw all line segments at the same
//    time. The mapper that we're using in this case does not XOR so it's ok.
//   
//    Kathleen Biagas, Mon Jun 11 17:15:39 MST 2012 
//    Have windows follow the APPLE path. 
//
// ****************************************************************************

void
ZoomInteractor::UpdateRubberBand(int aX, int aY, int lX, int lY,int nX,int nY)
{
    if (nX == lX && nY == lY)
    {
        //
        // No update necessary.
        //
        return;
    }

#if defined(__APPLE__) || defined(_WIN32)
    // This code assumes that the lines will be drawn at the same time
    // in an overlay window that the rubber-band mapper will create.
    vtkViewport *ren = proxy.GetBackground();
    vtkPoints *pts = rubberBand->GetPoints();
    pts->SetPoint(0, (double) aX, (double) aY, 0.);   
    pts->SetPoint(1, (double) nX, (double) aY, 0.);   
    pts->SetPoint(2, (double) nX, (double) nY, 0.);   
    pts->SetPoint(3, (double) aX, (double) nY, 0.);
    rubberBandMapper->RenderOverlay(ren, rubberBandActor);
#else
    // This code assumes that the line segments will be drawn individually
    // with XOR.

    //
    // Crossing over the anchor gives us a big problem.  Break up this case.
    //
    if ((nX-aX)*(lX-aX) < 0 || (nY-aY)*(lY-aY) < 0)
    {
        UpdateRubberBand(aX, aY, lX, lY, aX, aY);
        UpdateRubberBand(aX, aY, aX, aY, nX, nY);
        return;
    }

    //
    // Handle the line that is vertical and has the new corner as one of its
    // vertices.
    //
    if (nX == lX)
    {
        //
        // The box moved up in the y-direction, so extend what we had there
        // before.
        //
        int  lastY, newY;
        GetSegment(aY, lY, nY, lastY, newY);
        DrawRubberBandLine(lX, lastY, lX, newY);
    }
    else
    {
        //
        // Add the new line and erase the old one.
        //
        DrawRubberBandLine(nX, aY, nX, nY);
        DrawRubberBandLine(lX, aY, lX, lY);
    }

    //
    // Handle the line that is horizontal and has the new corner as one of its
    // vertices.
    //
    if (nY == lY)
    {
        //
        // The box moved over in the x-direction, so extend what we had there
        // before.
        //
        int  lastX, newX;
        GetSegment(aX, lX, nX, lastX, newX);
        DrawRubberBandLine(lastX, lY, newX, lY);
    }
    else
    {
        //
        // Add the new line and erase the old one.
        //
        DrawRubberBandLine(aX, nY, nX, nY);
        DrawRubberBandLine(aX, lY, lX, lY);
    }

    //
    // Handle the line that is vertical and has the anchor as one of its
    // vertices.
    //
    if (nY != lY)
    {
        int lastY, newY;
        GetSegment(aY, lY, nY, lastY, newY);
        DrawRubberBandLine(aX, lastY, aX, newY);
    }

    //
    // Handle the line that is horizontal and has the anchor as one of its
    // vertices.
    //
    if (nX != lX)
    {
        int lastX, newX;
        GetSegment(aX, lX, nX, lastX, newX);
        DrawRubberBandLine(lastX, aY, newX, aY);
    }
#endif
}


// ****************************************************************************
//  Method: ZoomInteractor::DrawRubberBandLine
//
//  Purpose:
//      Draws a rubber band line.
//
//  Arguments:
//      x1      The x-coordinate of the first endpoint in display coordinates.
//      y1      The y-coordinate of the first endpoint in display coordinates.
//      x2      The x-coordinate of the second endpoint in display coordinates.
//      y2      The y-coordinate of the second endpoint in display coordinates.
//
//  Programmer: Hank Childs
//  Creation:   May 24, 2000
//
// ****************************************************************************

void
ZoomInteractor::DrawRubberBandLine(int x1, int y1, int x2, int y2)
{
    //
    // The rubber band is drawn to the background since it is also in display
    // coordinates.
    //
    vtkViewport *ren = proxy.GetBackground();
    vtkPoints *pts = rubberBand->GetPoints();
    pts->SetPoint(0, (double) x1, (double) y1, 0.);   
    pts->SetPoint(1, (double) x2, (double) y2, 0.);   
    rubberBandMapper->RenderOverlay(ren, rubberBandActor);
}

// ****************************************************************************
//  Method: ZoomInteractor::SufficientDistanceMoved
//
//  Purpose:
//      Determines if the current zoom coordinates are sufficiently far apart
//      to justify performing the zoom, or if it should be cancelled instead
//
//  Programmer: Marc Durant
//  Creation:   December 19, 2011
//
// ****************************************************************************

bool
ZoomInteractor::SufficientDistanceMoved() {

    float distX = (lastX - anchorX) * (lastX - anchorX);
    if (distX <= 16) {
        return false;
    }

    float distY = (lastY - anchorY) * (lastY - anchorY);
    if (distY <= 16) {
        return false;
    }

    return true;
}
