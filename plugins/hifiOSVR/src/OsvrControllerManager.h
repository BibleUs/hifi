//
//  OsvrControllerManager.h
//  plugins/hifiOSVR/src
//
//  Created by David Rowe on 5 Jul 2016.
//  Copyright 2016 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef hifi_OsvrControllerManager_h
#define hifi_OsvrControllerManager_h

#include <QObject>

#include <controllers/InputDevice.h>
#include <plugins/InputPlugin.h>

class OsvrControllerManager : public InputPlugin {
    Q_OBJECT
public:
    bool isSupported() const override;
    const QString& getName() const override { return NAME; }

    void pluginFocusOutEvent() override;
    void pluginUpdate(float deltaTime, const controller::InputCalibrationData& inputCalibrationData) override;

private:
    static const QString NAME;
};

#endif
