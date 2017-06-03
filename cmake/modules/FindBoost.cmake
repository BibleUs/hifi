# 
#  FindBoost.cmake
# 
#  Try to find the Boost library.
#
#  You must provide a BOOST_ROOT_DIR which contains Lib and Include directories.
#
#  Once done this will define:
#  BOOST_FOUND - system found Boost
#  BOOST_INCLUDE_DIRS - the Boost include directory
#  BOOST_LIBRARIES - Link this to use Boost
#
#  Created by David Rowe on 6 Jul 2016.
#  Copyright 2016 High Fidelity, Inc.
#
#  Distributed under the Apache License, Version 2.0.
#  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
# 

if (NOT ANDROID)
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(BOOST DEFAULT_MSG BOOST_INCLUDE_DIRS BOOST_LIBRARIES)
endif()

mark_as_advanced(BOOST_INCLUDE_DIRS BOOST_LIBRARIES BOOST_SEARCH_DIRS)
