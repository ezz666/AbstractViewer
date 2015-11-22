#version 330 core
in float c, n, depth;
uniform sampler1D pal;
out vec4 FragColor;
const float lighting=0.75;
void main() {
    int tex_length = textureSize(pal,0);
    if (c <0. || c>1.) discard;
    float col = (c*(tex_length - 1.0)+0.5)/tex_length;
    FragColor =texture(pal, col)*(n+1.-lighting);
    gl_FragDepth =depth;
}
