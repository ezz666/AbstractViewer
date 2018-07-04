#version 330 core
uniform vec2 viewport;
uniform vec3 clr;
out vec4 FragColor;
void main() {
    //float col = ((gl_FragCoord.z+0.5)*(tex_length - 1.0)+0.5)/tex_length;
    FragColor =vec4(clr, 1);
    // Using gl_FragCoord.z as depth
    gl_FragDepth =gl_FragCoord.z;
}
