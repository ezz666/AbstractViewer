#include "plottable.hpp"
//--------------------------------------------------------------------------------
// Vertex array object (VAO)
//--------------------------------------------------------------------------------
VertexArray::VertexArray(){
	glGenVertexArrays(1, &AO);
	glGenBuffers(1, &EBO);
	this->bind();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    this->release();
    checkOpenGLerror();
}
//--------------------------------------------------------------------------------
VertexArray::~VertexArray(){
	for(auto && BO : BOs) if(BO) glDeleteBuffers(1,&BO);
	if (EBO) glDeleteBuffers(1,&EBO);
	if (AO) glDeleteVertexArrays(1,&AO);
}
//--------------------------------------------------------------------------------
void VertexArray::add_buffer(){
	BOs.emplace_back();
	attrs.emplace_back();
	this->bind();
	glGenBuffers(1, &BOs.back());
	this->release();
    checkOpenGLerror();
}
//--------------------------------------------------------------------------------
void VertexArray::bind(){
	glBindVertexArray(AO);
}
//--------------------------------------------------------------------------------
void VertexArray::release(){
	glBindVertexArray(0);
}
//--------------------------------------------------------------------------------
void VertexArray::load_data(int pos, int size, const void * data){
	this->bind();
	glBindBuffer(GL_ARRAY_BUFFER, BOs[pos]);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	this->release();
    checkOpenGLerror();
}
//--------------------------------------------------------------------------------
GLuint & VertexArray::get_BO(int pos){
	return BOs[pos];
}
//--------------------------------------------------------------------------------
void VertexArray::load_indices(int size, const void * data){
	this->bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW );
	this->release();
    checkOpenGLerror();
}
//--------------------------------------------------------------------------------
GLint & VertexArray::get_attr(int pos){
	return attrs[pos];
}
//--------------------------------------------------------------------------------
void VertexArray::enable_attr(int pos, int num, GLenum type){
	if (attrs[pos]!=-1){
		glEnableVertexAttribArray(attrs[pos]);
		glBindBuffer(GL_ARRAY_BUFFER, BOs[pos]);
		glVertexAttribPointer(attrs[pos], num, type, GL_FALSE, 0, 0);
	}
    checkOpenGLerror();
}
//------------------------------------------------------------------------------
// Plottable
//------------------------------------------------------------------------------
//Plottable::Plottable(){
//    //glGenVertexArrays(1, &VAO);
//    //glBindVertexArray(VAO);
//    //glGenBuffers(4,&VBO[0]);
//    //glBindVertexArray(0);
//};
////------------------------------------------------------------------------------
//Plottable::~Plottable(){
//    if (VBO[0]){
//        //std::cout<<"delete buffers"<<std::endl;
//        glDeleteBuffers(4,VBO);
//    }
//    if (VAO) glDeleteVertexArrays(1, &VAO);
//}
//------------------------------------------------------------------------------
// Axis
//------------------------------------------------------------------------------
void Axis::load_on_device(){
    int NTR = 3;
    //glGenVertexArrays(1, &VAO);
    //glBindVertexArray(VAO);
    //glGenBuffers(4,&VBO[0]);
    const glm::vec3 tri[6] = {  glm::vec3(0,0,0), glm::vec3(1,0,0), glm::vec3(0,0,0),
        glm::vec3(0,1,0), glm::vec3(0,0,0), glm::vec3(0,0,1) };
    const glm::vec3 norm[6] = {  glm::vec3(1,1,1), glm::vec3(1,1,1), glm::vec3(1,1,1),
        glm::vec3(1,1,1), glm::vec3(1,1,1), glm::vec3(1,1,1) };
    float ap[6] = {0.f,0.f,1.f,1.f,2.f,2.f};
    unsigned int indices[6] = {0,1, 2,3, 4,5};//Мы не можем сделать по-другому : цвет различается значит все индексы разные
    VAO.load_data(POS, sizeof(glm::vec3) * NTR*2, tri);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO[POS]);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * NTR*2, tri, GL_STATIC_DRAW);
    VAO.load_data(NRM, sizeof(glm::vec3) * NTR*2, norm);
    VAO.load_data(CLR, sizeof(float) * NTR*2, ap);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO[NRM]);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * NTR*2, norm, GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO[CLR]);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * NTR*2, ap, GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[IND]);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned int), indices, GL_STATIC_DRAW );
    VAO.load_indices(6*sizeof(unsigned int), indices);
    VAO.release();
}
//------------------------------------------------------------------------------
void Axis::AttachToShader(ShaderProg * spr) {
    //glBindVertexArray(VAO);
    VAO.bind();
    spr->AttachUniform(unif_minmax,"minmaxmul");
    spr->AttachAttr(VAO.get_attr(NRM),"normal");
    spr->AttachAttr(VAO.get_attr(CLR),"color");
    spr->AttachAttr(VAO.get_attr(POS),"coord");
    if (unif_minmax != -1) glUniform3f(unif_minmax, 0.f, 2.f, 0.5f);
    VAO.enable_attr(POS, 3, GL_FLOAT);
    VAO.enable_attr(NRM, 3, GL_FLOAT);
    VAO.enable_attr(CLR, 1, GL_FLOAT);
    //if (vattr != -1){
    //    glEnableVertexAttribArray(vattr);
    //    glBindBuffer(GL_ARRAY_BUFFER, VBO[POS]);
    //    glVertexAttribPointer(vattr, 3, GL_FLOAT, GL_FALSE, 0, 0);
    //}
    //if (nattr != -1){
    //    glEnableVertexAttribArray(nattr);
    //    glBindBuffer(GL_ARRAY_BUFFER, VBO[NRM]);
    //    glVertexAttribPointer(nattr, 3, GL_FLOAT, GL_FALSE, 0, 0);
    //}
    //if (cattr != -1) {
    //    glEnableVertexAttribArray(cattr);
    //    glBindBuffer(GL_ARRAY_BUFFER, VBO[CLR]);
    //    glVertexAttribPointer(cattr, 1, GL_FLOAT, GL_FALSE, 0, 0);
    //}
    VAO.release();
    //glBindVertexArray(0);
}
//------------------------------------------------------------------------------
void Axis::plot(ShaderProg * spr) {
    int Ntr = 3;
    AttachToShader(spr);
    //glBindVertexArray(VAO);
    VAO.bind();
    //glDrawElements(GL_LINES,Ntr*2, GL_UNSIGNED_INT, (void *)0);
    glDrawElementsInstanced(GL_LINES,Ntr*2, GL_UNSIGNED_INT, (void *)0,1);
    //glBindVertexArray(0);
    VAO.release();
    //glDisableVertexAttribArray(vattr);
    //glDisableVertexAttribArray(cattr);
    //glDisableVertexAttribArray(nattr);
    //glBindBuffer(GL_ARRAY_BUFFER,0);
}
//------------------------------------------------------------------------------
// SurfTemplate
//------------------------------------------------------------------------------
void SurfTemplate::plot(ShaderProg * spr) {
    int Ntr = select.size();
    attach_shader(spr); // Возможно оверкил делать это каждый раз
    //glBindVertexArray(VAO);
    VAO.bind();
    //glDrawElements(GL_TRIANGLES, Ntr, GL_UNSIGNED_INT, (void *)0);
    glDrawElementsInstanced(GL_TRIANGLES, Ntr, GL_UNSIGNED_INT, (void *)0, 1);
    //glBindVertexArray(0);
    VAO.release();
    //glDisableVertexAttribArray(vattr);
    //glDisableVertexAttribArray(cattr);
    //glDisableVertexAttribArray(nattr);
    //glBindBuffer(GL_ARRAY_BUFFER,0);
}
//------------------------------------------------------------------------------
float SurfTemplate::min(){
    return minmaxmul[0];
}
//------------------------------------------------------------------------------
float SurfTemplate::max(){
    return minmaxmul[1];
}
//------------------------------------------------------------------------------
void SurfTemplate::rangemove(float shift,bool l){
    float &min = minmaxmul[0];
    float &max = minmaxmul[1];
    float len = max - min;
    if (l) min -= shift * len;
    else max += shift * len;
}
//------------------------------------------------------------------------------
void SurfTemplate::extendrange(float factor){
    float &min = minmaxmul[0];
    float &max = minmaxmul[1];
    float &mul = minmaxmul[2];
    float len_2 = (max - min)*0.5f,
          center = (min + max)* 0.5f;
    min = center - len_2*factor;
    max = center + len_2*factor;
    mul = (min != max)? 1.f/(max  - min): 1.f;
}
//------------------------------------------------------------------------------
void SurfTemplate::refill_select(){
    int NTR = get_cells_size();
    select.clear();
    select.resize(NTR*3);
    for(unsigned int i=0; i < select.size(); i++) select[i] = i;
}
//------------------------------------------------------------------------------
// PaletteBox
//------------------------------------------------------------------------------
PaletteBox::PaletteBox(Texture * _tex, const glm::ivec2 _min, const glm::ivec2 _max):Plottable(),
    tex(_tex), xymin(_min), xymax(_max), vertical(false), unif_minmax(-1){
    VAO.add_buffer();
    VAO.add_buffer();
    checkOpenGLerror();
}
//------------------------------------------------------------------------------
void PaletteBox::load_on_device(){
    const glm::vec3 tri[4] = {  glm::vec3(xymin.x,xymin.y,0), glm::vec3(xymax.x,xymin.y,0),
	glm::vec3(xymin.x,xymax.y,0), glm::vec3(xymax.x,xymax.y,0)};

    float ap_v[4] = {0.f,0.f,1.f,1.f};
    float ap_h[4] = {0.f,1.f,0.f,1.f};
    unsigned int indices[6] = {0,1, 2,1, 2,3};//Мы не можем сделать по-другому : цвет различается значит все индексы разные
    VAO.load_data(POS, sizeof(glm::vec3) * 4, tri);
    VAO.load_data(CLR, sizeof(float) * 4, vertical?ap_v:ap_h);
    VAO.load_indices(6*sizeof(unsigned int), indices);
    VAO.release();
    checkOpenGLerror();
}
//------------------------------------------------------------------------------
void PaletteBox::AttachToShader(ShaderProg * spr) {
    //glBindVertexArray(VAO);
    VAO.bind();
    spr->AttachUniform(unif_minmax,"minmaxmul");
    //spr->AttachAttr(VAO.get_attr(NRM),"normal");
    spr->AttachAttr(VAO.get_attr(CLR),"color");
    spr->AttachAttr(VAO.get_attr(POS),"coord");
    if (unif_minmax != -1) glUniform3f(unif_minmax, 0.f, 1.f, 1.0f);
    VAO.enable_attr(POS, 3, GL_FLOAT);
    VAO.enable_attr(CLR, 1, GL_FLOAT);
    VAO.release();
    checkOpenGLerror();
}
//------------------------------------------------------------------------------
void PaletteBox::set_texture(Texture * _tex) {
    this->tex = _tex;
}
//------------------------------------------------------------------------------
Texture * PaletteBox::get_texture() {
    return tex;
}
//------------------------------------------------------------------------------
void PaletteBox::plot(ShaderProg * spr) {
    //int Ntr = 3;
    AttachToShader(spr);
    this->tex->use_texture(spr, "pal");
    //glBindVertexArray(VAO);
    VAO.bind();
    //glDrawElements(GL_LINES,Ntr*2, GL_UNSIGNED_INT, (void *)0);
    glDrawElementsInstanced(GL_TRIANGLES,6, GL_UNSIGNED_INT, (void *)0,1);
    //glBindVertexArray(0);
    VAO.release();
    checkOpenGLerror();
    //glDisableVertexAttribArray(vattr);
    //glDisableVertexAttribArray(cattr);
    //glDisableVertexAttribArray(nattr);
    //glBindBuffer(GL_ARRAY_BUFFER,0);
}
//------------------------------------------------------------------------------
bool PaletteBox::get_vertical(){
    return vertical;
}
//------------------------------------------------------------------------------
void PaletteBox::set_vertical(bool new_vert){
    vertical = new_vert;
}
//------------------------------------------------------------------------------
void PaletteBox::switch_vertical(){
    vertical ^=1;
}
//------------------------------------------------------------------------------
void PaletteBox::set_xyminmax(const int * newxymin, const int* newxymax){
    xymin = glm::ivec2(newxymin[0], newxymin[1]);
    xymax = glm::ivec2(newxymax[0], newxymax[1]);
}
//------------------------------------------------------------------------------
void PaletteBox::get_xyminmax(int * newxymin, int* newxymax){
    newxymin[0] = xymin.x;
    newxymin[1] = xymin.y;
    newxymax[0] = xymax.x;
    newxymax[1] = xymax.y;
}
//------------------------------------------------------------------------------
// PaletteAlphaControl
//------------------------------------------------------------------------------
PaletteAlphaControl::PaletteAlphaControl(PaletteBox * _tex):
    Plottable(), unif_clr(-1), clr(.5f), pal(_tex){
  VAO.add_buffer();
}
//------------------------------------------------------------------------------
void PaletteAlphaControl::set_alpha(int color_num, float new_alpha){
  pal->get_texture()->set_alpha(color_num, new_alpha);
  load_on_device();
}
//------------------------------------------------------------------------------
void PaletteAlphaControl::load_on_device(){
  Texture * tex = pal->get_texture();
  glm::ivec2 xymin, xymax;
  pal->get_xyminmax( reinterpret_cast<int *>(&xymin), reinterpret_cast<int *>(&xymax));
  line.reset(new glm::vec3 [(tex->get_length())]);
  std::unique_ptr< unsigned int []> indices(new unsigned int [tex->get_length()]);
  //float alpha = 0.5f * ((*tex)[3] + (*tex)[tex->get_length()*4 -1]);
  //if (pal->get_vertical()){
  //  line[0] = glm::vec3(xymax.x*alpha+ xymin.x*(1-alpha), xymin.y,0.1);
  //  line[tex->get_length() + 1] = glm::vec3(alpha*xymax.x+ xymin.x*(1-alpha), xymax.y, 0.1);
  //} else {
  //  line[0] = glm::vec3(xymin.x, xymin.y*(1-alpha)+ xymax.y*alpha, 0.1);
  //  line[tex->get_length() + 1] = glm::vec3(xymax.x, xymin.y*(1-alpha)+ xymax.y*alpha, 0.1);
  //}
  //indices[0] = 0;
  //indices[tex->get_length() +1] = tex->get_length() +1;
  float alpha;
  for(int i=0; i< tex->get_length(); ++i){
    float prop = float(i)/(tex->get_length()-1);
    alpha = (*tex)[4*i+3];
    if (pal->get_vertical()){
      float ycoord = xymax.y*prop + xymin.y*(1-prop);
      line[i] = glm::vec3(xymax.x*alpha + xymin.x*(1-alpha), ycoord, 0.1);
    } else {
      float xcoord = xymax.x*prop + xymin.x*(1-prop);
      line[i] = glm::vec3(xcoord,xymax.y*alpha + xymin.y*(1-alpha) , 0.1);
    }
    indices[i] = i;
  }
  VAO.load_data(POS, sizeof(glm::vec3) * (tex->get_length()), line.get());
  //std::cout<<"points:"<<std::endl;
  //for(int i=0; i<3*(tex->get_length());i++){
  //  std::cout<<line[i/3][i%3];
  //  if(i%3==2) std::cout<<std::endl;
  //  else std::cout<<",";
  //}
  //std::cout<<std::endl;
  //std::cout<<"indices"<<std::endl;
  //for(int i=0; i<(tex->get_length());i++){
  //  std::cout<<indices[i];
  //  std::cout<<" ";
  //}
  std::cout<<std::endl;
  VAO.load_indices((tex->get_length())*sizeof(unsigned int), indices.get());
  VAO.release();
  checkOpenGLerror();
}
//------------------------------------------------------------------------------
void PaletteAlphaControl::plot(ShaderProg * spr){
  AttachToShader(spr);
  VAO.bind();
  glDrawElementsInstanced(GL_LINE_STRIP, (pal->get_texture()->get_length()), GL_UNSIGNED_INT, (void*) 0, 1);
  VAO.release();
}
//------------------------------------------------------------------------------
void PaletteAlphaControl::AttachToShader(ShaderProg * spr) {
  VAO.bind();
  spr->AttachUniform(unif_clr,"clr");
  spr->AttachAttr(VAO.get_attr(POS),"coord");
  if (unif_clr != -1) glUniform3f(unif_clr, clr[0], clr[1], clr[2]);
  //std::cout<<"#"<<clr[0]<<", "<<clr[1]<<", "<<clr[2]<<std::endl;
  VAO.enable_attr(POS, 3, GL_FLOAT);
  VAO.release();
}
//------------------------------------------------------------------------------
// VolumeTemplate
//------------------------------------------------------------------------------
VolumeTemplate::VolumeTemplate():Plottable(), data(new Texture3D(nullptr, 0, 0, 0, GL_TEXTURE1, GL_RGB, GL_RGB, false)),//, GL_RGB, GL_RGB, false),
    unif_step(-1), unif_size(-1), unif_raystep(-1),
    unif_density(-1), unif_brightness(-1),unif_opacity(-1), unif_ftype(-1),
    unif_origin(-1), unif_cubemin(-1), unif_cubemax(-1),
    raystep(0),density(0.5),brightness(1.0),opacity(0.95), ftype(0){
      VAO.add_buffer(); //actualy this one wil contain only the vertices of a cube;)
    }
