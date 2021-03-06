c-----------------------------------------------------------------------------
c
c Copyright (c) 2000 - 2013, Lawrence Livermore National Security, LLC
c Produced at the Lawrence Livermore National Laboratory
c LLNL-CODE-442911
c All rights reserved.
c
c This file is  part of VisIt. For  details, see https://visit.llnl.gov/.  The
c full copyright notice is contained in the file COPYRIGHT located at the root
c of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
c
c Redistribution  and  use  in  source  and  binary  forms,  with  or  without
c modification, are permitted provided that the following conditions are met:
c
c  - Redistributions of  source code must  retain the above  copyright notice,
c    this list of conditions and the disclaimer below.
c  - Redistributions in binary form must reproduce the above copyright notice,
c    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
c    documentation and/or other materials provided with the distribution.
c  - Neither the name of  the LLNS/LLNL nor the names of  its contributors may
c    be used to endorse or promote products derived from this software without
c    specific prior written permission.
c
c THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
c AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
c IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
c ARE  DISCLAIMED. IN  NO EVENT  SHALL LAWRENCE  LIVERMORE NATIONAL  SECURITY,
c LLC, THE  U.S.  DEPARTMENT OF  ENERGY  OR  CONTRIBUTORS BE  LIABLE  FOR  ANY
c DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,  OR   CONSEQUENTIAL
c DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
c SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
c CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
c LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
c OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
c DAMAGE.
c
c-----------------------------------------------------------------------------

c-----------------------------------------------------------------
c Program: main
c
c Programmer: Brad Whitlock
c Date:       Thu Mar 11 12:13:06 PST 2010
c
c Modifications:
c
c-----------------------------------------------------------------
      program main
      implicit none
      include "visitfortransimV2interface.inc"
ccc   local variables
      integer err

      call simulationarguments()
      err = visitsetupenv()
      err = visitinitializesim("fupdateplots", 12,
     . "Demonstrates visitupdateplots function", 38,
     . "/no/useful/path", 15,
     . VISIT_F77NULLSTRING, VISIT_F77NULLSTRINGLEN,
     . VISIT_F77NULLSTRING, VISIT_F77NULLSTRINGLEN,
     . VISIT_F77NULLSTRING, VISIT_F77NULLSTRINGLEN)
      call mainloop()
      stop
      end

c-----------------------------------------------------------------
c mainloop
c-----------------------------------------------------------------
      subroutine mainloop()
      implicit none
      include "visitfortransimV2interface.inc"
ccc   local variables
      integer visitstate, result, blocking
ccc   SIMSTATE common block
      integer runflag, simcycle, simUpdate
      double precision simtime
      common /SIMSTATE/ simtime, runflag, simcycle, simUpdate
      save /SIMSTATE/

c     main loop
      runflag = 1
      simcycle = 0
      simtime = 0.
      simUpdate = 0
      do 10
          if(runflag.eq.1) then
              blocking = 0 
          else
              blocking = 1
          endif

          visitstate = visitdetectinput(blocking, -1)

          if (visitstate.lt.0) then
              goto 1234
          elseif (visitstate.eq.0) then
              call simulate_one_timestep()
          elseif (visitstate.eq.1) then
              runflag = 0
              result = visitattemptconnection()
              if (result.eq.1) then
                  write (6,*) 'VisIt connected!'
                  simUpdate = 1
              else
                  write (6,*) 'VisIt did not connect!'
              endif
          elseif (visitstate.eq.2) then
              runflag = 0
              if (visitprocessenginecommand().eq.0) then
                  result = visitdisconnect()
                  runflag = 1
              endif
          endif
10    continue
1234  end

      subroutine simulate_one_timestep()
      implicit none
      include "visitfortransimV2interface.inc" 
ccc   SIMSTATE common block
      integer runFlag, simcycle, simUpdate
      double precision simtime
      common /SIMSTATE/ simtime, runflag, simcycle, simUpdate
ccc   RECTMESH common block
      integer NX, NY
      parameter (NX = 50)
      parameter (NY = 50)
      real rmx(NX), rmy(NY), zonal(NX-1,NY-1)
      integer rmdims(3), rmndims
      common /RECTMESH/ rmdims, rmndims, rmx, rmy, zonal
      save /RECTMESH/
