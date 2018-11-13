#version 130

uniform sampler2D sprite_texture;
uniform vec4 sprite_color;

in vec2 uv;

out vec4 out_color;

void main() {
    // Don't change mul order here
    // Cause on my windows pc it will just show nothing
    // But on Ubuntu WSL it works
    // Maybe it is Nvidia driver bug
    out_color = sprite_color * texture(sprite_texture, uv);
}
