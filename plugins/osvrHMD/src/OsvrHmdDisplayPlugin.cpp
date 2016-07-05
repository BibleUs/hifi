//
//  OsvrHmdDisplayPlugin.cpp
//  plugins/osvrHMD/src
//
//  Created by David Rowe on 5 Jul 2016.
//  Copyright 2016 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "OsvrHmdDisplayPlugin.h"

const QString OsvrHmdDisplayPlugin::NAME("OSVR HMD");

bool OsvrHmdDisplayPlugin::isSupported() const {
    return true;  // TODO
}

void OsvrHmdDisplayPlugin::customizeContext() {
    // TODO
}

void OsvrHmdDisplayPlugin::submitSceneTexture(uint32_t frameIndex, const gpu::TexturePointer& sceneTexture) {
    // TODO: Remove this method as part of implementing rendering.
}

void OsvrHmdDisplayPlugin::submitOverlayTexture(const gpu::TexturePointer& overlayTexture) {
    // TODO: Remove this method as part of implementing rendering.
}

void OsvrHmdDisplayPlugin::hmdPresent() {
    // TODO
};

bool OsvrHmdDisplayPlugin::isHmdMounted() const {
    return true;  // TODO
}
