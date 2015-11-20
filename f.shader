#version 330 core
in float z,c;
in float n;
in vec3 co;
const float lighting=0.75;
uniform sampler1D pal;
out vec4 FragColor;
void main() {
    int tex_length = textureSize(pal,0);
    if (c <0. || c>1.) discard;
    float col = (c*(tex_length - 1.0)+0.5)/tex_length;
    FragColor =texture(pal, col)*(n+1.-lighting);
    gl_FragDepth =(z*1.)*.5 ;//0.5*gl_DepthRange.diff +0.5*(gl_DepthRange.far+gl_DepthRange.near);
}
