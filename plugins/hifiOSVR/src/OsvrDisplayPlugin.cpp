//
//  OsvrDisplayPlugin.cpp
//  plugins/hifiOSVR/src
//
//  Created by David Rowe on 5 Jul 2016.
//  Copyright 2016 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "OsvrDisplayPlugin.h"
#include "OsvrHelpers.h"

const QString OsvrDisplayPlugin::NAME("OSVR HMD");

bool OsvrDisplayPlugin::isSupported() const {
    return isOsvrDisplayAvailable();
}

void OsvrDisplayPlugin::customizeContext() {
    // TODO
}

void OsvrDisplayPlugin::submitSceneTexture(uint32_t frameIndex, const gpu::TexturePointer& sceneTexture) {
    // TODO: Remove this method as part of implementing rendering.
}

void OsvrDisplayPlugin::submitOverlayTexture(const gpu::TexturePointer& overlayTexture) {
    // TODO: Remove this method as part of implementing rendering.
}

void OsvrDisplayPlugin::hmdPresent() {
    // TODO
};

bool OsvrDisplayPlugin::isHmdMounted() const {
    return true;  // TODO
}
