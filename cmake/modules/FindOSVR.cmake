# 
#  FindOSVR.cmake
# 
#  Try to find the OSVR library.
#
#  You must provide a OSVR_ROOT_DIR which contains Lib and Include directories.
#
#  Once done this will define:
#  OSVR_FOUND - system found OSVR
#  OSVR_INCLUDE_DIRS - the OSVR include directory
#  OSVR_LIBRARIES - Link this to use OSVR
#
#  Created by David Rowe on 6 Jul 2016.
#  Copyright 2016 High Fidelity, Inc.
#
#  Distributed under the Apache License, Version 2.0.
#  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
# 

include("${MACRO_DIR}/HifiLibrarySearchHints.cmake")
hifi_library_search_hints("osvr")

find_path(OSVR_INCLUDE_DIRS osvr/ClientKit/ClientKit.h PATH_SUFFIXES include include/osvr include/osvr/ClientKit HINTS ${OSVR_SEARCH_DIRS})

find_library(OSVR_CLIENTKIT_LIBRARY NAMES osvrClientKit PATH_SUFFIXES lib HINTS ${OSVR_SEARCH_DIRS})
find_library(OSVR_RENDERMANAGER_LIBRARY NAMES osvrRenderManager PATH_SUFFIXES lib HINTS ${OSVR_SEARCH_DIRS})
set(OSVR_LIBRARIES ${OSVR_CLIENTKIT_LIBRARY} ${OSVR_RENDERMANAGER_LIBRARY})

find_path(OSVR_DLL_PATH osvrClientKit.dll PATH_SUFFIXES bin HINTS ${OSVR_SEARCH_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OSVR DEFAULT_MSG OSVR_INCLUDE_DIRS OSVR_LIBRARIES OSVR_DLL_PATH)

add_paths_to_fixup_libs(${OSVR_DLL_PATH})

mark_as_advanced(OSVR_INCLUDE_DIRS OSVR_LIBRARIES OSVR_SEARCH_DIRS)