c Rectilinear mesh data
      data rmndims /2/
      data rmdims /NX, NY, 1/
ccc   Local vars
      integer err

c Simulate one time step
      simcycle = simcycle + 1
      simtime = simtime + 3.14159 / 10.
      write (6,*) 'Simulating time step: cycle=',simcycle, 
     .            ' time=', simtime

      if(simUpdate.eq.1) then
c         Tell VisIt that the timestep changed
          err = visittimestepchanged()
c         Tell VisIt to update its plots
          err = visitupdateplots()
      endif
      end

ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c
c These functions must be defined to satisfy the visitfortransimV2interface lib.
c
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc

c---------------------------------------------------------------------------
c visitcommandcallback
c---------------------------------------------------------------------------
      subroutine visitcommandcallback (cmd, lcmd, args, largs) 
      implicit none
      character*8 cmd, args
      integer     lcmd, largs
      include "visitfortransimV2interface.inc"
ccc   SIMSTATE common block
      integer runflag, simcycle, simUpdate
      double precision simtime
      common /SIMSTATE/ simtime, runflag, simcycle, simUpdate

c     Handle the commands that we define in visitgetmetadata.
      if(visitstrcmp(cmd, lcmd, "halt", 4).eq.0) then
          runflag = 0
      elseif(visitstrcmp(cmd, lcmd, "step", 4).eq.0) then
          call simulate_one_timestep()
      elseif(visitstrcmp(cmd, lcmd, "run", 3).eq.0) then
          runflag = 1
      endif
      end

c---------------------------------------------------------------------------
c visitbroadcastintfunction
c---------------------------------------------------------------------------
      integer function visitbroadcastintfunction(value, sender)
      implicit none
      integer value, sender
c     REPLACE WITH MPI COMMUNICATION IF SIMULATION IS PARALLEL
      visitbroadcastintfunction = 0
      end

c---------------------------------------------------------------------------
c visitbroadcaststringfunction
c---------------------------------------------------------------------------
      integer function visitbroadcaststringfunction(str, lstr, sender)
      implicit none
      character*8 str
      integer     lstr, sender
c     REPLACE WITH MPI COMMUNICATION IF SIMULATION IS PARALLEL
      visitbroadcaststringfunction = 0
      end

c---------------------------------------------------------------------------
c visitslaveprocesscallback
c---------------------------------------------------------------------------
      subroutine visitslaveprocesscallback ()
      implicit none
c     REPLACE WITH MPI COMMUNICATION IF SIMULATION IS PARALLEL
      end

c---------------------------------------------------------------------------
c visitactivatetimestep
c---------------------------------------------------------------------------
      integer function visitactivatetimestep()
      implicit none
      include "visitfortransimV2interface.inc"
      visitactivatetimestep = VISIT_OKAY
      end

c---------------------------------------------------------------------------
c visitgetmetadata
c---------------------------------------------------------------------------
      integer function visitgetmetadata()
      implicit none
      include "visitfortransimV2interface.inc"
ccc   SIMSTATE common block
      integer runflag, simcycle, simUpdate
      double precision simtime
      common /SIMSTATE/ simtime, runflag, simcycle, simUpdate
      integer md, mmd, vmd, cmd, err

      if(visitmdsimalloc(md).eq.VISIT_OKAY) then
          err = visitmdsimsetcycletime(md, simcycle, simtime)
          if(runflag.eq.1) then
              err = visitmdsimsetmode(md, VISIT_SIMMODE_RUNNING)
          else
              err = visitmdsimsetmode(md, VISIT_SIMMODE_STOPPED)
          endif

c     Add a 2D rectilinear mesh
          err = visitmdmeshalloc(mmd);
          if(err.eq.VISIT_OKAY) then
              err = visitmdmeshsetname(mmd, "mesh2d", 6)
              err = visitmdmeshsetmeshtype(mmd, 
     .            VISIT_MESHTYPE_RECTILINEAR)
              err = visitmdmeshsettopologicaldim(mmd, 2)
              err = visitmdmeshsetspatialdim(mmd, 2)
              err = visitmdmeshsetnumdomains(mmd, 1)
              err = visitmdmeshsetdomaintitle(mmd, "Domains", 7)
              err = visitmdmeshsetdomainpiecename(mmd, "domain", 6)
              err = visitmdmeshsetxunits(mmd, "cm", 2)
              err = visitmdmeshsetyunits(mmd, "cm", 2)
              err = visitmdmeshsetxlabel(mmd, "Width", 5)
              err = visitmdmeshsetylabel(mmd, "Height", 6)
              err = visitmdmeshsetcellorigin(mmd, 1)
              err = visitmdmeshsetnodeorigin(mmd, 1)
              err = visitmdsimaddmesh(md, mmd)
          endif

