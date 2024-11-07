#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "gnuradio::gnuradio-DMO_TETRA" for configuration "Debug"
set_property(TARGET gnuradio::gnuradio-DMO_TETRA APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(gnuradio::gnuradio-DMO_TETRA PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libgnuradio-DMO_TETRA.so.1.0.0.0"
  IMPORTED_SONAME_DEBUG "libgnuradio-DMO_TETRA.so.1.0.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS gnuradio::gnuradio-DMO_TETRA )
list(APPEND _IMPORT_CHECK_FILES_FOR_gnuradio::gnuradio-DMO_TETRA "${_IMPORT_PREFIX}/lib/libgnuradio-DMO_TETRA.so.1.0.0.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
