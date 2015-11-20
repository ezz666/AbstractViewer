#version 330 core
in vec3 coord;
in vec3 normal;
in float color;
out float gl_ClipDistance[6];
uniform vec3 vmin;
uniform vec3 vmax;
uniform mat4 MVP;
uniform mat4 itMVP;
uniform vec3 minmaxmul;
out float z, c;
out float n;
out vec3 co;
const float lighting=0.75;
void main() {
    const float camera_light = 0.95;
    const float vertical_light = 0.05;
    vec4 res = MVP*vec4(coord,1.0);
    for(int i =0; i<3; i++){
        gl_ClipDistance[i]=coord[i]- vmin[i];
        gl_ClipDistance[i+3]=vmax[i] -coord[i];
    }
    co = coord;
    vec3 center = (vmin+vmax)*0.5;
    z = (dot((coord -center),normalize(vec4(0.,0.,1.,0.0)*itMVP).xyz))/(distance(vmax,vmin));
    n = lighting*abs(dot(normal,(camera_light*normalize(vec4(0.0,0.0,1.0,0.0)*itMVP).xyz)+
                (normal.x+normal.y+normal.z)/3.*vertical_light))/(camera_light+vertical_light);
    c = (color-minmaxmul.x)/(minmaxmul.y - minmaxmul.x);
    gl_Position = res;
}
