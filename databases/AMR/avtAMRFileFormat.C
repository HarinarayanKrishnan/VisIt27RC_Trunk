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
//                            avtAMRFileFormat.C                           //
// ************************************************************************* //
    
#include <avtAMRFileFormat.h>

#include <vtkCellType.h>

#include <vtkFloatArray.h>
#include <vtkRectilinearGrid.h>
//#include <vtkStructuredGrid.h>
#include <vtkUnstructuredGrid.h>

#include <avtDatabaseMetaData.h>

#include <DBOptionsAttributes.h>
#include <Expression.h>

#include <InvalidDBTypeException.h>
#include <InvalidVariableException.h>
#include <DebugStream.h>

#include <string>

using     std::string;

const string avtAMRFileFormat::amr_name     ="AMR";
const string avtAMRFileFormat::intf_name    ="Interface";
const string avtAMRFileFormat::invalid_name ="Invalid";

string avtAMRFileFormat::
composeName( const string& m, const string& v, const char app )
{
  return m+app+v;
}


void avtAMRFileFormat::
decomposeName( const string& s, string& m, string& v )
{
  if( s.compare( 0, amr_name.size(), amr_name )==0 )
    m = amr_name;
  else if( s.compare( 0, intf_name.size(), intf_name )==0 )
    m = intf_name;
  else
    m = invalid_name;

  if( m!=invalid_name ) 
    v = s.substr( m.size()+1 );
  else
    v = s;
}



int avtAMRFileFormat::
GetCycle()
{
  return reader_->GetNumCycles();
}


double avtAMRFileFormat::
GetTime()
{
  return reader_->GetSimuTime();
}



// ****************************************************************************
//  Method: avtAMRFileFormat constructor
//
//  Programmer: hpan -- generated by xml2avt
//  Creation:   Mon Apr 26 10:26:32 PDT 2010
//
// ****************************************************************************

avtAMRFileFormat::avtAMRFileFormat(const char *filename)
    : avtSTMDFileFormat(&filename, 1)
{
  debug2 << "Creating new avtAMRFileFormat ... filename=" << filename << "\n.";

#ifndef USING_AGG
  reader_ = new AMRreader;
#else
  reader_ = new AMRreaderAgg;
#endif
  int ierr = reader_->getInfo(filename);
  if( ierr!=0 ) {
    EXCEPTION1(InvalidDBTypeException,
           "The file could not be opened");
  }
}


avtAMRFileFormat::~avtAMRFileFormat()
{
  debug2 << "closed avtAMRFileFormat\n.";
  reader_->freedata();
  delete reader_;
}




// ****************************************************************************
//  Method: avtAMRFileFormat::FreeUpResources
//
//  Purpose:
//      When VisIt is done focusing on a particular timestep, it asks that
//      timestep to free up any resources (memory, file descriptors) that
//      it has associated with it.  This method is the mechanism for doing
//      that.
//
//  Programmer: hpan -- generated by xml2avt
//  Creation:   Mon Apr 26 10:26:32 PDT 2010
//
// ****************************************************************************

void
avtAMRFileFormat::FreeUpResources(void)
{
  debug2 << "Now free resource.\n";
  reader_->freedata();
  debug2 << "Now done free resource.\n";
}


// ****************************************************************************
//  Method: avtAMRFileFormat::PopulateDatabaseMetaData
//
//  Purpose:
//      This database meta-data object is like a table of contents for the
//      file.  By populating it, you are telling the rest of VisIt what
//      information it can request from you.
//
//  Programmer: hpan -- generated by xml2avt
//  Creation:   Mon Apr 26 10:26:32 PDT 2010
//
// ****************************************************************************

