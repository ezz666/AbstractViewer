#version 330 core
in vec3 coords;
in float z,c;
flat in ivec3 intcolor;
out vec3 FragColor;
uniform sampler1D pal;
void main() {
    if (c <0. || c>1.) discard;
    FragColor = vec3(intcolor)/256.;
    gl_FragDepth =z*0.5*gl_DepthRange.diff +0.5*(gl_DepthRange.far+gl_DepthRange.near);
}
