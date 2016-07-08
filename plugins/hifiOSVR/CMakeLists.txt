#
#  CMakeLists.txt
#  plugins/hifiOSVR
#
#  Created by David Rowe on 5 Jul 2016.
#  Copyright 2016 High Fidelity, Inc.
#
#  Distributed under the Apache License, Version 2.0.
#  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
#

if (WIN32)

    # We're using static GLEW, so define GLEW_STATIC
    add_definitions(-DGLEW_STATIC)

    set(TARGET_NAME hifiOSVR)
    setup_hifi_plugin(OpenGL)
    link_hifi_libraries(shared gl plugins display-plugins controllers)
    
    include_hifi_library_headers(octree)

	find_package(osvr)
    find_package(boost)

	if (OSVR_FOUND AND BOOST_FOUND)
        target_include_directories(${TARGET_NAME} PRIVATE ${OSVR_INCLUDE_DIRS})
        target_link_libraries(${TARGET_NAME} ${OSVR_LIBRARIES})
        target_include_directories(${TARGET_NAME} PRIVATE ${BOOST_INCLUDE_DIRS})
        target_link_libraries(${TARGET_NAME} ${BOOST_LIBRARIES})

        add_definitions(-DHAVE_OSVR)
	endif()

endif()
