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

if (NOT ANDROID)
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(OSVR DEFAULT_MSG OSVR_INCLUDE_DIRS OSVR_LIBRARIES)
endif()

mark_as_advanced(OSVR_INCLUDE_DIRS OSVR_LIBRARIES OSVR_SEARCH_DIRS)
