#version 330 core

uniform sampler2D texture_sampler;

in vec2 uv_;
in vec4 color_;

out vec4 out_color;

const float offset = 1 / 4096.0;

void main() {
    out_color = texture(texture_sampler, uv_) * color_;

    if (out_color.a < 0.1) {
        vec4 l = texture(texture_sampler, vec2(uv_.x - offset, uv_.y)) * color_;
        vec4 r = texture(texture_sampler, vec2(uv_.x + offset, uv_.y)) * color_;
        vec4 b = texture(texture_sampler, vec2(uv_.x, uv_.y - offset)) * color_;
        vec4 t = texture(texture_sampler, vec2(uv_.x, uv_.y + offset)) * color_;
        vec4 result = max(max(l, r), max(b, t));
        if (result.a > 0.1) {
            out_color = vec4(0.3, 0.3, 0.3, 1);
        }
    }
}
