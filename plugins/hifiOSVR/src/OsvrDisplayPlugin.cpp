//
//  OsvrDisplayPlugin.cpp
//  plugins/hifiOSVR/src
//
//  Created by David Rowe on 5 Jul 2016.
//  Copyright 2016 High Fidelity, Inc.
//
//  Assumes OSVR RenderManager is configured with asynchronous timewarp enabled.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "OsvrDisplayPlugin.h"

#include <display-plugins/CompositorHelper.h>
#include <gpu/gl/GLBackend.h>

#include <ViewFrustum.h>

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

    // OSVR doesn't currently have any function reflecting this. Instead, just return whether the HMD plug-in is activated.
    return _isActivated;
}

float OsvrDisplayPlugin::getTargetFrameRate() const {
    // HMD display refresh rate.
    return _targetFrameRate;
}

void OsvrDisplayPlugin::resetSensors() {
    // Reset HMD tracking.

    if (_osvrContext && _osvrRender) {
        _osvrContext->update();
        _osvrRender->ClearRoomToWorldTransform();
        _sensorZeroRotation = glm::inverse(toGlm(_osvrRender->GetRenderInfo()[0].pose.rotation));
        _sensorZeroTranslation = -0.5f * (toGlm(_osvrRender->GetRenderInfo()[0].pose.translation) 
            + toGlm(_osvrRender->GetRenderInfo()[1].pose.translation));
    }

    _currentRenderFrameInfo.renderPose = glm::mat4();
}

void OsvrDisplayPlugin::cycleDebugOutput() {
    // HMD-specific debug output.

    // OSVR doesn't have any specific debug overlay or similar; just lock the current texture instead.
    _lockCurrentTexture = !_lockCurrentTexture;
}

bool OsvrDisplayPlugin::internalActivate() {
    // Set up HMD rendering.

    qDebug() << "OSVR: Activate";

    // Initialize OSVR rendering
    _osvrContext = getOsvrContext();
    _osvrRender = osvr::renderkit::createRenderManager(_osvrContext->get(), "OpenGL");
    if ((_osvrRender == nullptr) || (!_osvrRender->doingOkay())) {
        qWarning() << "OSVR: Could not create RenderManager";
        if (_osvrRender != nullptr) {
            delete _osvrRender;
            _osvrRender = nullptr;
        }
        return false;
    }
    _osvrContext->update();

    auto hmdInfo = parseHMDInfo(_osvrContext->getStringParameter("/display"));
    qDebug() << "OSVR: HMD =" << hmdInfo[HMD_VENDOR] << hmdInfo[HMD_MODEL] << hmdInfo[HMD_VERSION];

    // FIXME: Interface crashes at the following line if start up in HMD mode; OSVR SDK 0.6.1337 build 329.
    // Work-around implemented as FIXME in Application::loadSettings().
    osvr::renderkit::RenderManager::OpenResults result = _osvrRender->OpenDisplay();
    if (result.status == osvr::renderkit::RenderManager::OpenStatus::FAILURE) {
        qWarning() << "OSVR: Could not open display";
        delete _osvrRender;
        _osvrRender = nullptr;
        return false;
    }
    if (result.library.OpenGL == nullptr) {
        qWarning() << "OSVR: Graphics library not configured as OpenGL";
        delete _osvrRender;
        _osvrRender = nullptr;
        return false;
    }

    if (_osvrRender->GetRenderInfo(_renderParams).size() != 2) {
        qWarning() << "OSVR: Display does not have 2 eyes";
        delete _osvrRender;
        _osvrRender = nullptr;
        return false;
    }

    // Get HMD's target frame rate.
    osvr::renderkit::RenderTimingInfo timingInfo;
    if (_osvrRender->GetTimingInfo(0, timingInfo)) {
        _targetFrameRate = roundf((float)USECS_PER_SECOND / (float)timingInfo.hardwareDisplayInterval.microseconds);
        qDebug() << "OSVR: HMD frame rate =" << _targetFrameRate;
    } else {
        const float DEFAULT_TARGET_FRAME_RATE = 60.0f;
        _targetFrameRate = DEFAULT_TARGET_FRAME_RATE;
        qDebug() << "OSVR: Could not obtain HMD's frame rate; using" << _targetFrameRate;
    }

    // Get HMD's IPD.
    osvr::clientkit::DisplayConfig* display = getOsvrDisplay();
    osvr::clientkit::Eye eyes[2] = { display->getEye(0, 0), display->getEye(0, 1) };
    OSVR_Pose3 eyePoses[2];
    if (eyes[0].getPose(eyePoses[0]) && eyes[1].getPose(eyePoses[1])) {
        _ipd = glm::distance(toGlm(eyePoses[0].translation), toGlm(eyePoses[1].translation));
        qDebug() << "OSVR: IPD =" << _ipd;
    } else {
        qWarning() << "OSVR: Could not obtain HMD's IPD; using " << _ipd;
    }

    // Set up Interface render parameters.
    std::vector<osvr::renderkit::RenderInfo> renderInfo;
    _renderParams.nearClipDistanceMeters = DEFAULT_NEAR_CLIP;
    _renderParams.farClipDistanceMeters = DEFAULT_FAR_CLIP;
    _renderParams.IPDMeters = _ipd;
    renderInfo = _osvrRender->GetRenderInfo(_renderParams);
    if (renderInfo.size() != 2) {  // Check again to be sure; size() has been observed to occasionally be 0 during operation.
        qWarning() << "OSVR: Could not obtain render info";
        delete _osvrRender;
        _osvrRender = nullptr;
        return false;
    }

    _renderTargetSize = uvec2(
        renderInfo[0].viewport.width + renderInfo[1].viewport.width,
        std::max(renderInfo[0].viewport.height, renderInfo[1].viewport.height)
        );

    flipProjection(renderInfo);
    _eyeProjections[0] = toGlm(renderInfo[0].projection);
    _eyeProjections[1] = toGlm(renderInfo[1].projection);

    _eyeOffsets[0] = glm::translate(mat4(), glm::vec3(-_ipd / 2.0f, 0.0f, 0.0f));
    _eyeOffsets[1] = glm::translate(mat4(), glm::vec3(_ipd / 2.0f, 0.0f, 0.0f));

    _cullingProjection = _eyeProjections[0];  // Eyes have identical projections.
    if (_eyeProjections[0] != _eyeProjections[1]) {
        // Warn if not the case in the future.
        qWarning() << "OSVR: Improper culling projection because eye projections are not identical";
    }

    // Set up OSVR render parameters.
    osvr::renderkit::OSVR_ViewportDescription textureViewportLeft{ 0.0f, 0.0f, 0.5f, 1.0f };
    _textureViewports.push_back(textureViewportLeft);
    osvr::renderkit::OSVR_ViewportDescription textureViewportRight{ 0.5f, 0.0f, 0.5f, 1.0f };
    _textureViewports.push_back(textureViewportRight);

    // Activate parent.
    _isActivated = Parent::internalActivate();
    if (!_isActivated) {
        delete _osvrRender;
        _osvrRender = nullptr;
    }

    return _isActivated;
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

    // Set up texture and buffer for OSVR rendering.
    GLuint colorBufferName;
    glGenTextures(1, &colorBufferName);
    glBindTexture(GL_TEXTURE_2D, colorBufferName);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _renderTargetSize.x, _renderTargetSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    _outputFramebuffer =  gpu::FramebufferPointer(
        gpu::Framebuffer::create("osvrOutput", gpu::Element::COLOR_RGBA_32, _renderTargetSize.x, _renderTargetSize.y));
    auto fbo = getGLBackend()->getFramebufferID(_outputFramebuffer);
    glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, colorBufferName, 0); 

    // Register render texture with OSVR library.
    _colorBuffer.OpenGL = new osvr::renderkit::RenderBufferOpenGL;
    _colorBuffer.OpenGL->colorBufferName = colorBufferName;
    _colorBuffers.push_back(_colorBuffer);
    _colorBuffers.push_back(_colorBuffer);  // Single buffer for both eyes.
    if (!_osvrRender->RegisterRenderBuffers(_colorBuffers)) {
        qWarning() << "OSVR: Could not register render buffers";
    }
}

