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
#include <osvr/RenderKit/RenderManager.h>

#include <gl/OglplusHelpers.h>

#include <GLMHelpers.h>


bool isOsvrAvailable();
bool isOsvrDisplayAvailable();

osvr::clientkit::ClientContext* getOsvrContext();
osvr::clientkit::DisplayConfig* getOsvrDisplay();

const QString HMD_VENDOR = "vendor";
const QString HMD_MODEL = "model";
const QString HMD_VERSION = "Version";
QMap<QString, QString> parseHMDInfo(const std::string& displayString);


inline void fixRenderInfo(std::vector<osvr::renderkit::RenderInfo> &renderInfo) {
    // Swap projection's top and bottom values so that the top value is +ve and the bottom value is -ve.
    // Interface needs this in order to get the correct projection values (though this could be fixed in goGlm().
    // OSVR RenderManager also needs this for a much smoother time-warp correction.
    // TODO: This may be a RenderManager bug; keep an eye out for fixes.
    std::swap(renderInfo[0].projection.top, renderInfo[0].projection.bottom);
    std::swap(renderInfo[1].projection.top, renderInfo[1].projection.bottom);
}

inline glm::mat4 toGlm(const osvr::renderkit::OSVR_ProjectionMatrix &projection) {
    GLdouble proj[16];
    OSVR_Projection_to_OpenGL(proj, projection);
    return glm::mat4(
        proj[0], proj[1], proj[2], proj[3],
        proj[4], proj[5], proj[6], proj[7],
        proj[8], proj[9], proj[10], proj[11],
        proj[12], proj[13], proj[14], proj[15]);
}

inline glm::quat toGlm(const OSVR_Quaternion &quat) {
    return glm::quat(osvrQuatGetW(&quat), -osvrQuatGetX(&quat), -osvrQuatGetY(&quat), -osvrQuatGetZ(&quat));
}

inline glm::vec3 toGlm(const OSVR_Vec3 &vec) {
    return glm::vec3(-osvrVec3GetX(&vec), -osvrVec3GetY(&vec), -osvrVec3GetZ(&vec));
}

#endif
