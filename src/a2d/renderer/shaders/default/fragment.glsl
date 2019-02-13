#version 330 core

uniform sampler2D main_texture;

in vec2 uv_;
in vec4 color_;

out vec4 out_color;

void main() {
    out_color = texture(main_texture, uv_) * color_;
}
