//
//  OsvrHmdControllerManager.cpp
//  plugins/osvrHMD/src
//
//  Created by David Rowe on 5 Jul 2016.
//  Copyright 2016 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "OsvrHmdControllerManager.h"

const QString OsvrHmdControllerManager::NAME = "OSVR HMD";

bool OsvrHmdControllerManager::isSupported() const {
    return true;
}

void OsvrHmdControllerManager::pluginFocusOutEvent() {
    // TODO
}

void OsvrHmdControllerManager::pluginUpdate(float deltaTime, const controller::InputCalibrationData& inputCalibrationData) {
    // TODO
}