void
avtAMRFileFormat::PopulateDatabaseMetaData(avtDatabaseMetaData *md)
{
  // meshes
  int nblks = reader_->GetNumberOfBlocks();
  if( nblks>0 ) {
    avtMeshMetaData *mmd = new avtMeshMetaData;
    mmd->name = amr_name;
    mmd->spatialDimension = 3;
    mmd->topologicalDimension = 3;
    mmd->meshType = AVT_RECTILINEAR_MESH;
    mmd->numBlocks = nblks;
    md->Add(mmd);
  }

  int np,ne;
  reader_->GetInterfaceSizes( &np, &ne );
  if( np>0 && ne>0 ) {
    avtMeshMetaData *mmd = new avtMeshMetaData;
    mmd->name = intf_name;
    mmd->spatialDimension = 3;
    mmd->topologicalDimension = 2;
    mmd->meshType = AVT_UNSTRUCTURED_MESH;
    mmd->numBlocks = 1;
    md->Add(mmd);
  }

  // AMR variable
  if( nblks>0 ) {
    // scalar
    avtScalarMetaData *smd;

    smd = new avtScalarMetaData;
    smd->name = composeName( amr_name, "density");
    smd->meshName = amr_name;
    smd->centering = AVT_ZONECENT;
    smd->hasUnits = false;
    md->Add(smd);

    smd = new avtScalarMetaData;
    smd->name = composeName( amr_name, "u velocity");
    smd->meshName = amr_name;
    smd->centering = AVT_ZONECENT;
    smd->hasUnits = false;
    md->Add(smd);

    smd = new avtScalarMetaData;
    smd->name = composeName( amr_name, "v velocity");
    smd->meshName = amr_name;
    smd->centering = AVT_ZONECENT;
    smd->hasUnits = false;
    md->Add(smd);

    smd = new avtScalarMetaData;
    smd->name = composeName( amr_name, "w velocity");
    smd->meshName = amr_name;
    smd->centering = AVT_ZONECENT;
    smd->hasUnits = false;
    md->Add(smd);

    smd = new avtScalarMetaData;
    smd->name = composeName( amr_name, "pressure");
    smd->meshName = amr_name;
    smd->centering = AVT_ZONECENT;
    smd->hasUnits = false;
    md->Add(smd);

    smd = new avtScalarMetaData;
    smd->name = composeName( amr_name, "temperature");
    smd->meshName = amr_name;
    smd->centering = AVT_ZONECENT;
    smd->hasUnits = false;
    md->Add(smd);

    smd = new avtScalarMetaData;
    smd->name = composeName( amr_name, "sound speed");
    smd->meshName = amr_name;
    smd->centering = AVT_ZONECENT;
    smd->hasUnits = false;
    md->Add(smd);

    smd = new avtScalarMetaData;
    smd->name = composeName( amr_name, "internal energy");
    smd->meshName = amr_name;
    smd->centering = AVT_ZONECENT;
    smd->hasUnits = false;
    md->Add(smd);

    smd = new avtScalarMetaData;
    smd->name = composeName( amr_name, "kinetic energy");
    smd->meshName = amr_name;
    smd->centering = AVT_ZONECENT;
    smd->hasUnits = false;
    md->Add(smd);

    // vector
    avtVectorMetaData *vmd;
    vmd = new avtVectorMetaData;
    vmd->name = composeName( amr_name, "velocity");
    vmd->meshName = amr_name;
    vmd->centering = AVT_ZONECENT;
    vmd->hasUnits = false;
    // ??? is it necessary
    //vmd->varDim = 3;
    md->Add(vmd);
  }



  // interface 
  if( np>0 && ne>0 ) {
    // scalar
    avtScalarMetaData *smd;

    smd = new avtScalarMetaData;
    smd->name = composeName( intf_name, "u velocity");
    smd->meshName = intf_name;
    smd->centering = AVT_NODECENT;
    smd->hasUnits = false;
    md->Add(smd);

    smd = new avtScalarMetaData;
    smd->name = composeName( intf_name, "v velocity");
    smd->meshName = intf_name;
    smd->centering = AVT_NODECENT;
    smd->hasUnits = false;
    md->Add(smd);

    smd = new avtScalarMetaData;
    smd->name = composeName( intf_name, "w velocity");
    smd->meshName = intf_name;
    smd->centering = AVT_NODECENT;
    smd->hasUnits = false;
    md->Add(smd);

    smd = new avtScalarMetaData;
    smd->name = composeName( intf_name, "pressure");
    smd->meshName = intf_name;
    smd->centering = AVT_ZONECENT;
    smd->hasUnits = false;
    md->Add(smd);

    // vector
    avtVectorMetaData *vmd;
    vmd = new avtVectorMetaData;
    vmd->name = composeName( intf_name, "velocity");
    vmd->meshName = intf_name;
    vmd->centering = AVT_NODECENT;
    vmd->hasUnits = false;
    // ??? is it necessary
    //vmd->varDim = 3;
    md->Add(vmd);
  }
    //
    // Here's the way to add expressions:
    //Expression momentum_expr;
    //momentum_expr.SetName("momentum");
    //momentum_expr.SetDefinition("{u, v}");
    //momentum_expr.SetType(Expression::VectorMeshVar);
    //md->AddExpression(&momentum_expr);
    //Expression KineticEnergy_expr;
    //KineticEnergy_expr.SetName("KineticEnergy");
    //KineticEnergy_expr.SetDefinition("0.5*(momentum*momentum)/(rho*rho)");
    //KineticEnergy_expr.SetType(Expression::ScalarMeshVar);
    //md->AddExpression(&KineticEnergy_expr);
    //
}


