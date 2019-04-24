//
// Created by selya on 26.10.2018.
//

#ifndef A2D_A2D_HPP
#define A2D_A2D_HPP

// Audio
#include <a2d/audio/audio.hpp>
#include <a2d/audio/audio_clip.hpp>
#include <a2d/audio/audio_source.hpp>

// Core
#include <a2d/core/component.hpp>
#include <a2d/core/engine.hpp>
#include <a2d/core/intrusive_ptr.hpp>
#include <a2d/core/macro.hpp>
#include <a2d/core/object2d.hpp>
#include <a2d/core/ref_counter.hpp>
#include <a2d/core/resources.hpp>

// Filesystem
#include <a2d/filesystem/filesystem.hpp>

// Renderer
#include <a2d/renderer/bitmap_font.hpp>
#include <a2d/renderer/drawable.hpp>
#include <a2d/renderer/gl.hpp>
#include <a2d/renderer/renderer.hpp>
#include <a2d/renderer/shader.hpp>
#include <a2d/renderer/sprite_batch.hpp>
#include <a2d/renderer/texture/texture.hpp>
#include <a2d/renderer/texture/texture_buffer.hpp>
#include <a2d/renderer/texture/texture_region.hpp>
#include <a2d/renderer/material.hpp>
#include <a2d/renderer/sprite.hpp>

// Input
#include <a2d/input/input.hpp>

// Math
#include <a2d/math/math.hpp>

// Components
#include <a2d/core/camera.hpp>
#include <a2d/renderer/sprite.hpp>
#include <a2d/renderer/text.hpp>
#include <a2d/physics/rigidbody.hpp>
#include <a2d/renderer/line.hpp>
#include <a2d/physics/rigidbody.hpp>
#include <a2d/physics/physics_collider.hpp>
#include <a2d/physics/circle_collider.hpp>
#include <a2d/physics/polygon_collider.hpp>
#include <a2d/physics/chain_collider.hpp>
#include <a2d/core/animation/animator.hpp>
#include <a2d/core/animation/animation.hpp>

namespace a2d {

DECLARE_SMART_POINTER(Object2D)
DECLARE_SMART_POINTER(TextureRegion)

}

#endif //A2D_A2D_H
