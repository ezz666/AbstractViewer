#version 330 core
in float c;
flat in ivec3 intcolor;
out vec3 FragColor;
uniform sampler1D pal;
void main() {
    if (c <0. || c>1.) discard;
    FragColor = vec3(intcolor)/256.;
    gl_FragDepth =gl_FragCoord.z ;
}
