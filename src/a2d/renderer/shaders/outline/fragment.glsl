#version 330 core

uniform sampler2D main_texture;
uniform ivec2 resolution;

in vec2 uv_;
in vec4 color_;

out vec4 out_color;

void main() {
    out_color = texture(main_texture, uv_) * color_;

    vec2 offset = vec2(1.0) / resolution * 3;

    if (out_color.a < 0.4) {
        vec4 v[8];
        v[0] = texture(main_texture, vec2(uv_.x - offset.x, uv_.y)) * color_;
        v[1] = texture(main_texture, vec2(uv_.x + offset.x, uv_.y)) * color_;
        v[2] = texture(main_texture, vec2(uv_.x, uv_.y - offset.y)) * color_;
        v[3] = texture(main_texture, vec2(uv_.x, uv_.y + offset.y)) * color_;
        v[4] = texture(main_texture, vec2(uv_.x + offset.x, uv_.y + offset.y)) * color_;
        v[5] = texture(main_texture, vec2(uv_.x + offset.x, uv_.y - offset.y)) * color_;
        v[6] = texture(main_texture, vec2(uv_.x - offset.x, uv_.y + offset.y)) * color_;
        v[7] = texture(main_texture, vec2(uv_.x - offset.x, uv_.y - offset.y)) * color_;
        vec4 result = v[0];
        for (int i = 1; i < 8; ++i) {
            result = max(result, v[i]);
        }
        if (result.a > 0.1) {
            out_color = vec4(1, 0, 0, 1);
        }
    }
}
