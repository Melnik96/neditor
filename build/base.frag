#version 130

in vec4 frag_color;
// in vec2 frag_uv;

// uniform sampler2D u_tex;
// out vec4 color;

void main() {
  gl_FragColor = frag_color/* * texture2D(u_tex, frag_uv.st)*/;
}