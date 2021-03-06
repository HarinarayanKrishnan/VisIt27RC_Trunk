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
#  Hank Childs, Thu Jul 22 09:55:03 PDT 2010
#  Add attributes for multi-window saves.
#
#  Kathleen Bonnell, Wed Apr 20 12:10:37 MST 2011
#  Allow visitfrontend to be built on windows.
#
#  Brad Whitlock, Tue Dec 14 16:25:32 PST 2010
#  Add PySelectionProperties, PySelectionList, PySelectionSummary,
#  PySelectionVariableSummary.
#
#  Kathleen Bonnell, Wed Apr 20 12:10:37 MST 2011
#  Allow visitfrontend to be built on windows.
#
#  Cyrus Harrison, Thu Sep 29 15:57:46 PDT 2011
#  Added PySide.
#
#  Cyrus Harrison, Tue Apr 24 23:12:25 MDT 2012
#  Added visit_utils
#
#  Kathleen Biagas, Fri May 4 14:05:27 PDT 2012
#  Don't change LIBRARY_OUTPUT_DIRECTORY for visitmodule if on windows. 
#
#  Brad Whitlock, Fri May 18 16:35:34 PST 2012
#  Change the version file that we use.
#
#****************************************************************************/

SET(COMMON_SOURCES
common/CallbackHandlers.C
common/CallbackManager.C
common/Logging.C
common/MethodDoc.C
common/PyImageObject.C
common/PyLegendAttributesObject.C
common/PyLineObject.C
common/PySILRestrictionBase.C
common/PySILRestriction.C
common/PyText2DObject.C
common/PyText3DObject.C
common/PyTimeSliderObject.C
common/ViewerRPCArguments.C
common/ViewerRPCCallbacks.C
common/visitmodule.C
)

SET(PY_SOURCES
common/PyMapNode.C
common/PyVariant.C
)

SET(METADATA_SOURCES
visitpy/PyavtArrayMetaData.C 
visitpy/PyavtBaseVarMetaData.C 
visitpy/PyavtCurveMetaData.C 
visitpy/PyavtDatabaseMetaData.C 
visitpy/PyavtDefaultPlotMetaData.C 
visitpy/PyavtLabelMetaData.C 
visitpy/PyavtMatSpeciesMetaData.C 
visitpy/PyavtMaterialMetaData.C 
visitpy/PyavtMeshMetaData.C 
visitpy/PyavtScalarMetaData.C 
visitpy/PyavtSimulationCommandSpecification.C 
visitpy/PyavtSimulationInformation.C 
visitpy/PyavtSpeciesMetaData.C 
visitpy/PyavtSubsetsMetaData.C 
visitpy/PyavtSymmetricTensorMetaData.C 
visitpy/PyavtTensorMetaData.C 
visitpy/PyavtVarMetaData.C 
visitpy/PyavtVectorMetaData.C
)

SET(GENERATED_PY_SOURCES
visitpy/PyAnimationAttributes.C 
visitpy/PyAnnotationAttributes.C 
visitpy/PyAxes2D.C 
visitpy/PyAxes3D.C 
visitpy/PyAxesArray.C 
visitpy/PyAxisAttributes.C 
visitpy/PyAxisLabels.C 
visitpy/PyAxisTickMarks.C 
visitpy/PyAxisTitles.C 
visitpy/PyColorAttribute.C 
visitpy/PyColorAttributeList.C 
visitpy/PyColorControlPoint.C 
visitpy/PyColorControlPointList.C 
visitpy/PyColorTableAttributes.C 
visitpy/PyConstructDataBinningAttributes.C 
visitpy/PyDatabaseCorrelation.C 
visitpy/PyDBOptionsAttributes.C 
visitpy/PyExportDBAttributes.C 
visitpy/PyExpression.C 
visitpy/PyExpressionList.C 
visitpy/PyFontAttributes.C 
visitpy/PyFileOpenOptions.C 
visitpy/PyGaussianControlPoint.C 
visitpy/PyGaussianControlPointList.C 
visitpy/PyGlobalAttributes.C 
visitpy/PyGlobalLineoutAttributes.C 
visitpy/PyInteractorAttributes.C 
visitpy/PyKeyframeAttributes.C 
visitpy/PyLaunchProfile.C 
visitpy/PyLightAttributes.C 
visitpy/PyMachineProfile.C 
visitpy/PyMaterialAttributes.C 
visitpy/PyMeshManagementAttributes.C 
visitpy/PyNameschemeAttributes.C 
visitpy/PyPickAttributes.C 
visitpy/PyPickVarInfo.C 
visitpy/PyPlot.C 
visitpy/PyPlotList.C 
visitpy/PyPrinterAttributes.C 
visitpy/PyProcessAttributes.C 
visitpy/PyQueryAttributes.C 
visitpy/PyQueryOverTimeAttributes.C 
visitpy/PyRenderingAttributes.C 
visitpy/PySaveWindowAttributes.C 
visitpy/PySaveSubWindowAttributes.C 
visitpy/PySaveSubWindowsAttributes.C 
visitpy/PySelectionProperties.C
visitpy/PySelectionSummary.C
visitpy/PySelectionVariableSummary.C
visitpy/PySelectionList.C
visitpy/PyView2DAttributes.C 
visitpy/PyView3DAttributes.C 
visitpy/PyViewAttributes.C 
visitpy/PyViewAxisArrayAttributes.C 
visitpy/PyViewerClientAttributes.C 
visitpy/PyViewerClientInformation.C 
visitpy/PyViewerClientInformationElement.C 
visitpy/PyViewCurveAttributes.C 
visitpy/PyViewerRPC.C 
visitpy/PyWindowInformation.C
)

