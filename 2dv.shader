#version 330 core
in vec3 coord;
in float color;
uniform mat4 proj;
uniform vec3 vmin, vmax, minmaxmul;
uniform vec2 viewport;
uniform float scale;
out float c;
void main() {
    c = (color-minmaxmul.x)*minmaxmul.z;
    float l = min(viewport.x, viewport.y);
    //vec4 res = MVP*(vec4(coord,1.0));
    //vec3 center = (vmin +vmax)*0.5;
    //res.z = ((MVP*vec4(center ,1.)).z-res.z)*scale/distance(vmax,vmin)*2;// in [-1:1]
    gl_Position = proj*(vec4(coord,1.0));
    //res;
}

