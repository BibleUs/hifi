//
//  OsvrControllerManager.cpp
//  plugins/hifiOSVR/src
//
//  Created by David Rowe on 5 Jul 2016.
//  Copyright 2016 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "OsvrControllerManager.h"

const QString OsvrControllerManager::NAME = "OSVR";

bool OsvrControllerManager::isSupported() const {
    return false;  // OSVR controller support not implemented.
}

void OsvrControllerManager::pluginFocusOutEvent() {
    // OSVR controller support not implemented.
}

void OsvrControllerManager::pluginUpdate(float deltaTime, const controller::InputCalibrationData& inputCalibrationData) {
    // OSVR controller support not implemented.
}
