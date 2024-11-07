find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_DMO_TETRA gnuradio-DMO_TETRA)

FIND_PATH(
    GR_DMO_TETRA_INCLUDE_DIRS
    NAMES gnuradio/DMO_TETRA/api.h
    HINTS $ENV{DMO_TETRA_DIR}/include
        ${PC_DMO_TETRA_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_DMO_TETRA_LIBRARIES
    NAMES gnuradio-DMO_TETRA
    HINTS $ENV{DMO_TETRA_DIR}/lib
        ${PC_DMO_TETRA_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-DMO_TETRATarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_DMO_TETRA DEFAULT_MSG GR_DMO_TETRA_LIBRARIES GR_DMO_TETRA_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_DMO_TETRA_LIBRARIES GR_DMO_TETRA_INCLUDE_DIRS)
