//
//  RenderableParticleEffectEntityItem.h
//  interface/src/entities
//
//  Created by Jason Rickwald on 3/2/15.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef hifi_RenderableParticleEffectEntityItem_h
#define hifi_RenderableParticleEffectEntityItem_h

#include "RenderableEntityItem.h"
#include <ParticleEffectEntityItem.h>
#include <TextureCache.h>

namespace render { namespace entities {

class ParticleEffectEntityRenderer : public TypedEntityRenderer<ParticleEffectEntityItem> {
    using Parent = TypedEntityRenderer<ParticleEffectEntityItem>;
    friend class EntityRenderer;

public:
    ParticleEffectEntityRenderer(const EntityItemPointer& entity);

protected:
    virtual bool needsRenderUpdateFromTypedEntity(const TypedEntityPointer& entity) const override;

    virtual void doRenderUpdateSynchronousTyped(const ScenePointer& scene, Transaction& transaction, const TypedEntityPointer& entity) override;
    virtual void doRenderUpdateAsynchronousTyped(const TypedEntityPointer& entity) override;

    virtual ItemKey getKey() override;
    virtual ShapeKey getShapeKey() override;
    virtual Item::Bound getBound() override;
    virtual void doRender(RenderArgs* args) override;

private:
    using PipelinePointer = gpu::PipelinePointer;
    using BufferPointer = gpu::BufferPointer;
    using TexturePointer = gpu::TexturePointer;
    using Format = gpu::Stream::Format;
    using Buffer = gpu::Buffer;
    using BufferView = gpu::BufferView;

    // CPU particles
    // FIXME either switch to GPU compute particles or switch to simd updating of the particles
    struct CpuParticle {
        float seed { 0.0f };
        uint64_t expiration { 0 };
        float lifetime { 0.0f };
        glm::vec3 basePosition;
        glm::vec3 relativePosition;
        glm::vec3 velocity;
        glm::vec3 acceleration;

        void integrate(float deltaTime) {
            glm::vec3 atSquared = (0.5f * deltaTime * deltaTime) * acceleration;
            relativePosition += velocity * deltaTime + atSquared;
            velocity += acceleration * deltaTime;
            lifetime += deltaTime;
        }
    };
    using CpuParticles = std::deque<CpuParticle>;


    template<typename T>
    struct InterpolationData {
        T start;
        T middle;
        T finish;
        T spread;
    };

    struct ParticleUniforms {
        InterpolationData<float> radius;
        InterpolationData<glm::vec4> color; // rgba
        InterpolationData<float> spin;
        float lifespan;
        int rotateWithEntity;
        glm::vec2 spare;
    };

    static CpuParticle createParticle(uint64_t now, const Transform& baseTransform, const particle::Properties& particleProperties);
    void stepSimulation();

    particle::Properties _particleProperties;
    bool _prevEmitterShouldTrail;
    bool _prevEmitterShouldTrailInitialized { false };
    CpuParticles _cpuParticles;
    bool _emitting { false };
    uint64_t _timeUntilNextEmit { 0 };
    BufferPointer _particleBuffer{ std::make_shared<Buffer>() };
    BufferView _uniformBuffer;
    quint64 _lastSimulated { 0 };

    PulsePropertyGroup _pulseProperties;

    NetworkTexturePointer _networkTexture;
    ScenePointer _scene;
};

} } // namespace 

#endif // hifi_RenderableParticleEffectEntityItem_h
