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
//                            avtADIOSFileFormat.C                           //
// ************************************************************************* //

#include <avtMTMDFileFormatInterface.h>
#include <avtParallel.h>
#include <ADIOSFileObject.h>
#include <avtADIOSBasicFileFormat.h>
#include <vtkRectilinearGrid.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <avtDatabaseMetaData.h>
#include <DBOptionsAttributes.h>
#include <InvalidDBTypeException.h>
#include <InvalidVariableException.h>
#include <DebugStream.h>
#include <avtDatabase.h>
#include <sys/types.h>

#include <map>
#include <string>
#include <vector>

// ****************************************************************************
//  Method: avtEMSTDFileFormat::Identify
//
//  Purpose:
//      Determine if this file is of this flavor.
//
//  Programmer: Dave Pugmire
//  Creation:   Wed Feb 10 15:55:14 EST 2010
//
// ****************************************************************************

bool
avtADIOSBasicFileFormat::Identify(const char *fname)
{
    //Any bp file is a basic file format.
    return true;
}


// ****************************************************************************
//  Method: avtEMSTDFileFormat::CreateInterface
//
//  Purpose:
//      Create an interface for this reader.
//
//  Programmer: Dave Pugmire
//  Creation:   Wed Feb 10 15:55:14 EST 2010
//
// ****************************************************************************

avtFileFormatInterface *
avtADIOSBasicFileFormat::CreateInterface(const char *const *list,
                                         int nList,
                                         int nBlock)
{
    int nTimestepGroups = nList / nBlock;
    avtMTMDFileFormat **ffl = new avtMTMDFileFormat*[nTimestepGroups];
    for (int i = 0 ; i < nTimestepGroups ; i++)
        ffl[i] = new avtADIOSBasicFileFormat(list[i*nBlock]);
    
    return new avtMTMDFileFormatInterface(ffl, nTimestepGroups);
}

// ****************************************************************************
//  Method: avtADIOSBasicFileFormat constructor
//
//  Programmer: Dave Pugmire
//  Creation:   Thu Sep 17 11:23:05 EDT 2009
//
// ****************************************************************************

avtADIOSBasicFileFormat::avtADIOSBasicFileFormat(const char *nm)
    : avtMTMDFileFormat(nm)
{
    fileObj = new ADIOSFileObject(nm);
    initialized = false;
}

// ****************************************************************************
//  Method: avtADIOSBasicFileFormat destructor
//
//  Programmer: Dave Pugmire
//  Creation:   Thu Sep 17 11:23:05 EDT 2009
//
// ****************************************************************************

avtADIOSBasicFileFormat::~avtADIOSBasicFileFormat()
{
    delete fileObj;
    fileObj = NULL;
}


// ****************************************************************************
//  Method: avtEMSTDFileFormat::GetNTimesteps
//
//  Purpose:
//      Tells the rest of the code how many timesteps there are in this file.
//
//  Programmer: Dave Pugmire
//  Creation:   Thu Sep 17 11:23:05 EDT 2009
//
// ****************************************************************************

int
avtADIOSBasicFileFormat::GetNTimesteps()
{
  cout<<"NSTEPS: "<<fileObj->NumTimeSteps()<<endl;
    return fileObj->NumTimeSteps();
}

// ****************************************************************************
//  Method: avtEMSTDFileFormat::GetCycles
//
//  Purpose:
//      
//
//  Programmer: Dave Pugmire
//  Creation:   Thu Sep 17 11:23:05 EDT 2009
//
// ****************************************************************************

void
avtADIOSBasicFileFormat::GetCycles(std::vector<int> &cycles)
{
  int nt = fileObj->NumTimeSteps();
  cycles.resize(nt);
  for (int i = 0; i < nt; i++)
        cycles[i] = i;
  
  cout<<"cycles sz= "<<cycles.size()<<endl;
}


// ****************************************************************************
//  Method: avtADIOSBasicFileFormat::FreeUpResources
//
//  Purpose:
//      When VisIt is done focusing on a particular timestep, it asks that
//      timestep to free up any resources (memory, file descriptors) that
//      it has associated with it.  This method is the mechanism for doing
//      that.
//
//  Programmer: Dave Pugmire
//  Creation:   Thu Sep 17 11:23:05 EDT 2009
//
// ****************************************************************************

void
avtADIOSBasicFileFormat::FreeUpResources(void)
{
}


// ****************************************************************************
//  Method: avtADIOSBasicFileFormat::PopulateDatabaseMetaData
//
//  Purpose:
//      This database meta-data object is like a table of contents for the
//      file.  By populating it, you are telling the rest of VisIt what
//      information it can request from you.
//
//  Programmer: Dave Pugmire
//  Creation:   Thu Sep 17 11:23:05 EDT 2009
//
//  Modifications:
//   Dave Pugmire, Tue Mar  9 12:40:15 EST 2010
//   Use name/originalName for names starting with /.
//
// ****************************************************************************

