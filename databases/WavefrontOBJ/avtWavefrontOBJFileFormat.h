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
//                         avtWavefrontOBJFileFormat.h                       //
// ************************************************************************* //

#ifndef AVT_WAVEFRONT_OBJ_FILE_FORMAT_H
#define AVT_WAVEFRONt_OBJ_FILE_FORMAT_H

#include <avtSTSDFileFormat.h>


// ****************************************************************************
//  Class: avtWavefrontOBJFileFormat
//
//  Purpose:
//      Handles files of the .obj file format.
//
//  Programmer: Hank Childs
//  Creation:   February 14, 2002
//
//  Modifications:
//
//    Kathleen Bonnell, Tue Mar 19 08:27:40 PST 2002
//    vtkScalars has been deprecated in VTK 4.0, use vtkDataArray instead.
//
// ****************************************************************************

class avtWavefrontOBJFileFormat : public avtSTSDFileFormat
{
  public:
                          avtWavefrontOBJFileFormat(const char *);
    virtual              ~avtWavefrontOBJFileFormat();

    virtual vtkDataSet   *GetMesh(const char *);
    virtual vtkDataArray *GetVar(const char *);

    virtual const char   *GetType(void)  { return "WavefrontOBJ File Format"; };

    virtual void          PopulateDatabaseMetaData(avtDatabaseMetaData *);

    virtual void          FreeUpResources(void);

  protected:
    vtkDataSet           *dataset;
    bool                  readInDataset;
    int                   nVars;

    static const char    *MESHNAME;
    static const char    *VARNAME1;
    static const char    *VARNAME2;

    void                  ReadInDataset(void);
};


#endif


