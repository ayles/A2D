#version 330 core

uniform mat4 camera_matrix;
uniform mat4 model_matrix;

in vec2 position;

out vec2 uv;

void main() {
    uv = position.xy + 0.5;
    gl_Position = camera_matrix * model_matrix * vec4(position.x, position.y, 0, 1);
}
