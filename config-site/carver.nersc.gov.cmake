#/global/common/carver/usg/cmake/2.8.10.1/bin/cmake
##
## /global/common/shared/graphics/installs/visit_svn/trunk/src/svn_bin/build_visit generated host.cmake
## created: Wed Jan 15 18:56:09 PST 2014
## system: Linux cvrsvc01 2.6.18-238.12.1.el5.nersc #1 SMP Mon Jul 11 16:15:58 PDT 2011 x86_64 x86_64 x86_64 GNU/Linux
## by: ghweber

##
## Setup VISITHOME & VISITARCH variables.
##
SET(VISITHOME /usr/common/graphics/installs/visit_3rdparty)
SET(VISITARCH linux-x86_64_gcc-4.1)

VISIT_OPTION_DEFAULT(VISIT_INSTALL_PROFILES_TO_HOSTS "nersc" TYPE STRING)
VISIT_OPTION_DEFAULT(CMAKE_INSTALL_PREFIX /usr/common/graphics/visit)
VISIT_OPTION_DEFAULT(VISIT_INSTALL_THIRD_PARTY ON)
#VISIT_OPTION_DEFAULT(VISIT_VERBOSE_MAKEFILE TRUE)

##
## Compiler flags.
##
VISIT_OPTION_DEFAULT(VISIT_C_COMPILER gcc TYPE FILEPATH)
VISIT_OPTION_DEFAULT(VISIT_CXX_COMPILER g++ TYPE FILEPATH)
VISIT_OPTION_DEFAULT(VISIT_C_FLAGS "-m64 -fPIC -fvisibility=hidden" TYPE STRING)
VISIT_OPTION_DEFAULT(VISIT_CXX_FLAGS "-m64 -fPIC -fvisibility=hidden" TYPE STRING)

##
## Parallel Options
##
VISIT_OPTION_DEFAULT(VISIT_MPI_C_FLAGS "-DOMPI_SKIP_MPICXX -I/usr/common/usg/openmpi/1.4.5/gcc/include")
VISIT_OPTION_DEFAULT(VISIT_MPI_CXX_FLAGS "-DOMPI_SKIP_MPICXX -I/usr/common/usg/openmpi/1.4.5/gcc/include")
VISIT_OPTION_DEFAULT(VISIT_PARALLEL_LINKER_FLAGS "-Wl,-undefined,dynamic_lookup")
VISIT_OPTION_DEFAULT(VISIT_MPI_LIBS "/usr/common/usg/openmpi/1.4.5/gcc/lib/libmpi.so")
VISIT_OPTION_DEFAULT(VISIT_PARALLEL ON TYPE BOOL)
VISIT_OPTION_DEFAULT(VISIT_PARALLEL_RPATH "/usr/common/usg/openmpi/1.4.5/gnu/lib")

##
## VisIt Thread Option
##
VISIT_OPTION_DEFAULT(VISIT_THREAD OFF TYPE BOOL)

##############################################################
##
## Database reader plugin support libraries
##
## The HDF4, HDF5 and NetCDF libraries must be first so that
## their libdeps are defined for any plugins that need them.
##
## For libraries with LIBDEP settings, order matters.
## Libraries with LIBDEP settings that depend on other
## Library's LIBDEP settings must come after them.
##############################################################

##
## R
##
VISIT_OPTION_DEFAULT(VISIT_R_DIR ${VISITHOME}/R/2.13.2/${VISITARCH}/lib/R)

##
## Mesa
##
VISIT_OPTION_DEFAULT(VISIT_MESA_DIR ${VISITHOME}/mesa/7.10.2/${VISITARCH})

##
## Python
##
VISIT_OPTION_DEFAULT(VISIT_PYTHON_DIR ${VISITHOME}/python/2.7.5/${VISITARCH})

##
## Qt
##
VISIT_OPTION_DEFAULT(VISIT_QT_BIN ${VISITHOME}/qt/4.8.3/${VISITARCH}/bin)

##
## VTK
##
VISIT_OPTION_DEFAULT(VISIT_VTK_DIR ${VISITHOME}/vtk-r/6.0.0/${VISITARCH})

##
## SZIP
##
VISIT_OPTION_DEFAULT(VISIT_SZIP_DIR ${VISITHOME}/szip/2.1/${VISITARCH})

##
## HDF5
##
VISIT_OPTION_DEFAULT(VISIT_HDF5_DIR ${VISITHOME}/hdf5/1.8.7/${VISITARCH})
VISIT_OPTION_DEFAULT(VISIT_HDF5_LIBDEP ${VISITHOME}/szip/2.1/${VISITARCH}/lib sz /usr/lib z TYPE STRING)

##
## FastBit
##
VISIT_OPTION_DEFAULT(VISIT_FASTBIT_DIR ${VISITHOME}/fastbit/1.2.0/${VISITARCH})

##
## HDF4
##
VISIT_OPTION_DEFAULT(VISIT_HDF4_DIR ${VISITHOME}/hdf4/4.2.5/${VISITARCH})
VISIT_OPTION_DEFAULT(VISIT_HDF4_LIBDEP ${VISITHOME}/szip/2.1/${VISITARCH}/lib sz ${VISITHOME}/vtk/5.8.0/${VISITARCH}/lib vtkjpeg TYPE STRING)

