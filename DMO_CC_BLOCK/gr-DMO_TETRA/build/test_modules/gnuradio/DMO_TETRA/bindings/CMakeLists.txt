# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT DMO_TETRA_sources)
    message(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT DMO_TETRA_sources)

########################################################################
# Check for pygccxml
########################################################################
gr_python_check_module_raw("pygccxml" "import pygccxml" PYGCCXML_FOUND)

include(GrPybind)

########################################################################
# Python Bindings
########################################################################

list(APPEND DMO_TETRA_python_files
    DMO_burst_extract_python.cc
    mac_extract_python.cc python_bindings.cc)

gr_pybind_make_oot(DMO_TETRA ../../.. gr::DMO_TETRA "${DMO_TETRA_python_files}")

# copy bindings extension for use in QA test module
add_custom_command(
    TARGET DMO_TETRA_python
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:DMO_TETRA_python>
            ${PROJECT_BINARY_DIR}/test_modules/gnuradio/DMO_TETRA/)

install(
    TARGETS DMO_TETRA_python
    DESTINATION ${GR_PYTHON_DIR}/gnuradio/DMO_TETRA
    COMPONENT pythonapi)
