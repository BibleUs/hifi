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

// Free up "near" and "far" default identifiers defined by Windows and used by OSVR.
#undef near
#undef far


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

    void hmdPresent() override;
    bool isHmdMounted() const override;

private:
    static const QString NAME;

    osvr::clientkit::ClientContext* _osvrContext{ nullptr };
    osvr::renderkit::RenderManager* _osvrRender{ nullptr };

    osvr::renderkit::RenderManager::RenderParams _renderParams;
    std::vector<osvr::renderkit::RenderInfo> _renderInfo;
    std::vector<osvr::renderkit::RenderInfo> _presentInfo;

    osvr::renderkit::RenderBuffer _colorBuffer;
    std::vector<osvr::renderkit::RenderBuffer> _colorBuffers;
    std::vector<osvr::renderkit::OSVR_ViewportDescription> _textureViewports;

    bool _isActivated{ false };

    float _targetFrameRate{ 60.0f };

    uint32_t _presentedFrame{ 0 };

    glm::quat _sensorZeroRotation{ glm::quat() };
    glm::vec3 _sensorZeroTranslation{ glm::vec3() };

    gpu::FramebufferPointer _outputFramebuffer;
};

#endif
