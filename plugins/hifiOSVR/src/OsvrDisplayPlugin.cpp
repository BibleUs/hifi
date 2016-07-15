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

#include <display-plugins/CompositorHelper.h>

#include "OsvrHelpers.h"

// The following need to come after other #includes.
#include <GL/gl.h>
#include <osvr/RenderKit/GraphicsLibraryOpenGL.h>  // Must be included after <GL/gl.h>


const QString OsvrDisplayPlugin::NAME("OSVR HMD");

bool OsvrDisplayPlugin::isSupported() const {
    return isOsvrDisplayAvailable();
}

void OsvrDisplayPlugin::init() {
    // Assumes isSupported() is true.
    Plugin::init();
    emit deviceConnected(getName());
}

bool OsvrDisplayPlugin::isHmdMounted() const {
    // Is the user wearing the HMD?

    // TODO

    return true;
}

float OsvrDisplayPlugin::getTargetFrameRate() const {
    return 60.0f;  // TODO
}

void OsvrDisplayPlugin::resetSensors() {
    // Reset HMD tracking.

    // TODO
}

void OsvrDisplayPlugin::cycleDebugOutput() {
    _lockCurrentTexture = !_lockCurrentTexture;

    // TODO

}

bool OsvrDisplayPlugin::internalActivate() {
    // Set up HMD rendering.

    // Initialize OSVR rendering
    _osvrContext = getOsvrContext();
    _osvrRender = osvr::renderkit::createRenderManager(_osvrContext->get(), "OpenGL");
    if ((_osvrRender == nullptr) || (!_osvrRender->doingOkay())) {
        qDebug() << "OSVR: Could not create RenderManager";  // TODO: Delete?
        // TODO: Delete _osvrContext?
        return false;
    }

    osvr::renderkit::RenderManager::OpenResults result = _osvrRender->OpenDisplay();
    if (result.status == osvr::renderkit::RenderManager::OpenStatus::FAILURE) {
        qDebug() << "OSVR: Could not open display";  // TODO: Delete?
        delete _osvrRender;
        // TODO: Delete _osvrContext?
        return false;
    }
    if (result.library.OpenGL == nullptr) {
        qDebug() << "OSVR: Graphics library not configured as OpenGL";
        delete _osvrRender;
        // TODO: Delete _osvrContext?
        return false;
    }

    // Set up render parameters
    std::vector<osvr::renderkit::RenderInfo> renderInfo;
    _osvrContext->update();
    renderInfo = _osvrRender->GetRenderInfo();
    if (renderInfo.size() != 2) {
        qDebug() << "OSVR: Display does not have 2 eyes";
        delete _osvrRender;
        // TODO: Delete _osvrContext?
        return false;
    }

    _renderTargetSize = uvec2(
        renderInfo[0].viewport.width + renderInfo[1].viewport.width,
        std::max(renderInfo[0].viewport.height, renderInfo[1].viewport.height)
        );

    osvr::renderkit::OSVR_ViewportDescription textureViewportLeft{ 0.0f, 0.0f, 0.5f, 1.0f };
    _textureViewports.push_back(textureViewportLeft);
    osvr::renderkit::OSVR_ViewportDescription textureViewportRight{ 0.5f, 0.0f, 0.5f, 1.0f };
    _textureViewports.push_back(textureViewportRight);

    return Parent::internalActivate();
    // TODO: Delete _osvrContext if returnValue == false?
}

void OsvrDisplayPlugin::customizeContext() {
    // Customize OpenGL context.

    // Display plug-in DLLs must initialize glew locally.
    static std::once_flag once;
    std::call_once(once, [] {
        glewExperimental = true;
        GLenum err = glewInit();
        glGetError();  // Clear the potential error from glewExperimental.
    });

    Parent::customizeContext();  // Must be before RegisterRenderBuffers().

    // Make OSVR display from Interface frame buffer.
    _colorBuffer.OpenGL = new osvr::renderkit::RenderBufferOpenGL;
    _colorBuffer.OpenGL->colorBufferName = GetName(_compositeFramebuffer->color);
    _colorBuffers.clear();
    _colorBuffers.push_back(_colorBuffer);
    _colorBuffers.push_back(_colorBuffer);  // Single buffer for both eyes.
    if (!_osvrRender->RegisterRenderBuffers(_colorBuffers)) {
        qDebug() << "OSVR: Could not register render buffers";
        delete _osvrContext;  // TODO: Correct?
        //return false;  // TODO: How to handle?
    }
}

bool OsvrDisplayPlugin::beginFrameRender(uint32_t frameIndex) {
    // Prepare to render frame.

    _osvrContext->update();  // Update tracker state.
    _renderInfo = _osvrRender->GetRenderInfo();  // TODO: Pass info into this call, e.g. IPD?

    _currentRenderFrameInfo = FrameInfo();
    //_currentRenderFrameInfo.sensorSampleTime = ?  // TODO: Needed?
    //_currentRenderFrameInfo.predictedDisplayTime = ?  // TODO: Needed?

    // TODO: Use the head pose rather than the left eye's pose.
    GLdouble modelView[16];
    osvr::renderkit::OSVR_PoseState_to_OpenGL(modelView, _renderInfo[0].pose);
    _currentRenderFrameInfo.renderPose = static_cast<glm::mat4>(modelView[0]);
    _currentRenderFrameInfo.presentPose = _currentRenderFrameInfo.renderPose;  // TODO: Needed? C.f. updatePresentPose()?

    withRenderThreadLock([&] {
        _uiModelTransform = DependencyManager::get<CompositorHelper>()->getModelTransform();
        _frameInfos[frameIndex] = _currentRenderFrameInfo;
    });

    return Parent::beginFrameRender(frameIndex);
}

void OsvrDisplayPlugin::updatePresentPose() {
    // Update pose and projection prior to present.

    // TODO
}

void OsvrDisplayPlugin::hmdPresent() {
    // Submit frame to HMD.

    const bool FLIP_IN_Y = true;

    if (!_osvrRender->PresentRenderBuffers(_colorBuffers,
        _renderInfo,
        osvr::renderkit::RenderManager::RenderParams(), // TODO: Update if pass parameters into GetRenderInfo() at start of frame.
        _textureViewports,
        FLIP_IN_Y)) {
        // TODO: What to do?
    }
};

void OsvrDisplayPlugin::postPreview() {
    // Tidying after HMD scene mirrored to desktop.

    // TODO: Delete this method if not needed.
}

void OsvrDisplayPlugin::uncustomizeContext() {
    // Revert OpenGL context to desktop's.
    delete _colorBuffer.OpenGL;
    Parent::uncustomizeContext();
}

void OsvrDisplayPlugin::internalDeactivate() {
    // Tear down HMD rendering.
    Parent::internalDeactivate();
    delete _osvrRender;
}
