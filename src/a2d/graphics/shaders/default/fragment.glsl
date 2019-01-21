#version 330 core

uniform sampler2D texture_sampler;

in vec2 uv_;
in vec4 color_;

out vec4 out_color;

void main() {
    out_color = texture(texture_sampler, uv_) * color_;
}
