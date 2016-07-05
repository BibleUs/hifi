#
#  CMakeLists.txt
#  plugins/osvrHMD
#
#  Created by David Rowe on 5 Jul 2016.
#  Copyright 2016 High Fidelity, Inc.
#
#  Distributed under the Apache License, Version 2.0.
#  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
#

#if (WIN32)

    # We're using static GLEW, so define GLEW_STATIC
    add_definitions(-DGLEW_STATIC)

    set(TARGET_NAME osvrHMD)
    setup_hifi_plugin(OpenGL)
    link_hifi_libraries(shared gl plugins display-plugins controllers)
    
    include_hifi_library_headers(octree)

#endif()
