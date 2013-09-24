#*****************************************************************************
#
# Copyright (c) 2000 - 2013, Lawrence Livermore National Security, LLC
# Produced at the Lawrence Livermore National Laboratory
# LLNL-CODE-442911
# All rights reserved.
#
# This file is  part of VisIt. For  details, see https://visit.llnl.gov/.  The
# full copyright notice is contained in the file COPYRIGHT located at the root
# of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
#
# Redistribution  and  use  in  source  and  binary  forms,  with  or  without
# modification, are permitted provided that the following conditions are met:
#
#  - Redistributions of  source code must  retain the above  copyright notice,
#    this list of conditions and the disclaimer below.
#  - Redistributions in binary form must reproduce the above copyright notice,
#    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
#    documentation and/or other materials provided with the distribution.
#  - Neither the name of  the LLNS/LLNL nor the names of  its contributors may
#    be used to endorse or promote products derived from this software without
#    specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
# ARE  DISCLAIMED. IN  NO EVENT  SHALL LAWRENCE  LIVERMORE NATIONAL  SECURITY,
# LLC, THE  U.S.  DEPARTMENT OF  ENERGY  OR  CONTRIBUTORS BE  LIABLE  FOR  ANY
# DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,  OR   CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
# SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
# CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
# LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
# OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
# DAMAGE.
#
# Modifications:
#
#****************************************************************************/

SET(ENGINELIB_SOURCES
EngineSubjectProxy.C
)

# If the engine is static then we need some static symbol lookup functions
#IF(VISIT_STATIC)
#    SET(ENGINE_STATIC_SOURCES EngineStaticSymbolLocator.C)
#ENDIF(VISIT_STATIC)

#IF (NOT WIN32)
    # This keeps comm's exceptions visible when using -fvisibility=hidden
#    ADD_DEFINITIONS(-DCOMM_EXPORTS)
#ENDIF (NOT WIN32)

INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}
${VISIT_COMMON_INCLUDES}
${VISIT_SOURCE_DIR}/engine/main
${VISIT_SOURCE_DIR}/engine/rpc
${VISIT_SOURCE_DIR}/engine/proxy
${VISIT_SOURCE_DIR}/avt/DBAtts/MetaData
${VISIT_SOURCE_DIR}/avt/DBAtts/SIL
${VISIT_SOURCE_DIR}/avt/Database/Database
${VISIT_SOURCE_DIR}/avt/Math
${VISIT_SOURCE_DIR}/avt/Pipeline/Data
${VISIT_SOURCE_DIR}/avt/Pipeline/Pipeline
${VISIT_SOURCE_DIR}/avt/Pipeline/Sources
${VISIT_SOURCE_DIR}/avt/Pipeline/Sinks
${VISIT_SOURCE_DIR}/avt/VisWindow/VisWindow
${VISIT_SOURCE_DIR}/visit_vtk/lightweight
)

LINK_DIRECTORIES(
${LIBRARY_OUTPUT_DIRECTORY}
${MESA_LIBRARY_DIR}
${VTK_LIBRARY_DIRS}
${GLEW_LIBRARY_DIR}
${ALL_THIRDPARTY_IO_LIBRARY_DIR})

add_definitions(-fvisibility=default)
#*************************** The engine library ******************************
ADD_LIBRARY(enginesubjectproxy ${ENGINELIB_SOURCES})
#IF (WIN32)
#    SET_TARGET_PROPERTIES(enginesubject PROPERTIES OUTPUT_NAME enginesubjectlib)
#ENDIF (WIN32)

TARGET_LINK_LIBRARIES(enginesubjectproxy
visitcommon
engine_ser
enginerpc
engineproxy
avtview
avtwriter_ser
avtplotter_ser
avtquery_ser
avtviswindow_ser
avtpipeline_ser
)

VISIT_INSTALL_TARGETS(enginesubjectproxy)
