#ifdef RENDERER_GL_ES

precision highp float;
#define IN varying
#define OUT_COLOR gl_FragColor

#else

#define IN in
#define OUT_COLOR out_color
out vec4 out_color;

#endif

IN vec4 color_;

void main() {
    OUT_COLOR = color_;
}
