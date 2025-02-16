#
#  Created by Bradley Austin Davis on 2015/11/18
#  Copyright 2015 High Fidelity, Inc.
#
#  Distributed under the Apache License, Version 2.0.
#  See the accompanying file LICENSE or http:#www.apache.org/licenses/LICENSE-2.0.html
#

if ((WIN32 OR UNIX) AND (NOT USE_GLES))
    set(TARGET_NAME openvr)
    setup_hifi_plugin(Gui Qml Multimedia)
    link_hifi_libraries(shared task gl qml networking controllers ui
        plugins display-plugins ui-plugins input-plugins script-engine
        audio-client render-utils graphics shaders gpu render material-networking model-networking model-baker hfm fbx ktx image procedural ${PLATFORM_GL_BACKEND})
    include_hifi_library_headers(octree)

    target_openvr()

    if (WIN32)
        # we're using static GLEW, so define GLEW_STATIC
        add_definitions(-DGLEW_STATIC)
        target_link_libraries(${TARGET_NAME} Winmm.lib)
    endif()
endif()