// ****************************************************************************
//  Method: avtAMRFileFormat::GetMesh
//
//  Purpose:
//      Gets the mesh associated with this file.  The mesh is returned as a
//      derived type of vtkDataSet (ie vtkRectilinearGrid, vtkStructuredGrid,
//      vtkUnstructuredGrid, etc).
//
//  Arguments:
//      domain      The index of the domain.  If there are NDomains, this
//                  value is guaranteed to be between 0 and NDomains-1,
//                  regardless of block origin.
//      meshname    The name of the mesh of interest.  This can be ignored if
//                  there is only one mesh.
//
//  Programmer: hpan -- generated by xml2avt
//  Creation:   Mon Apr 26 10:26:32 PDT 2010
//
// ****************************************************************************

vtkDataSet *
avtAMRFileFormat::GetMesh(int domain, const char *meshname)
{
  // amr
  if( amr_name.compare(meshname)==0 ) {
    int blkdim[3];
#ifndef USING_AGG
    reader_->GetBlockDimensions( blkdim );
#else
    reader_->GetBlockDimensions( domain, blkdim );
#endif
    // count the # of points
    ++blkdim[0];    ++blkdim[1];    ++blkdim[2];

    float xs[3],dx[3];
    int ierr = reader_->GetBlockMesh( domain, xs, dx );
    if( ierr!=0 ) {
      EXCEPTION1( InvalidVariableException, 
          "Failed to get given block mesh.\n" );
    }

    vtkFloatArray *crd[3];
    for( int as=0; as<3; as++ ) {
      crd[as] = vtkFloatArray::New();
      crd[as]->SetNumberOfTuples( blkdim[as] );
      float* vv = (float*)crd[as]->GetVoidPointer(0);
      for( int i=0; i<blkdim[as]; i++ )
    vv[i] = xs[as] + (float)i*dx[as];
    }

    vtkRectilinearGrid *grd = vtkRectilinearGrid::New();
    grd->SetDimensions( blkdim );
    grd->SetXCoordinates( crd[0] );
    grd->SetYCoordinates( crd[1] );
    grd->SetZCoordinates( crd[2] );

    for( int as=0; as<3; as++ ) 
      crd[as]->Delete();

    return grd;
  }

  // interface
  if( intf_name.compare(meshname)==0 ) {
    int np,ne;
    reader_->GetInterfaceSizes( &np, &ne );

    vtkPoints* crd = vtkPoints::New();
    crd->SetNumberOfPoints(np);
    float *pts = (float *) crd->GetVoidPointer(0);
    int ierr = reader_->GetInterfaceVariable( AMRreader::i_coor, pts );
    if( ierr!=0 ) {
      EXCEPTION1( InvalidVariableException, 
          "Failed to read in interface coordinates.\n" );
    }

    int *elmt = new int[ 4*ne ];
    ierr = reader_->GetInterfaceVariable( AMRreader::i_segt, elmt );
    if( ierr!=0 ) {
      string msg="Failed to read in interface elements of "; msg+=meshname;
      EXCEPTION1( InvalidVariableException, msg );
    }

    vtkUnstructuredGrid *ugrid = vtkUnstructuredGrid::New(); 
    ugrid->SetPoints( crd );
    ugrid->Allocate( ne );

    vtkIdType verts[4];
    int* cnt=elmt;
    for( int i=0; i<ne; i++ ) {
      verts[0] = *cnt++;  verts[0]-=1;
      verts[1] = *cnt++;  verts[1]-=1;
      verts[2] = *cnt++;  verts[2]-=1;
      verts[3] = *cnt++;  verts[3]-=1;
      ugrid->InsertNextCell( VTK_QUAD, 4, verts );
    }

    delete [] elmt;

    return ugrid;
  }

  string msg="Unrecongnized mesh name: ";msg+=meshname;
  EXCEPTION1( InvalidVariableException, msg );
}