SET(CLI_SOURCES
cli/cli.C
)
IF(VISIT_STATIC)
    SET(CLI_SOURCES ${CLI_SOURCES} cli/ScriptingStaticSymbolLocator.C)
ENDIF(VISIT_STATIC)

INCLUDE_DIRECTORIES(
${VISIT_COMMON_INCLUDES}
${VISIT_SOURCE_DIR}/visitpy/common
${VISIT_SOURCE_DIR}/visitpy/visitpy
${VISIT_SOURCE_DIR}/avt/DBAtts/MetaData
${VISIT_SOURCE_DIR}/avt/DBAtts/SIL
${VISIT_SOURCE_DIR}/viewer/proxy
${VISIT_SOURCE_DIR}/viewer/rpc
${PYTHON_INCLUDE_PATH}
)

IF (NOT WIN32)
    # This keeps comm's exceptions visible when using -fvisibility=hidden
    ADD_DEFINITIONS(-DCOMM_EXPORTS)
    # This keeps visitpy functions visible. On Windows, we don't need to do this
    # because the visitpy library that exports the symbols already gets
    # visitpy_EXPORTS defined for it.
    ADD_DEFINITIONS(-DVISITPY_EXPORTS)
ENDIF (NOT WIN32)

LINK_DIRECTORIES(${LIBRARY_OUTPUT_DIRECTORY})

# Add the visitpy library, which contains all of the Py extension classes.
ADD_LIBRARY(visitpy
${PY_SOURCES}
${METADATA_SOURCES}
${GENERATED_PY_SOURCES}
)
TARGET_LINK_LIBRARIES(visitpy
avtdbatts
viewerrpc
visitcommon
${PYTHON_LIBRARIES}
)

# Create the CLI
ADD_EXECUTABLE(cli
${COMMON_SOURCES}
${CLI_SOURCES}
${VISIT_CLI_RESOURCE_FILE}
)
TARGET_LINK_LIBRARIES(cli 
${cli_IOperator}
${cli_SOperator}
${cli_IPlot}
${cli_SPlot}
viewerproxy
viewerrpc
avtdbatts
visitcommon
visitpy
${PYTHON_LIBRARIES}
${CMAKE_THREAD_LIBS}
)
IF(NOT WIN32)
    TARGET_LINK_LIBRARIES(cli dl)
    IF(NOT APPLE)
        IF(NOT AIX)
            # Is this a Python companion libary? We need it for unresolved openpty/forkpty
            TARGET_LINK_LIBRARIES(cli util)
        ENDIF(NOT AIX)
    ENDIF(NOT APPLE)
ENDIF(NOT WIN32)

# If we're building statically then the cli can't be linked until the plugin
# sources are built
IF(VISIT_STATIC)
    ADD_DEPENDENCIES(cli
        ${cli_IOperator}
        ${cli_SOperator}
        ${cli_IPlot}
        ${cli_SPlot}
    )

    VISIT_INSTALL_TARGETS(cli)
ELSE(VISIT_STATIC)


    # Create the visitmodule
    PYTHON_ADD_MODULE(visitmodule
                      ${COMMON_SOURCES})
    TARGET_LINK_LIBRARIES(visitmodule
        viewerproxy
        viewerrpc
        avtdbatts
        visitcommon
        visitpy
        ${PYTHON_LIBRARIES}
    )

    IF(NOT WIN32)
        SET_TARGET_PROPERTIES(visitmodule PROPERTIES
                                          LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/site-packages/visit)
    ENDIF(NOT WIN32)


    IF(NOT APPLE)
        SET_TARGET_PROPERTIES(visitmodule PROPERTIES INSTALL_RPATH "$ORIGIN")
    ENDIF(NOT APPLE)


    VISIT_INSTALL_TARGETS(visitpy cli)
    VISIT_INSTALL_TARGETS_RELATIVE(site-packages/visit visitmodule)
ENDIF(VISIT_STATIC)

#
# For distutils setup of the pure python parts of the visit module
#
ADD_SUBDIRECTORY(visitmodule)
ADD_SUBDIRECTORY(visit_utils)


IF(VISIT_PYTHON_FILTERS)
    ADD_SUBDIRECTORY(pyavt)
    ADD_SUBDIRECTORY(mpicom)
ENDIF(VISIT_PYTHON_FILTERS)

ADD_SUBDIRECTORY(pyui)
