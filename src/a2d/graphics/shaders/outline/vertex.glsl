#version 330 core

uniform mat4 camera_matrix;

in vec2 position;
in vec2 uv;
in vec4 color;

out vec2 uv_;
out vec4 color_;

void main() {
    uv_ = uv;
    color_ = color;
    gl_Position = camera_matrix * vec4(position.x, position.y, 0, 1);
}
