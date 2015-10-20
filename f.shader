varying float z,c;
varying vec3 n;
varying vec3 co;
uniform sampler1D pal;
uniform float tex_length;
uniform vec3 vmax;
uniform vec3 vmin;
void main() {
    bool clip =true;
    for(int i =0; i<3 ;i++){
        clip = clip && (co[i]-vmin[i]>=0.);
        clip = clip && (vmax[i]-co[i]>=0.);
    }
    if(!clip) discard;
    if (c <0. || c>1.) discard;
    //if (c ==0.) discard;
    float col = (c*(tex_length - 1.0)+0.5)/tex_length;
    gl_FragColor = texture1D(pal, col)*(abs(dot(vec3(0.0,0.0,0.99),n))+0.01);
    //vec4 co = texture1D(pal, col)*(abs(dot(vec3(0.0,0.0,0.99),n))+0.01);
    //gl_FragColor = vec4(co.x,co.y,co.z, 0.5);
    //gl_FragColor = vec4(1.0,c,1.0-c,1.0)*(abs(dot(vec3(0.0,0.0,0.99),n))+0.01);
    gl_FragDepth =z*0.5*gl_DepthRange.diff +0.5*(gl_DepthRange.far+gl_DepthRange.near);
}