void
avtADIOSBasicFileFormat::PopulateDatabaseMetaData(avtDatabaseMetaData *md, int timeState)
{
    debug5<<"avtADIOSBasicFileFormat::PopulateDatabaseMetaData()"<<endl;
    Initialize();
    md->SetFormatCanDoDomainDecomposition(true);
    
    // Add 2D/3D mesh metadata
    std::map<std::string, meshInfo>::const_iterator m;
    for (m = meshes.begin(); m != meshes.end(); m++)
    {
        avtMeshMetaData *mesh = new avtMeshMetaData;
        mesh->name = m->first;
        mesh->meshType = AVT_RECTILINEAR_MESH;
        mesh->numBlocks = 1; // must be 1 for automatic decomposition.
        mesh->blockOrigin = 0;
        mesh->spatialDimension = m->second.dim;
        mesh->topologicalDimension = m->second.dim;
        mesh->blockTitle = "blocks";
        mesh->blockPieceName = "block";
        mesh->hasSpatialExtents = false;
        md->Add(mesh);
        
        debug5 <<"added metadata: mesh "<<m->first<<endl;
    }

    // Add variables' metadata
    std::map<std::string, ADIOSVar>::const_iterator v;
    for (v = fileObj->variables.begin(); v != fileObj->variables.end(); v++)
    {
        if (v->second.dim == 1) 
        { 
            // define as a curve
            avtCurveMetaData *curve = new avtCurveMetaData;
            curve->name = v->first;
            if (curve->name[0] == '/')
            {
                curve->originalName = curve->name;
                curve->name = std::string(&curve->name[1]);
            }
            md->Add(curve);
            debug5<< "added metadata: curve " << v->first << endl;

        } 
        else
        {
            std::string meshName = GenerateMeshName(v->second);
            avtScalarMetaData *smd = new avtScalarMetaData();
            
            smd->name = v->first;
            if (smd->name[0] == '/')
            {
                smd->originalName = smd->name;
                smd->name = std::string(&smd->name[1]);
            }

            smd->meshName = meshName;
            smd->centering = AVT_NODECENT;
            md->Add(smd);
            debug5 << "added metadata: var "<<v->first<<" on mesh "<<meshName<<endl;
        }
    }
}


// ****************************************************************************
//  Method: avtADIOSBasicFileFormat::GetMesh
//
//  Purpose:
//      Gets the mesh associated with this file.  The mesh is returned as a
//      derived type of vtkDataSet (ie vtkRectilinearGrid, vtkStructuredGrid,
//      vtkUnstructuredGrid, etc).
//
//  Arguments:
//      timestate   The index of the timestate.  If GetNTimesteps returned
//                  'N' time steps, this is guaranteed to be between 0 and N-1.
//      meshname    The name of the mesh of interest.  This can be ignored if
//                  there is only one mesh.
//
//  Programmer: Dave Pugmire
//  Creation:   Thu Sep 17 11:23:05 EDT 2009
//
// ****************************************************************************

vtkDataSet *
avtADIOSBasicFileFormat::GetMesh(int timestate, int domain, const char *meshname)
{
    debug1 << "avtADIOSBasicFileFormat::GetMesh " << meshname << endl;
    cout << "avtADIOSBasicFileFormat::GetMesh " << meshname << " "<<domain<<" "<<timestate<<endl;
    Initialize();

    // Look it up in the mesh table.
    std::map<std::string,meshInfo>::const_iterator m = meshes.find(meshname);
    if (m != meshes.end())
    {
        vtkRectilinearGrid *grid = NULL;
        grid = CreateUniformGrid(m->second.start,
                                 m->second.count);
        grid->Register(NULL);
        return grid;
    }

    //It might be a curve.
    std::map<std::string, ADIOSVar>::const_iterator v = fileObj->variables.find(meshname);
    if (v != fileObj->variables.end() && v->second.dim == 1)
    {
        vtkRectilinearGrid *grid = NULL;
        grid = CreateUniformGrid(v->second.start,
                                 v->second.count);
        vtkDataArray *vals = NULL;
        if (!fileObj->ReadVariable(meshname, timestate, 0, &vals))
            EXCEPTION1(InvalidVariableException, meshname);
        vals->SetName(meshname);
        grid->GetPointData()->SetScalars(vals);
        vals->Delete();
        grid->Register(NULL);
        return grid;
    }
    debug1<<meshname<<" not found"<<endl;
    EXCEPTION1(InvalidVariableException, meshname);
}


