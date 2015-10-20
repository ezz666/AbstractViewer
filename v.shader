attribute vec3 coord;
attribute vec3 normal;
attribute float color;
//uniform vec3 vmin;
//uniform vec3 vmax;
uniform mat4 MVP;
uniform vec3 minmaxmul;
varying float z,c;
varying vec3 n;
varying vec3 co;
void main() {
    vec4 res = MVP*vec4(coord,1.0);
    //for(int i =0; i<3; i++){
    //    gl_ClipDistance[i]=coord[i]- vmin[i];
    //    gl_ClipDistance[i+3]=vmax[i] -coord[i];
    //}
    co = coord;
    z = res.z;
    n = normal;
    c = (color-minmaxmul.x)/(minmaxmul.y - minmaxmul.x);
    gl_Position = res;
}