bool OsvrDisplayPlugin::beginFrameRender(uint32_t frameIndex) {
    // Prepare to render frame.

    _osvrContext->update();  // Update tracker state.
    _renderInfo = _osvrRender->GetRenderInfo(_renderParams);
    if (_renderInfo.size() != 2) {
        qWarning() << "OSVR: No eye information available to render frame";
        return false;
    }

    _currentRenderFrameInfo = FrameInfo();
    glm::quat rotation = toGlm(_renderInfo[0].pose.rotation) * _sensorZeroRotation;  // Both eye views have the same rotation.
    glm::vec3 translation = rotation 
        * ((toGlm(_renderInfo[0].pose.translation) + toGlm(_renderInfo[1].pose.translation)) / 2.0f + _sensorZeroTranslation);

    _currentRenderFrameInfo.renderPose = glm::translate(glm::mat4(), translation) * glm::mat4_cast(rotation);
    _currentRenderFrameInfo.presentPose = _currentRenderFrameInfo.renderPose;

    withNonPresentThreadLock([&] {
        _uiModelTransform = DependencyManager::get<CompositorHelper>()->getModelTransform();
        _frameInfos[frameIndex] = _currentRenderFrameInfo;
    });

    return Parent::beginFrameRender(frameIndex);
}

void OsvrDisplayPlugin::hmdPresent() {
    // Submit frame to HMD.

    auto frameIndex = _currentFrame->frameIndex;

    if (frameIndex == _presentedFrame) {
        return;  // Only submit new frame so that OSVR RenderManager's asynchronous timewarp can do its thing.
    }
    _presentedFrame = frameIndex;

    PROFILE_RANGE_EX(__FUNCTION__, 0xff00ff00, (uint64_t)frameIndex);

    // Copy image into HMD buffer for display.
    // FIXME per similar OculusDisplayPlugin FIXME.
    render([&](gpu::Batch& batch) {
        batch.enableStereo(false);
        auto source = _compositeFramebuffer;
        auto sourceRect = ivec4(ivec2(0), source->getSize());
        auto dest = _outputFramebuffer;
        auto destRect = ivec4(ivec2(0), dest->getSize());
        batch.blit(source, sourceRect, dest, destRect);
    });

    _presentInfo = _renderInfo;
    flipProjection(_presentInfo);
    const bool FLIP_IN_Y = true;
    if (!_osvrRender->PresentRenderBuffers(_colorBuffers, _presentInfo, _renderParams, _textureViewports, FLIP_IN_Y)) {
        qWarning() << "OSVR: Failed to present image on HMD";
    }

    _presentRate.increment();
};

void OsvrDisplayPlugin::uncustomizeContext() {
    // Revert OpenGL context to desktop's.
    glDeleteTextures(1, &_colorBuffer.OpenGL->colorBufferName);
    delete _colorBuffer.OpenGL;
    _colorBuffer.OpenGL = nullptr;
    _colorBuffers.clear();
    _outputFramebuffer.reset();

    Parent::uncustomizeContext();
}

void OsvrDisplayPlugin::internalDeactivate() {
    // Tear down HMD rendering.

    qDebug() << "OSVR: Deactivate";

    Parent::internalDeactivate();
    delete _osvrRender;
    _osvrRender = nullptr;

    _isActivated = false;
}
