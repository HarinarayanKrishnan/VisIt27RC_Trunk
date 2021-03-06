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
//                            VisWinParallelAxes.h                           //
// ************************************************************************* //

#ifndef VIS_WIN_PARALLEL_AXES_H
#define VIS_WIN_PARALLEL_AXES_H
#include <viswindow_exports.h>


#include <VisWinColleague.h>


class vtkVisItAxisActor2D;
class VisWindowColleagueProxy;

// ****************************************************************************
//  Class: VisWinParallelAxes
//
//  Purpose:
//      This is a concrete colleague for the mediator VisWindow.  It puts
//      updating axes around plots with arrays of parallel axes, like the
//      Parallel Curves plot.
//
//  Programmer: Eric Brugger
//  Creation:   December 9, 2008
//
//  Modifications:
//    Eric Brugger, Tue Jan 20 11:33:00 PST 2009
//    I implemented autoSetTicks, labelVisibility, titleVisibility,
//    tickVisibility and setting the major and minor tick locations.
//
//    Eric Brugger, Mon Nov  5 15:19:24 PST 2012
//    I added the ability to display the parallel axes either horizontally
//    or vertically.
//
// ****************************************************************************

class VISWINDOW_API VisWinParallelAxes : public VisWinColleague
{
  public:
                              VisWinParallelAxes(VisWindowColleagueProxy &);
    virtual                  ~VisWinParallelAxes();

    virtual void              SetForegroundColor(double, double, double);
    virtual void              UpdateView(void);
    virtual void              SetViewport(double, double, double, double);
    virtual void              UpdatePlotList(std::vector<avtActor_p> &);

    virtual void              HasPlots(void);
    virtual void              NoPlots(void);

    virtual void              StartParallelAxesMode(void);
    virtual void              StopParallelAxesMode(void);

    void                      SetTitles();

    void                      SetLabelVisibility(int);
    void                      SetTitleVisibility(int);
    void                      SetVisibility(int);
    void                      SetTickVisibility(bool,bool);
    void                      SetAutoSetTicks(int);
    void                      SetMajorTickMinimum(double);
    void                      SetMajorTickMaximum(double);
    void                      SetMajorTickSpacing(double);
    void                      SetMinorTickSpacing(double);
    void                      SetLabelFontHeight(double);
    void                      SetTitleFontHeight(double);
    void                      SetLineWidth(int);
    void                      SetLabelScaling(bool, int);
    void                      SetLabelTextAttributes(
                                  const VisWinTextAttributes &atts);
    void                      SetTitleTextAttributes(
                                  const VisWinTextAttributes &atts);

    enum Orientation {
        Horizontal,
        Vertical
    };
    void                      SetAxisOrientation(Orientation);
  protected:
    struct AxisInfo {
        vtkVisItAxisActor2D *axis;
        vtkVisItAxisActor2D *axisCap1;
        vtkVisItAxisActor2D *axisCap2;
        int lastPow;
        int lastAxisDigits;
        char units[256];
        char title[256];
        double range[2];
        double xpos;
        AxisInfo()
            : axis(NULL), lastPow(0), lastAxisDigits(3)
        {
            xpos = 0;
            units[0] = '\0';
            title[0] = '\0';
        }
        AxisInfo(vtkVisItAxisActor2D *a, int lp, int lad)
            : axis(a), lastPow(lp), lastAxisDigits(lad)
        {
            xpos = 0;
            units[0] = '\0';
            title[0] = '\0';
        }
    };
    std::vector<AxisInfo> axes;

    bool                      addedAxes;
    bool                      autolabelScaling;
    double                    vl, vr;
    double                    vb, vt;

    double                    fr, fg, fb;
    int                       axisVisibility;
    bool                      labelVisibility;
    bool                      titleVisibility;
    bool                      tickVisibility;
    bool                      tickLabelVisibility;
    bool                      autoSetTicks;
    double                    majorTickMinimum;
    double                    majorTickMaximum;
    double                    majorTickSpacing;
    double                    minorTickSpacing;
    double                    labelFontHeight;
    double                    titleFontHeight; 
    int                       lineWidth;
    VisWinTextAttributes      titleTextAttributes;
    VisWinTextAttributes      labelTextAttributes;
    int                       axisPow;
    int                       userPow;
    Orientation               axisOrientation;

    void                      SetNumberOfAxes(int);
    bool                      AdjustValues(double, double);
    void                      AdjustRange(double, double);
    void                      GetRange(double &, double &, double &, double &);
    void                      AddAxesToWindow(void);
    void                      RemoveAxesFromWindow(void);
    bool                      ShouldAddAxes(void);
    void                      UpdateLabelTextAttributes(double, double,
                                  double);
    void                      UpdateTitleTextAttributes(double, double,
                                  double);
};


#endif