c     Add a zonal variable on mesh2d
      if(visitmdvaralloc(vmd).eq.VISIT_OKAY) then
          err = visitmdvarsetname(vmd, "zonal", 5)
          err = visitmdvarsetmeshname(vmd, "mesh2d", 6)
          err = visitmdvarsetcentering(vmd, VISIT_VARCENTERING_ZONE)
          err = visitmdvarsettype(vmd, VISIT_VARTYPE_SCALAR)
          err = visitmdsimaddvariable(md, vmd)
      endif

c     Add a curve variable
      if(visitmdcurvealloc(cmd).eq.VISIT_OKAY) then
          err = visitmdcurvesetname(cmd, "sine", 4)
          err = visitmdcurvesetxlabel(cmd, "angle", 5)
          err = visitmdcurvesetxunits(cmd, "radians", 7)
          err = visitmdcurvesetylabel(cmd, "amplitude", 9)
          err = visitmdsimaddcurve(md, cmd)
      endif

c     Add simulation commands
          err = visitmdcmdalloc(cmd)
          if(err.eq.VISIT_OKAY) then
              err = visitmdcmdsetname(cmd, "halt", 4)
              err = visitmdsimaddgenericcommand(md, cmd)
          endif
          err = visitmdcmdalloc(cmd)
          if(err.eq.VISIT_OKAY) then
              err = visitmdcmdsetname(cmd, "step", 4)
              err = visitmdsimaddgenericcommand(md, cmd)
          endif
          err = visitmdcmdalloc(cmd)
          if(err.eq.VISIT_OKAY) then
              err = visitmdcmdsetname(cmd, "run", 3)
              err = visitmdsimaddgenericcommand(md, cmd)
          endif
      endif
      visitgetmetadata = md
      end

c---------------------------------------------------------------------------
c visitgetmesh
c---------------------------------------------------------------------------
      integer function visitgetmesh(domain, name, lname)
      implicit none
      character*8 name
      integer     domain, lname
      include "visitfortransimV2interface.inc" 
ccc   RECTMESH common block (shared with simulate_one_timestep)
      integer NX, NY
      parameter (NX = 50)
      parameter (NY = 50)
      real rmx(NX), rmy(NY), zonal(NX-1,NY-1)
      integer rmdims(3), rmndims
      common /RECTMESH/ rmdims, rmndims, rmx, rmy, zonal
ccc   local variables
      integer I, J, h, x, y, err

      h = VISIT_INVALID_HANDLE
      if(visitstrcmp(name, lname, "mesh2d", 6).eq.0) then          
c Create a rectilinear mesh here
          if(visitrectmeshalloc(h).eq.VISIT_OKAY) then
c Create mesh coordinates
              do 300 I=1,NX
                  rmx(I) = (float(I-1)/float(NX-1)) * 5. - 2.5
300           continue
              do 310 I=1,NY
                  rmy(I) = (float(I-1)/float(NY-1)) * 5. - 2.5
310           continue

              err = visitvardataalloc(x)
              err = visitvardataalloc(y)
              err = visitvardatasetf(x,VISIT_OWNER_SIM,1,NX,rmx)
              err = visitvardatasetf(y,VISIT_OWNER_SIM,1,NY,rmy)

              err = visitrectmeshsetcoordsxy(h, x, y)
          endif
      endif
      visitgetmesh = h
      end

c---------------------------------------------------------------------------
c visitgetvariable
c---------------------------------------------------------------------------
      integer function visitgetvariable(domain, name, lname)
      implicit none
      character*8 name
      integer     domain, lname
      include "visitfortransimV2interface.inc"
ccc   SIMSTATE common block
      integer runFlag, simcycle, simUpdate
      double precision simtime
      common /SIMSTATE/ simtime, runflag, simcycle, simUpdate
