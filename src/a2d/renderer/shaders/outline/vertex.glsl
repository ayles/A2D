#version 330 core

uniform mat4 camera_matrix;
uniform ivec2 resolution;

in vec2 position;
in vec2 uv;
in vec4 color;

out vec2 uv_;
out vec4 color_;

void main() {
    vec2 offset = vec2(1.0) / resolution;
    uv_ = uv;
    color_ = color;
    gl_Position = camera_matrix * vec4(position.x, position.y, 0, 1);
}
