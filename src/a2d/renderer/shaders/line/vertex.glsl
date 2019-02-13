#version 330 core

uniform mat4 camera_matrix;
uniform mat4 transform_matrix;

in vec2 position;
in vec4 color;

out vec4 color_;

void main() {
    color_ = color;
    gl_Position = camera_matrix * transform_matrix * vec4(position.x, position.y, 0, 1);
}