ccc   RECTMESH common block
      integer NX, NY
      parameter (NX = 50)
      parameter (NY = 50)
      real rmx(NX), rmy(NY), zonal(NX-1,NY-1)
      integer rmdims(3), rmndims
      common /RECTMESH/ rmdims, rmndims, rmx, rmy, zonal
ccc   local vars
      integer h, I, J, err
      real angle, xpos, ypos, cellX, cellY, dX, dY, tx, ty

      h = VISIT_INVALID_HANDLE
      if(visitstrcmp(name, lname, "zonal", 5).eq.0) then
c         Calculate a zonal variable that depends on the simulation time.
          angle = simtime
          xpos = 2.5 * cos(angle)
          ypos = 2.5 * sin(angle)
          do 5010 J=1,NY-1
              ty = float(J-1) / float(NY-2)
              cellY = (1.-ty)*(-2.5) + 2.5*ty
              dY = cellY - ypos
              do 5000 I=1,NX-1
                  tx = float(I-1) / float(NX-2)
                  cellX = (1.-tx)*(-2.5) + 2.5*tx
                  dX = cellX - xpos
                  zonal(I,J) = sqrt(dX * dX + dY * dY)
5000          continue
5010      continue

          if(visitvardataalloc(h).eq.VISIT_OKAY) then
              err = visitvardatasetf(h,VISIT_OWNER_SIM,1,
     .            (NX-1)*(NY-1),zonal)
          endif
      endif

      visitgetvariable = h
      end

c---------------------------------------------------------------------------
c visitgetcurve
c---------------------------------------------------------------------------
      integer function visitgetcurve(name, lname)
      implicit none
      character*8 name
      integer     lname
      include "visitfortransimV2interface.inc"
ccc   SIMSTATE common block
      integer runFlag, simcycle, simUpdate
      double precision simtime
      common /SIMSTATE/ simtime, runflag, simcycle, simUpdate
ccc   local vars
      integer i, h, hx, hy, NPTS, err
      parameter   (NPTS = 200)
      real        x(NPTS), y(NPTS), t, angle

      h = VISIT_INVALID_HANDLE
      if(visitstrcmp(name, lname, "sine", 4).eq.0) then
          do 10000 i=1,NPTS
              t = float(i-1) / float(NPTS-1)
              angle = simtime + t * 4. * 3.14159
              x(i) = angle
              y(i) = sin(x(i))
10000     continue

          if(visitcurvedataalloc(h).eq.VISIT_OKAY) then
              err = visitvardataalloc(hx)
              err = visitvardataalloc(hy)
              err = visitvardatasetf(hx, VISIT_OWNER_COPY, 1, NPTS, x)
              err = visitvardatasetf(hy, VISIT_OWNER_COPY, 1, NPTS, y)
              err = visitcurvedatasetcoordsxy(h, hx, hy)
          endif
      endif
      visitgetcurve = h
      end

c---------------------------------------------------------------------------
c visitgetdomainlist
c---------------------------------------------------------------------------
      integer function visitgetdomainlist(name, lname)
      implicit none
      character*8 name
      integer     lname
      include "visitfortransimV2interface.inc"
      visitgetdomainlist = VISIT_INVALID_HANDLE
      end

c---------------------------------------------------------------------------
c visitgetdomainbounds
c---------------------------------------------------------------------------
      integer function visitgetdomainbounds(name, lname)
      implicit none
      character*8 name
      integer     lname
      include "visitfortransimV2interface.inc"
      visitgetdomainbounds = VISIT_INVALID_HANDLE
      end

c---------------------------------------------------------------------------
c visitgetdomainnesting
c---------------------------------------------------------------------------
      integer function visitgetdomainnesting(name, lname)
      implicit none
      character*8 name
      integer     lname
      include "visitfortransimV2interface.inc"
      visitgetdomainnesting = VISIT_INVALID_HANDLE
      end

c---------------------------------------------------------------------------
c visitgetmaterial
c---------------------------------------------------------------------------
      integer function visitgetmaterial(domain, name, lname)
      implicit none
      character*8 name
      integer     domain, lname
      include "visitfortransimV2interface.inc"
      visitgetmaterial = VISIT_INVALID_HANDLE
      end





























