#version 330 core
in vec3 coord;
in vec3 normal;
in float color;
uniform mat4 MVP;
uniform mat4 itMVP;
uniform vec3 minmaxmul;
uniform vec3 vmax;
uniform vec3 vmin;
out float gl_ClipDistance[6];
flat out ivec3 intcolor;
out vec3 coords;
out float z,c;
void main() {
    for(int i =0; i<3; i++){
        gl_ClipDistance[i]=coord[i]- vmin[i];
        gl_ClipDistance[i+3]=vmax[i] -coord[i];
    }
    vec4 res = MVP*vec4(coord,1.0);
    coords = coord;
    int r = (gl_VertexID & (255));
    int g = (gl_VertexID >> 8 ) & (255);
    int b = (gl_VertexID >> 16) & (65535);
    z = res.z;
    c = (color-minmaxmul.x)/(minmaxmul.y - minmaxmul.x);
    intcolor = ivec3(r,g,b);
    gl_Position = res;
}
