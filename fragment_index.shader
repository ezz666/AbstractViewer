#version 330 core
in float c;
flat in ivec3 intcolor;
out ivec4 FragColor;
uniform sampler1D pal;
void main() {
    if (c <0. || c>1.) discard;
    FragColor = ivec4(intcolor,128);
    gl_FragDepth =gl_FragCoord.z ;
}
