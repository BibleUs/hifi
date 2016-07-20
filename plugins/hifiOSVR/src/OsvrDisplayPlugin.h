//
//  OsvrDisplayPlugin.h
//  plugins/hifiOSVR/src
//
//  Created by David Rowe on 5 Jul 2016.
//  Copyright 2016 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef hifi_OsvrDisplayPlugin_h
#define hifi_OsvrDisplayPlugin_h

#include <QtGlobal>

#include <osvr/ClientKit/ClientKit.h>
#include <osvr/RenderKit/RenderManager.h>

#include <display-plugins/hmd/HmdDisplayPlugin.h>

class OsvrDisplayPlugin : public HmdDisplayPlugin {
    using Parent = HmdDisplayPlugin;
public:
    bool isSupported() const override;
    const QString& getName() const override { return NAME; }

    void init() override;

    float getTargetFrameRate() const override;

    void customizeContext() override;
    void uncustomizeContext() override;

    void resetSensors() override;

    bool beginFrameRender(uint32_t frameIndex) override;
    void cycleDebugOutput() override;

protected:
    bool internalActivate() override;
    void internalDeactivate() override;
    void updatePresentPose() override;

    void hmdPresent() override;
    bool isHmdMounted() const override;
    void postPreview() override;

private:
    static const QString NAME;

    osvr::clientkit::ClientContext* _osvrContext{ nullptr };
    osvr::renderkit::RenderManager* _osvrRender{ nullptr };

    osvr::renderkit::RenderManager::RenderParams _renderParams;
    std::vector<osvr::renderkit::RenderInfo> _renderInfo;

    osvr::renderkit::RenderBuffer _colorBuffer;
    std::vector<osvr::renderkit::RenderBuffer> _colorBuffers;
    std::vector<osvr::renderkit::OSVR_ViewportDescription> _textureViewports;

    float _targetFrameRate{ 60.0f };

    glm::quat _sensorZeroRotation{ glm::quat() };
};

#endif