// ****************************************************************************
//  Method: avtAMRFileFormat::GetVar
//
//  Purpose:
//      Gets a scalar variable associated with this file.  Although VTK has
//      support for many different types, the best bet is vtkFloatArray, since
//      that is supported everywhere through VisIt.
//
//  Arguments:
//      domain     The index of the domain.  If there are NDomains, this
//                 value is guaranteed to be between 0 and NDomains-1,
//                 regardless of block origin.
//      varname    The name of the variable requested.
//
//  Programmer: hpan -- generated by xml2avt
//  Creation:   Mon Apr 26 10:26:32 PDT 2010
//
// ****************************************************************************

vtkDataArray *
avtAMRFileFormat::GetVar(int domain, const char *name)
{
  string mesh,varname;
  decomposeName( name, mesh, varname );
  debug2 << "geting variable " << varname << " on mesh " << mesh << "...\n";

  // amr 
  if( mesh.compare(amr_name)==0 ) {
    int vid;
    if( varname.compare("density")==0 ) 
      vid=AMRreader::v_dens;
    else if( varname.compare("u velocity")==0 )
      vid=AMRreader::v_uvel;
    else if( varname.compare("v velocity")==0 ) 
      vid=AMRreader::v_vvel;
    else if( varname.compare("w velocity")==0 ) 
      vid=AMRreader::v_wvel;
    else if( varname.compare("pressure")==0 ) 
      vid=AMRreader::v_pres;
    else if( varname.compare("temperature")==0 ) 
      vid=AMRreader::v_temp;
    else if( varname.compare("sound speed")==0 ) 
      vid=AMRreader::v_sndv;
    else if( varname.compare("internal energy")==0 ) 
      vid=AMRreader::v_eint;
    else if( varname.compare( "kinetic energy")==0 ) 
      vid=AMRreader::v_eknt;
    else {
      string msg = "Unknown scalar variable "; msg+=name; msg+=" is specified.";
      EXCEPTION1( InvalidVariableException, msg );
    }

#ifndef USING_AGG
    int sz = reader_->GetBlockSize();
#else
    int sz = reader_->GetBlockSize( domain );
#endif
    vtkFloatArray* var = vtkFloatArray::New();
    var->SetNumberOfTuples( sz );
    float *val = (float *)var->GetVoidPointer(0);

    int err = reader_->GetBlockVariable( domain, vid, val );
    if( err!=0 ) {
      string msg = "Failed to read in scalar variable "; msg+=name;
      EXCEPTION1( InvalidVariableException, msg );
    }
    return var;
  }

  // interface
  if( mesh.compare(intf_name)==0 ) {
    int np,ne;
    reader_->GetInterfaceSizes( &np, &ne );

    int vid,cmp,sz;
    if( varname.compare("u velocity")==0 ) {
      vid=AMRreader::i_velo;  cmp=0; sz=np; }
    else if( varname.compare("v velocity")==0 ) {
      vid=AMRreader::i_velo;  cmp=1; sz=np; }
    else if( varname.compare("w velocity")==0 ) {
      vid=AMRreader::i_velo;  cmp=2; sz=np; }
    else if( varname.compare("pressure")==0 ) {
      vid=AMRreader::i_pres;  sz=ne; }
    else {
      string msg = "Unknown scalar variable "; msg+=name; msg+=" is specified.";
      EXCEPTION1( InvalidVariableException, msg );
    }

    vtkFloatArray* var = vtkFloatArray::New();
    var->SetNumberOfTuples( sz );
    float *val = (float *)var->GetVoidPointer(0);

    if( vid==AMRreader::i_velo ) {
      float* buf = new float[sz*3];
      int err = reader_->GetInterfaceVariable( vid, buf );
      if( err!=0 ) {
    string msg = "Failed to read in scalar variable "; msg+=name;
    EXCEPTION1( InvalidVariableException, msg );
      }

      for( int i=0; i<np; i++ )
    val[i] = buf[3*i+cmp];

      delete [] buf;
    }
    else {
      int err = reader_->GetInterfaceVariable( vid, val );
      if( err!=0 ) {
    string msg = "Failed to read in scalar variable "; msg+=name;
    EXCEPTION1( InvalidVariableException, msg );
      }
    }
    return var;
  }

  string msg="Unrecongnized mesh for scalar variable "; msg+=name;
  EXCEPTION1( InvalidVariableException, msg );
}


