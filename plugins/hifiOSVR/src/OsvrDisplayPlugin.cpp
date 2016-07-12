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

void OsvrDisplayPlugin::init() {
    // Initialize plugin. Assumes isSupported() is true.
    Plugin::init();
    emit deviceConnected(getName());
}

float OsvrDisplayPlugin::getTargetFrameRate() const {
    return 60.0f;  // TODO
}

void OsvrDisplayPlugin::customizeContext() {
    // TODO
}

void OsvrDisplayPlugin::uncustomizeContext() {
    // TODO
    //Parent::uncustomizeContext();
}

void OsvrDisplayPlugin::resetSensors() {
    // TODO
}

bool OsvrDisplayPlugin::beginFrameRender(uint32_t frameIndex) {
    // TODO
    return Parent::beginFrameRender(frameIndex);
}

void OsvrDisplayPlugin::cycleDebugOutput() {
    _lockCurrentTexture = !_lockCurrentTexture;
}

void OsvrDisplayPlugin::submitSceneTexture(uint32_t frameIndex, const gpu::TexturePointer& sceneTexture) {
    // TODO: Remove this method as part of implementing rendering.
}

void OsvrDisplayPlugin::submitOverlayTexture(const gpu::TexturePointer& overlayTexture) {
    // TODO: Remove this method as part of implementing rendering.
}

bool OsvrDisplayPlugin::internalActivate() {
    // TODO

    // Initialize OSVR rendering
    _osvrContext = getOsvrContext();
    _osvrRender = osvr::renderkit::createRenderManager(_osvrContext->get(), "OpenGL");
    if ((_osvrRender == nullptr) || (!_osvrRender->doingOkay())) {
        qDebug() << "OSVR: Could not create RenderManager";  // TODO: Delete?
        return false;
    }

    osvr::renderkit::RenderManager::OpenResults result = _osvrRender->OpenDisplay();
    if (result.status == osvr::renderkit::RenderManager::OpenStatus::FAILURE) {
        qDebug() << "OSVR: Could not open display";  // TODO: Delete?
        delete _osvrRender;
        return false;
    }
    if (result.library.OpenGL == nullptr) {
        qDebug() << "OSVR: Graphics library not configured as OpenGL";
        delete _osvrRender;
        return false;
    }
    // Set up render parameters
    std::vector<osvr::renderkit::RenderInfo> renderInfo;
    _osvrContext->update();
    renderInfo = _osvrRender->GetRenderInfo();
    _renderTargetSize = uvec2(
        renderInfo[0].viewport.width + renderInfo[1].viewport.width,
        std::max(renderInfo[0].viewport.height, renderInfo[1].viewport.height)
        );

    return Parent::internalActivate();
}

void OsvrDisplayPlugin::internalDeactivate() {
    Parent::internalDeactivate();
    // TODO
}

void OsvrDisplayPlugin::updatePresentPose() {
    // TODO
}

void OsvrDisplayPlugin::hmdPresent() {
    // TODO
};

bool OsvrDisplayPlugin::isHmdMounted() const {
    return true;  // TODO
}

void OsvrDisplayPlugin::postPreview() {
    // TODO
}
