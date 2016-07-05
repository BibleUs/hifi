//
//  OsvrHmdDisplayPlugin.h
//  plugins/osvrHMD/src
//
//  Created by David Rowe on 5 Jul 2016.
//  Copyright 2016 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef hifi_OsvrHmdDisplayPlugin_h
#define hifi_OsvrHmdDisplayPlugin_h

#include <QtGlobal>

#include <display-plugins/hmd/HmdDisplayPlugin.h>

class OsvrHmdDisplayPlugin : public HmdDisplayPlugin {
    using Parent = HmdDisplayPlugin;
public:
    bool isSupported() const override;
    const QString& getName() const override { return NAME; }

    void customizeContext() override;

    void submitSceneTexture(uint32_t frameIndex, const gpu::TexturePointer& sceneTexture) override;
    void submitOverlayTexture(const gpu::TexturePointer& overlayTexture) override;

protected:
    void hmdPresent() override;
    bool isHmdMounted() const override;

private:
    static const QString NAME;
};

#endif