##
## NetCDF
##
VISIT_OPTION_DEFAULT(VISIT_NETCDF_DIR ${VISITHOME}/netcdf/4.1.1/${VISITARCH})
VISIT_OPTION_DEFAULT(VISIT_NETCDF_LIBDEP HDF5_LIBRARY_DIR hdf5_hl HDF5_LIBRARY_DIR hdf5 ${VISIT_HDF5_LIBDEP} TYPE STRING)

##
## CGNS
##
VISIT_OPTION_DEFAULT(VISIT_CGNS_DIR ${VISITHOME}/cgns/3.0.8/${VISITARCH})
VISIT_OPTION_DEFAULT(VISIT_CGNS_LIBDEP HDF5_LIBRARY_DIR hdf5 ${VISIT_HDF5_LIBDEP} TYPE STRING)

##
## GDAL
##
VISIT_OPTION_DEFAULT(VISIT_GDAL_DIR ${VISITHOME}/gdal/1.10.0/${VISITARCH})

##
## Boxlib
##
VISIT_OPTION_DEFAULT(VISIT_BOXLIB_DIR ${VISITHOME}/boxlib/1.0.7/${VISITARCH})

##
## CCMIO
##
VISIT_OPTION_DEFAULT(VISIT_CCMIO_DIR ${VISITHOME}/ccmio/2.6.1/${VISITARCH})

##
## CFITSIO
##
VISIT_OPTION_DEFAULT(VISIT_CFITSIO_DIR ${VISITHOME}/cfitsio/3006/${VISITARCH})

##
## H5Part
##
VISIT_OPTION_DEFAULT(VISIT_H5PART_DIR ${VISITHOME}/h5part/1.6.6/${VISITARCH})
VISIT_OPTION_DEFAULT(VISIT_H5PART_LIBDEP HDF5_LIBRARY_DIR hdf5 ${VISIT_HDF5_LIBDEP} TYPE STRING)

##
## Ice-T
##
VISIT_OPTION_DEFAULT(VISIT_ICET_DIR ${VISITHOME}/icet/1.0.0/${VISITARCH})

##
## ITAPS
##
## MOAB implementation
ITAPS_INCLUDE_DIRECTORIES(MOAB ${VISITHOME}/itaps/1.4/MOAB/4.6.0/${VISITARCH}/include)
ITAPS_FILE_PATTERNS(MOAB *.cub)
ITAPS_LINK_LIBRARIES(MOAB iMesh MOAB hdf5_hl hdf5 sz z  )
ITAPS_LINK_DIRECTORIES(MOAB  ${VISITHOME}/itaps/1.4/MOAB/4.6.0/${VISITARCH}/lib  ${VISITHOME}/hdf5/1.8.7/${VISITARCH}/lib  ${VISITHOME}/szip/2.1/${VISITARCH}/lib  )
## FMDB implementation
ITAPS_INCLUDE_DIRECTORIES(FMDB ${VISITHOME}/itaps/1.4/FMDB/1.4.0/${VISITARCH}/include)
ITAPS_FILE_PATTERNS(FMDB *.sms)
ITAPS_LINK_LIBRARIES(FMDB FMDB SCORECModel SCORECUtil )
ITAPS_LINK_DIRECTORIES(FMDB ${VISITHOME}/itaps/1.4/FMDB/1.4.0/${VISITARCH}/lib)
## GRUMMP implementation
ITAPS_INCLUDE_DIRECTORIES(GRUMMP ${VISITHOME}/itaps/1.4/GRUMMP/0.6.5/${VISITARCH}/include)
ITAPS_FILE_PATTERNS(GRUMMP *.bdry *.smesh *.vmesh)
ITAPS_LINK_LIBRARIES(GRUMMP iMesh_GRUMMP GR_3D GR_surf GR_2D GR_geom GR_base SUMAAlog_lite OptMS cgm dl)
ITAPS_LINK_DIRECTORIES(GRUMMP ${VISITHOME}/itaps/1.4/GRUMMP/0.6.5/${VISITARCH}/lib)

##
## PySide
##
VISIT_OPTION_DEFAULT(VISIT_PYSIDE_DIR ${VISITHOME}/pyside/1.1.1/${VISITARCH}/)

##
## Silo
##
VISIT_OPTION_DEFAULT(VISIT_SILO_DIR ${VISITHOME}/silo/4.9.1/${VISITARCH})
VISIT_OPTION_DEFAULT(VISIT_SILO_LIBDEP HDF5_LIBRARY_DIR hdf5 ${VISIT_HDF5_LIBDEP} TYPE STRING)

##
## AdvIO
##
VISIT_OPTION_DEFAULT(VISIT_ADVIO_DIR ${VISITHOME}/AdvIO/1.2/${VISITARCH}/)

##
## Xdmf
##
VISIT_OPTION_DEFAULT(VISIT_XDMF_DIR ${VISITHOME}/Xdmf/2.1.1/${VISITARCH})
VISIT_OPTION_DEFAULT(VISIT_XDMF_LIBDEP HDF5_LIBRARY_DIR hdf5   VTK_LIBRARY_DIRS vtklibxml2-6.0  TYPE STRING)