// ****************************************************************************
//  Method: avtADIOSBasicFileFormat::GetVar
//
//  Purpose:
//      Gets a scalar variable associated with this file.  Although VTK has
//      support for many different types, the best bet is vtkFloatArray, since
//      that is supported everywhere through VisIt.
//
//  Arguments:
//      timestate  The index of the timestate.  If GetNTimesteps returned
//                 'N' time steps, this is guaranteed to be between 0 and N-1.
//      varname    The name of the variable requested.
//
//  Programmer: Dave Pugmire
//  Creation:   Thu Sep 17 11:23:05 EDT 2009
//
// ****************************************************************************

vtkDataArray *
avtADIOSBasicFileFormat::GetVar(int timestate, int domain, const char *varname)
{
    debug1 << "avtADIOSBasicFileFormat::GetVar " << varname << endl;
    cout << "avtADIOSBasicFileFormat::GetVar " << varname << " "<<timestate<<" "<<domain<<endl;
    Initialize();
    vtkDataArray *arr = NULL;
    if (! fileObj->ReadVariable(varname, timestate, 0, &arr))
        EXCEPTION1(InvalidVariableException, varname);
        
    return arr;
}


// ****************************************************************************
//  Method: avtADIOSBasicFileFormat::GetVectorVar
//
//  Purpose:
//      Gets a vector variable associated with this file.  Although VTK has
//      support for many different types, the best bet is vtkFloatArray, since
//      that is supported everywhere through VisIt.
//
//  Arguments:
//      timestate  The index of the timestate.  If GetNTimesteps returned
//                 'N' time steps, this is guaranteed to be between 0 and N-1.
//      varname    The name of the variable requested.
//
//
//  Programmer: Dave Pugmire
//  Creation:   Thu Sep 17 11:23:05 EDT 2009
//
// ****************************************************************************

vtkDataArray *
avtADIOSBasicFileFormat::GetVectorVar(int timestate, int domain, const char *varname)
{
    return NULL;
}

// ****************************************************************************
//  Method: avtADIOSBasicFileFormat::GenerateMeshName
//
//  Purpose:
//      Create a generic mesh name from the dimensions of the variable.
//
//  Programmer: Dave Pugmire
//  Creation:   Wed Feb 10 16:08:01 EST 2010
//
// ****************************************************************************

std::string
avtADIOSBasicFileFormat::GenerateMeshName(const ADIOSVar &v)
{
    std::vector<int64_t> dimT, dims;
    std::string meshname = "mesh_";

    for (int i=0; i<v.dim; i++)
        dims.insert(dims.begin(), v.global[i]);

    for (int i=0; i <dims.size(); i++)
    {
        std::stringstream ss;
        ss<<dims[i];
        meshname += ss.str();
        if (i<dims.size()-1)
            meshname += "x";
    }
    return meshname;
}

// ****************************************************************************
//  Method: avtADIOSBasicFileFormat::Initialize
//
//  Purpose:
//      Initialize the reader.
//
//  Programmer: Dave Pugmire
//  Creation:   Thu Sep 17 11:23:05 EDT 2009
//
// ****************************************************************************

void
avtADIOSBasicFileFormat::Initialize()
{
  cout<<"INitialize()"<<endl;
    if (! fileObj->Open())
        EXCEPTION0(ImproperUseException);

    if (initialized)
        return;

    //Create meshes for each variable.
    std::map<std::string, ADIOSVar>::iterator vi;
    for (vi = fileObj->variables.begin(); vi != fileObj->variables.end(); ++vi)
    {
        ADIOSVar &v = (*vi).second;
        
        std::string meshname = GenerateMeshName(v);
        
        //Add mesh, if not found...
        if (meshes.find(meshname) == meshes.end())
        {
            meshInfo mi;
            for (int i=0; i<3; i++)
            {
                mi.start[i] = v.start[i];
                mi.count[i] = v.count[i];
                mi.global[i] = v.global[i];
            }
            mi.dim = v.dim;
            mi.name = meshname;
            meshes[meshname] = mi;
            
            debug5<<"Add mesh "<<meshname<<" ["<<mi.count[0]<<" "<<mi.count[1]<<" "<<mi.count[2]<<"]"<<endl;
        }
    }
    
    DoDomainDecomposition();
    
    initialized = true;
}

// ****************************************************************************
//  Method: avtADIOSBasicFileFormat::ComputeStartCount
//
//  Purpose:
//      Figure out the start/count arrays for a given processor count.
//
//  Programmer: Dave Pugmire
//  Creation:   Wed Feb 10 16:08:01 EST 2010
//
//  Modifications:
//   Dave Pugmire, Tue Mar  9 12:40:15 EST 2010
//   Use uint64_t for start/count arrays.
//
// ****************************************************************************

