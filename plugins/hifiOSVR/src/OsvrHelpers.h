//
//  OsvrHelpers.h
//  plugins/hifiOSVR/src
//
//  Created by David Rowe on 8 Jul 2016.
//  Copyright 2016 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef hifi_OsvrHelpers_h
#define hifi_OsvrHelpers_h

#include <osvr/ClientKit/ClientKit.h>
#include <osvr/ClientKit/Display.h>

#include <gl/OglplusHelpers.h>

#include <GLMHelpers.h>


bool isOsvrAvailable();
bool isOsvrDisplayAvailable();

osvr::clientkit::ClientContext* getOsvrContext();
osvr::clientkit::DisplayConfig* getOsvrDisplay();


inline glm::mat4 toGlm(const GLdouble* proj) {
    return glm::mat4(
        proj[0], proj[1], proj[2], proj[3], 
        proj[4], -proj[5], proj[6], proj[7],  // Flip y [1,1].
        proj[8], proj[9], proj[10], proj[11], 
        proj[12], proj[13], proj[14], proj[15]);
}

inline glm::quat toGlm(const OSVR_Quaternion &quat) {
    return glm::quat(osvrQuatGetW(&quat), osvrQuatGetX(&quat), osvrQuatGetY(&quat), osvrQuatGetZ(&quat));
}

inline glm::vec3 toGlm(const OSVR_Vec3 &vec) {
    return glm::vec3(osvrVec3GetX(&vec), osvrVec3GetY(&vec), osvrVec3GetZ(&vec));
}

#endif
