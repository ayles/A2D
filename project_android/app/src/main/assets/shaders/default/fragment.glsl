#ifdef ES

precision highp float;
#define IN varying
#define TEXTURE texture2D
#define OUT_COLOR gl_FragColor

#else

#define IN in
#define TEXTURE texture
#define OUT_COLOR out_color
out vec4 out_color;

#endif

uniform sampler2D sprite_texture;
uniform vec4 sprite_color;

IN vec2 uv;

void main() {
    // Don't change mul order here
    // Cause on my windows pc it will just show nothing
    // But on Ubuntu WSL it works
    // Maybe it is Nvidia driver bug
    OUT_COLOR = sprite_color * TEXTURE(sprite_texture, uv);
}