// ****************************************************************************
//  Method: avtAMRFileFormat::GetVectorVar
//
//  Purpose:
//      Gets a vector variable associated with this file.  Although VTK has
//      support for many different types, the best bet is vtkFloatArray, since
//      that is supported everywhere through VisIt.
//
//  Arguments:
//      domain     The index of the domain.  If there are NDomains, this
//                 value is guaranteed to be between 0 and NDomains-1,
//                 regardless of block origin.
//      varname    The name of the variable requested.
//
//  Programmer: hpan -- generated by xml2avt
//  Creation:   Mon Apr 26 10:26:32 PDT 2010
//
// ****************************************************************************

vtkDataArray *
avtAMRFileFormat::GetVectorVar(int domain, const char *name)
{
  string mesh,varname;
  decomposeName( name, mesh, varname );
  debug2 << "geting variable " << varname << " on mesh " << mesh << "...\n";

  if( mesh.compare(amr_name)==0 ) {
    int vid;
    if( varname.compare("velocity")==0 ) 
      vid=AMRreader::v_velo;
    else {
      string msg = "Unknown vector variable "; msg+=name; msg+=" is specified.";
      EXCEPTION1( InvalidVariableException, msg );
    }

#ifndef USING_AGG
    int sz = reader_->GetBlockSize();
#else
    int sz = reader_->GetBlockSize( domain );
#endif
    vtkFloatArray* var = vtkFloatArray::New();
    var->SetNumberOfComponents(3);
    var->SetNumberOfTuples( sz );
    float *val = (float *)var->GetVoidPointer(0);

    int err = reader_->GetBlockVariable( domain, vid, val );
    if( err!=0 ) {
      string msg="Failed to read in vector variable "; msg+=name;
      EXCEPTION1( InvalidVariableException, msg );
    }
    return var;
  }


  if( mesh.compare(intf_name)==0 ) {
    int np,ne;
    reader_->GetInterfaceSizes( &np, &ne );

    int vid;
    if( varname.compare("velocity")==0 ) 
      vid=AMRreader::i_velo;
    else {
      string msg = "Unknown vector variable "; msg+=name; msg+=" is specified.";
      EXCEPTION1( InvalidVariableException, msg );
    }

    vtkFloatArray* var = vtkFloatArray::New();
    var->SetNumberOfComponents(3);
    var->SetNumberOfTuples( np );
    float *val = (float *)var->GetVoidPointer(0);

    int err = reader_->GetInterfaceVariable( vid, val );
    if( err!=0 ) {
      string msg="Failed to read in vector variable "; msg+=name;
      EXCEPTION1( InvalidVariableException, msg );
    }
    return var;
  }

  string msg="Unrecongnized mesh for vector variable "; msg+=name;
  EXCEPTION1( InvalidVariableException, msg );
}