void VolumeTemplate::load_on_device(){
  reload_cube();
  reload_texture();
}
void VolumeTemplate::reload_cube(){
  int NTR=12;
  float &xmin = cubemin[0], &xmax = cubemax[0],
    &ymin = cubemin[1], &ymax = cubemax[1],
    &zmin = cubemin[2], &zmax = cubemax[2];
  const glm::vec3 cube[8] = {  glm::vec3(xmin,ymin,zmin), glm::vec3(xmax,ymin,zmin), glm::vec3(xmin,ymax,zmin), glm::vec3(xmin,ymin,zmax),
    glm::vec3(xmax,ymax,zmin), glm::vec3(xmin,ymax,zmax), glm::vec3(xmax,ymin,zmax), glm::vec3(xmax,ymax,zmax)};
  unsigned int indices[36] = {
    0, 2, 4,
    0, 4, 1,
    0, 3, 6,
    0, 6, 1,
    0, 5, 2,
    0, 3, 5,
    1, 6, 7,
    1, 7, 4,
    2, 5, 7,
    2, 7, 4,
    3, 5, 7,
    3, 7, 6
  };
  VAO.load_data(POS, sizeof(glm::vec3) * 8, cube);
  VAO.load_indices(NTR*3*sizeof(unsigned int), indices);
  VAO.release();
}
void VolumeTemplate::set_density(float dens){
  density = dens;
}
float VolumeTemplate::get_density(){
  return density;
}
void VolumeTemplate::set_brightness(float bright){
  brightness = bright;
}
float VolumeTemplate::get_brightness(){
  return brightness;
}
void VolumeTemplate::set_opacity(float op){
  opacity = op;
}
float VolumeTemplate::get_opacity(){
  return opacity;
}
void VolumeTemplate::set_raystep(float rs){
  raystep = rs;
}
float VolumeTemplate::get_raystep(){
  return raystep;
}
void VolumeTemplate::set_ftype(int ft){
  ftype = ft;
}
int VolumeTemplate::get_ftype(){
  return ftype;
}
void VolumeTemplate::get_auto_box(float * bb_min, float * bb_max){
  bbox(bb_min, bb_max);
  //auto lab_steps = trans::storage_scales(UA->CG->get_steps());
  for(int i=0; i<3; i++){
    bb_min [i] -= 1.f;//float(lab_steps[i]);
    bb_max [i] += 1.f;//float(lab_steps[i]);
  }
}
void VolumeTemplate::attach_shader(ShaderProg * spr){
  //glBindVertexArray(VAO);
  this->VAO.bind();
  //spr->AttachUniform(unif_bmin,"bmin");
  //spr->AttachUniform(unif_bmax,"bmax");
  spr->AttachUniform(unif_cminmaxmul,"minmaxmul");
  spr->AttachUniform(unif_step,"rstep");
  spr->AttachUniform(unif_size,"size");
  spr->AttachUniform(unif_raystep,"raystep");
  spr->AttachUniform(unif_density,"density");
  spr->AttachUniform(unif_brightness,"brightness");
  spr->AttachUniform(unif_opacity,"opacity");
  spr->AttachUniform(unif_ftype,"ftype");
  spr->AttachUniform(unif_origin,"origin");
  spr->AttachUniform(unif_cubemin,"cubemin");
  spr->AttachUniform(unif_cubemax,"cubemax");
  spr->AttachAttr(this->VAO.get_attr(POS),"coord");
  //auto size = get_sizes();
  //auto steps = get_steps();
  float steps[3];
  int size[3];
  get_steps(steps[0], steps[1], steps[2]);
  float rstep[3] {1.f/steps[0], 1.f/steps[1], 1.f/steps[2]};
  get_sizes(size[0], size[1], size[2]);
  //if (unif_bmin != -1) glUniform3f(unif_bmin, bmin[0], bmin[1], bmin[2]);
  //if (unif_bmax != -1) glUniform3f(unif_bmax, bmax[0], bmax[1], bmax[2]);
  if (unif_cminmaxmul != -1) glUniform3f(unif_cminmaxmul, cminmaxmul[0], cminmaxmul[1], cminmaxmul[2]);
  if (unif_step != -1) glUniform3f(unif_step, rstep[0], rstep[1], rstep[2]);
  if (unif_size != -1) glUniform3i(unif_size, size[0], size[1], size[2]);
  if (unif_raystep != -1) glUniform1f(unif_raystep, raystep);
  if (unif_density != -1) glUniform1f(unif_density, density);
  if (unif_brightness != -1) glUniform1f(unif_brightness, brightness);
  if (unif_opacity != -1) glUniform1f(unif_opacity, opacity);
  if (unif_ftype != -1) glUniform1i(unif_ftype, ftype);
  if (unif_origin != -1){
    float org[3];
    get_origin(org[0], org[1], org[2]);
    glUniform3f(unif_origin, org[0],org[1],org[2]);
  }
  if (unif_cubemin != -1) glUniform3fv(unif_cubemin,1, cubemin);
  if (unif_cubemax != -1) glUniform3fv(unif_cubemax,1, cubemax);
  this->VAO.enable_attr(POS, 3, GL_FLOAT);
  this->VAO.release();
}
void VolumeTemplate::plot(ShaderProg * spr){
  attach_shader(spr);
  auto NTR=12;
  data->use_texture(spr,"data");
  VAO.bind();
  //glDrawElements(GL_TRIANGLES, Ntr, GL_UNSIGNED_INT, (void *)0);
  glDrawElementsInstanced(GL_TRIANGLES, NTR*3, GL_UNSIGNED_INT, (void *)0, 1);
  //glBindVertexArray(0);
  VAO.release();
}
//------------------------------------------------------------------------------
void VolumeTemplate::rangemove(float shift,bool l){
  float &min = cminmaxmul[0];
  float &max = cminmaxmul[1];
  float len = max - min;
  if (l) min -= shift * len;
  else max += shift * len;
  cminmaxmul[2] = (min < max)? 1.f/(max  - min): 1.f;
}
void VolumeTemplate::extendrange(float factor){
  float &min = cminmaxmul[0];
  float &max = cminmaxmul[1];
  float &mul = cminmaxmul[2];
  float len_2 = (max - min)*0.5f,
        center = (min + max)* 0.5f;
  min = center - len_2*factor;
  max = center + len_2*factor;
  mul = (min < max)? 1.f/(max  - min): 1.f;
}
//------------------------------------------------------------------------------
void VolumeTemplate::set_range(float lower, float upper){
  cminmaxmul[0] = lower;
  cminmaxmul[1] = upper;
  cminmaxmul[2] = (lower < upper)? 1.f/(upper  - lower): 1.f;
}
//------------------------------------------------------------------------------
float VolumeTemplate::min(){
  return cminmaxmul[0];
}
//------------------------------------------------------------------------------
float VolumeTemplate::max(){
  return cminmaxmul[1];
}
//------------------------------------------------------------------------------
void VolumeTemplate::adjust_cube(const Viewer3D & V){
  get_auto_box(cubemin, cubemax);
  auto vmin = V.get_vmin();
  auto vmax = V.get_vmax();
  for(int i=0; i<3; i++){
    cubemin[i] = std::max(cubemin[i], vmin[i]);
    cubemax[i] = std::min(cubemax[i], vmax[i]);
  }
  reload_cube();
}
//------------------------------------------------------------------------------
float VolumeTemplate::get_ranges(int i){
  if (i<3)
    return cubemin[i];
  else return cubemax[i%3];
}
//------------------------------------------------------------------------------