void
avtADIOSBasicFileFormat::ComputeStartCount(uint64_t *globalDims,
                                           int dim,
                                           uint64_t *start,
                                           uint64_t *count)
{
#if PARALLEL
    int domCount[3] = {0, 0, 0};
    int s[3] = {start[0],start[1],start[2]}, c[3] = {count[0],count[1],count[2]};
    avtDatabase::ComputeRectilinearDecomposition(dim, PAR_Size(),
                                                 globalDims[0], globalDims[1], globalDims[2],
                                                 &domCount[0], &domCount[1], &domCount[2]);
    
    // Determine this processor's logical domain (e.g. domain ijk) indices
    int domLogicalCoords[3] = {0, 0, 0};
    avtDatabase::ComputeDomainLogicalCoords(dim, domCount, PAR_Rank(),
                                            domLogicalCoords);
    
    // Compute domain bounds.
    for (int i = 0; i < 3; i++)
    {
        avtDatabase::ComputeDomainBounds(globalDims[i], domCount[i], domLogicalCoords[i],
                                         &(s[i]),
                                         &(c[i]));
        
        c[i]++;
        if (s[i]+c[i] >= globalDims[i])
            c[i]--;
        
        start[i] = s[i];
        count[i] = c[i];
    }
#endif
}


// ****************************************************************************
//  Method: avtADIOSBasicFileFormat::DoDomainDecomposition
//
//  Purpose:
//      Decompose across processor set.
//
//  Programmer: Dave Pugmire
//  Creation:   Thu Sep 17 11:23:05 EDT 2009
//
//  Modifications:
//   Dave Pugmire, Tue Mar  9 12:40:15 EST 2010
//   Use uint64_t for start/count arrays.
//
// ****************************************************************************

void
avtADIOSBasicFileFormat::DoDomainDecomposition()
{
    debug5<<"avtADIOSBasicFileFormat::DoDomainDecomposition()"<<endl;

#ifdef PARALLEL
    //Set the variables.
    std::map<std::string, ADIOSVar>::iterator v;
    for (v = fileObj->variables.begin(); v != fileObj->variables.end(); v++)
    {
        uint64_t globalDims[3], start[3], count[3];
        
        for (int i = 0; i < 3; i++)
            globalDims[i] = v->second.global[i];

        avtADIOSBasicFileFormat::ComputeStartCount(globalDims, v->second.dim, start, count);
        
        for (int i = 0; i < 3; i++)
        {
            v->second.start[i] = start[i];
            v->second.count[i] = count[i];
        }
    }

        //Set each mesh.v
    std::map<std::string, meshInfo>::iterator m;
    for (m = meshes.begin(); m != meshes.end(); m++)
    {

        uint64_t globalDims[3] = {1,1,1};
        uint64_t start[3], count[3];
        for (int i = 0; i < 3; i++)
            globalDims[i] = m->second.global[i];

        avtADIOSBasicFileFormat::ComputeStartCount(globalDims, m->second.dim, start, count);
        
        for (int i = 0; i < 3; i++)
        {
            m->second.start[i] = start[i];
            m->second.count[i] = count[i];
        }
    }
    
#endif
}


// ****************************************************************************
//  Method: avtADIOSBasicFileFormat::CreateUniformGrid
//
//  Purpose: 
//      Make a 0..x-1,0..y-1,0..z-1 uniform grid with stepping 1
//      Used for both 2D/3D variables (mesh) and 1D variables (curve)
//      y/z must be 1 at least since vtkRectilinearGrid is always a 3D structure
//
//  Programmer: Dave Pugmire
//  Creation:   Thu Sep 17 11:23:05 EDT 2009
//
//  Modifications:
//   Dave Pugmire, Tue Mar  9 12:40:15 EST 2010
//   Use uint64_t for start/count arrays.
//
// ****************************************************************************

vtkRectilinearGrid *
avtADIOSBasicFileFormat::CreateUniformGrid(const uint64_t *start,
                                           const uint64_t *count)
{
    vtkRectilinearGrid *grid = vtkRectilinearGrid::New();
    vtkFloatArray *coords[3] = {NULL,NULL,NULL};

    int dims[3] = {1,1,1};
    for (int i = 0; i<3; i++)
    {
        dims[i] = (int)count[i];

        coords[i] = vtkFloatArray::New();
        coords[i]->SetNumberOfTuples(dims[i]);
        float *data = (float *)coords[i]->GetVoidPointer(0);

        int x = (int)start[i];

        for (int j = 0; j < dims[i]; j++, x++)
            data[j] = (float)x;
    }
    grid->SetDimensions(dims);
    grid->SetXCoordinates(coords[0]);
    grid->SetYCoordinates(coords[1]);
    grid->SetZCoordinates(coords[2]);
    
    for (int i = 0; i<3; i++)
        coords[i]->Delete();

    debug1 <<"  create mesh: " << coords[0]->GetNumberOfTuples() << "x"
           <<coords[1]->GetNumberOfTuples() << "x"
           <<coords[2]->GetNumberOfTuples() << endl;

    return grid;
}
