#include <fstream>
#include <iostream>
#include <iomanip>
//#include <string>
#include <cstring>
#include <chrono>
#include "viewer_template.hpp"
#include <glm/ext.hpp>
//#include "frontview.hpp"
const double much_enough = 1e3;
//------------------------------------------------------------------------------
void checkOpenGLerror()
{
    GLenum errCode;
    if((errCode=glGetError()) != GL_NO_ERROR)
        std::cout << "OpenGl error! - " << gluErrorString(errCode)<<std::endl;
}
//Для простоты это будет октаэдр
void Viewer::create_model(){
    //free_BO();
    //if (sur){delete sur;};
    //sur = new Surface<2>();
    //sur->load(*inp);
    //sur->load_on_device(1);
    //checkOpenGLerror();
    //sur->set_minmax();
    //delete [] normals;
    checkOpenGLerror();
}
//--------------------------------------------------------------------------------
//void Viewer::free_BO(){
//    checkOpenGLerror();
//    printf("before free BO\n");
//    printf("after free BO\n");
//}
////--------------------------------------------------------------------------------
//void Viewer::set_pal(){
//    int length = 4;
    //tex= new(tex) Texture(pal,length);
//    printf("before set pal\n");
//    checkOpenGLerror();
//    printf("in set pal\n");
//    int length = 4;
//    printf("before Active set pal\n");
//    //glActiveTexture(GL_TEXTURE0);
//    glGenTextures(1, &textureID);
//    glBindTexture(GL_TEXTURE_1D, textureID);
//    glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//    glTexImage1D(GL_TEXTURE_1D, 0, 3, length, 0, GL_RGB, GL_FLOAT, pal);
//    checkOpenGLerror();
//    glBindTexture(GL_TEXTURE_1D, 0);
//    checkOpenGLerror();
//    printf("after set pal\n");
//}
//--------------------------------------------------------------------------------
void Viewer::togglewire() {
    set_wire(!wire);
}
void Viewer::set_wire(bool tf){
    wire = tf;
    glPolygonMode(GL_FRONT_AND_BACK, wire ? GL_LINE : GL_FILL);
}
//--------------------------------------------------------------------------------
//Повороты
void Viewer::mouse_click(int button, int state, int x, int y){
    if (button ==GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        tr0.x = ((float)x)/min_size*2;
        tr0.y = ((float)y)/min_size*2;
        tr0.z = tr0.z;
        left_click = true;
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
       // glm::quat rot_tmp;
      //  if (right_click){
            glm::quat quatx = glm::angleAxis(rotx*Sens, glm::vec3(1.f,0.f,0.f));
            glm::quat rot_tmp = glm::angleAxis(roty*Sens, quatx * glm::vec3(0.f,1.f,0.f) /* quatx*/) *
                quatx ;
    //    } else rot_tmp = glm::quat(0.f,0.f,0.f,1.f);
        pos =  tr*scale - glm::vec3(0.f,0.f,scale) +
                rot_tmp*(pos+glm::vec3(0.f,0.f,scale));
        tr.x=0.f; tr.y =0.f; tr.z =0.f;
        tr = glm::vec3(0.f);
        left_click = false;
        calc_mvp();
    }
    if (button ==GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
        ox = ((float)x)/min_size*2;
        oy = ((float)y)/min_size*2;
        oz = 0.f;
        right_click = true;
    }
    if (button ==GLUT_RIGHT_BUTTON && state == GLUT_UP){
        glm::quat quatx = glm::angleAxis(rotx*Sens , glm::vec3(1,0,0));
        glm::quat rot_tmp = glm::angleAxis(roty*Sens, quatx * glm::vec3(0.f,1.f,0.f) /* quatx*/) *
                quatx ;
        orient = rot_tmp * orient;
        pos = rot_tmp * ( pos+glm::vec3(0.f,0.f,scale))- glm::vec3(0.f,0.f,scale);
        //Учесть трансформацию вектора
        rotx = 0.f; roty = 0.f; rotz = 0.f;
        ox = 0.f; oy = 0.f; oz = 0.f;
        right_click = false;
        calc_mvp();
    }
    //if (button ==GLUT_MIDDLE_BUTTON&& state == GLUT_DOWN){
    //    autoscale();
    //    reshape(width,height);
    //}
    if (button ==3&& state == GLUT_DOWN){
        pos += glm::vec3(0.f,0.f,scale*0.1);
        scale *= 0.9;
        reshape(width,height);
    } else if (button == 4&& state == GLUT_DOWN){
        pos -= glm::vec3(0.f,0.f,scale*0.1);
        scale /=0.9;
        reshape(width,height);
    }
}
//--------------------------------------------------------------------------------
void Viewer::drag(int x, int y){
    if (left_click){
        float px = ((float)x)/min_size*2;
        float py = ((float)y)/min_size*2;
        tr.x = px -tr0.x;
        tr.y = -py+tr0.y;
        calc_mvp();
    }
    if (right_click){
        float px = ((float)x)/min_size*2;
        float py = ((float)y)/min_size*2;
        //rotx=0;
        rotz = 0.;
        rotx = glm::degrees((py - oy) * 4*M_PI/180.f);
        roty = glm::degrees((px - ox) * 4*M_PI/180.f);
        //rotate(ox,py,px);
        calc_mvp();
    }
    glutPostRedisplay();
}
void Viewer::GL_init(){
    checkOpenGLerror();
    printf("init begin\n");
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    //glEnable( GL_BLEND );
    //glBlendFunc(GL_ONE, GL_ONE);
    glewInit();
    //glEnable(GL_CLIP_DISTANCE0);
    //glEnable(GL_CLIP_DISTANCE1);
    //glEnable(GL_CLIP_DISTANCE2);
    //glEnable(GL_CLIP_DISTANCE3);
    //glEnable(GL_CLIP_DISTANCE4);
    //glEnable(GL_CLIP_DISTANCE5);
    checkOpenGLerror();
    printf("init end\n");
}
//--------------------------------------------------------------------------------
//i==0,1,2 — min x,y,z
//i=3,4,5 — max ,x,y,z
void Viewer::clip_plane_move(float shift, int num){
    if (num<3) min[num] += shift*scale;
    else max[num%3] += shift*scale;
}
//--------------------------------------------------------------------------------
Viewer::Viewer(){
    //Может это вообще перенести в python?
    //это разумно вообще
    //с другой стороны концепция один вьюер — одно окно,
    //никуда не делась
    //с третьей стороны если привязку клавиш делать из питона все равно
    //эффективной разницы не будет.
    //Итого: Это все переносим в python
    //класс нужен для хранения всяких переменных и т.д.
    //в pythonего стоит ещё раз обернуть, если мы хотим
    //простой работы со всем получившимся
    //sur = NULL;
    rotx=0.0f; roty=0.0f; rotz=0.0f;
    ox = 0.f; oy = 0.f; oz = 0.f;
    scale = 1.f;
    pos = glm::vec3(0.f); pos.z = -scale;
    tr = glm::vec3(0.f); tr0 = glm::vec3(0.f);
    min=glm::vec3(0.); max=glm::vec3(0.);
    left_click = false; right_click = false;
    wire = false;
    width = 900; height = 900;
    orient =glm::quat(glm::vec3(0.,-M_PI_2,-M_PI_2));
        //glm::angleAxis(-(float)M_PI*0.5f*Sens,glm::vec3(1.f,0.f,0.f)) *
        //glm::angleAxis(-(float)M_PI*0.5f*Sens,glm::vec3(0.f,0.f,1.f));
    ort = glm::ortho(-1,1,-1,1,-6,6);
    axis_sw = false;
    //inp = new std::ifstream("fronts/F");
    //spr = new ShaderProg();
    //tex = new Texture();
   // spr->extern_load("v.shader","f.shader");
}
//--------------------------------------------------------------------------------
Viewer::~Viewer(){
    //if (inp) delete inp;
    //if (sur) delete sur;
    //if (tex) delete tex;
    //if (spr) delete spr;
    //free_BO();
    //glDeleteProgram(sprog);
    //exit(0);
}
//--------------------------------------------------------------------------------
glm::mat4 Viewer::calc_mvp(){
    glm::quat quatx = glm::angleAxis(rotx*Sens, glm::vec3(1.f,0.f,0.f));
    glm::quat rot_tmp = glm::angleAxis(roty*Sens, quatx * glm::vec3(0.f,1.f,0.f) /* quatx*/) *
                quatx ;
//    float scale_tmp = scale;//Костыль!
//    scale =0.0001f;
    if (! axis_sw){
        MVP = ort * glm::translate(tr*scale - glm::vec3(0.f,0.f,scale) +
                rot_tmp*(pos+glm::vec3(0.f,0.f,scale))) *
            glm::toMat4( rot_tmp * orient);
    } else {
        float m = (float)std::max(width, height);
        MVP = glm::ortho(-1.f*width/m,1.f*width/m,-1.f*height/m,1.f*height/m,-1.f,1.f) * glm::toMat4( rot_tmp * orient);
    }
    return MVP;
    //glUniformMatrix4fv( mvp_loc, 1, GL_FALSE, glm::value_ptr(MVP));
//    scale =scale_tmp;
}
//--------------------------------------------------------------------------------
void Viewer::set_view(float pitch, float yaw, float roll){
    orient = glm::quat(glm::vec3(pitch, yaw, roll));
}
void Viewer::get_view(float & pitch, float & yaw, float & roll) const{
    const glm::vec3 view = glm::eulerAngles(orient);
    pitch =view.x;
    yaw = view.y;
    roll = view.z;
    //std::cout<< (pitch ==0) <<" "<< (yaw == -1.5703080892562866f)<<std::endl;
    if (pitch ==0. && yaw ==-1.5703080892562866f&& roll ==0.) roll = -M_PI_2;
}
//--------------------------------------------------------------------------------
void Viewer::set_pos(float x, float y, float z){
    pos = glm::vec3(x,y,z);
}
////--------------------------------------------------------------------------------
void Viewer::get_pos(float* p){
    p[0] = pos.x;
    p[1] = pos.y;
    p[2] = pos.z;
}
//void Viewer::set_xrange(float lower, float upper){
//    min[0] = lower; max[0]= upper;
//}
////--------------------------------------------------------------------------------
//void Viewer::set_yrange(float lower, float upper){
//    min[1] = lower; max[1]= upper;
//}
////--------------------------------------------------------------------------------
//void Viewer::set_zrange(float lower, float upper){
//    min[2] = lower; max[2]= upper;
//}
//--------------------------------------------------------------------------------
void Viewer::display(){
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 //   set_pal();
 //   GLint samp = glGetUniformLocation(spr->sprog, "pal");
 //   glUniform1i(samp, 0);
    //spr->load_mvp(MVP);
    //tex->use_texture();
    //spr->render(sur,MVP);
    //glBindBuffer(GL_ARRAY_BUFFER,0);
    //glutSwapBuffers();
   // free_BO();
}
//--------------------------------------------------------------------------------
//void Viewer::idle(){
//}
//--------------------------------------------------------------------------------
void Viewer::reshape(int w, int h){
    glViewport(0, 0, w, h);
    width = w; height = h;
    int l = (w<h)? w:h;
    min_size = l;
    float max_size = std::max(w,h);
    ort = glm::ortho(-(GLfloat)w/(GLfloat)l*scale, (GLfloat)w/(GLfloat)l*scale,- (GLfloat)h/(GLfloat)l*scale,(GLfloat)h/(GLfloat)l*scale, (GLfloat)(-max_size*scale/min_size* much_enough),(GLfloat)(max_size*scale/min_size+scale* much_enough));
    calc_mvp();
    glutPostRedisplay();
}
//--------------------------------------------------------------------------------
//void Viewer::rescale(float mult){
//    if (mult*scale !=0 ) scale *= mult;
//}
//--------------------------------------------------------------------------------
void Viewer::axis_switch(){
    int sf = axis_sw?1:10;
    glViewport(0, 0, width/sf, height/sf);
    axis_sw = !axis_sw;
}
//--------------------------------------------------------------------------------
float Viewer::get_scale() const{
    return scale;
}
glm::vec3 Viewer::get_vmin() const{
    if (axis_sw) return glm::vec3(-1.f,-1.f,-1.f);
    return min;
}
glm::vec3 Viewer::get_vmax() const{
    if (axis_sw) return glm::vec3(1.f,1.f,1.f);
    return max;
}
float Viewer::get_bounding_box(int i, bool mm) const{
    const glm::vec3 & getting = mm ? max: min;
    int j =0; float rej_value= getting.x;
    for(const float & c: {getting.x, getting.y, getting.z}){
        if(i==j) return c;
        j++;
        rej_value = mm? std::max(rej_value, c): std::min(rej_value, c);
    }
    return rej_value;
}
void Viewer::set_bounding_box(int i, float v,  bool mm){
    glm::vec3 & changing = mm ? max: min;
    int j =0;
    for(float * c: {&changing.x, &changing.y, &changing.z}){
        if(i==j) {*c = v; break;}
        j++;
    }
}
//--------------------------------------------------------------------------------
void Viewer::set_scale(float sc){
    scale = sc;
}
//--------------------------------------------------------------------------------
//static std::string Viewer::read_string(){
//};
std::string read_string(){
    std::string input_command;
    std::cin>>input_command;
    return input_command;
};
//--------------------------------------------------------------------------------
std::string Viewer::get_command(){
    std::string com;
    if (!command_fut.valid()) {
        command_fut = std::async(std::launch::async, read_string);
        com = std::string("");
    }
    std::chrono::milliseconds span (1);
    bool done = (command_fut.wait_for(span) == std::future_status::ready);
    if (done) com = command_fut.get();
    return com;
}
//--------------------------------------------------------------------------------
//template<class T> void Viewer::setminmax(SurfTemplate<T> * Sur){
//    const glm::vec3 * triangles = Sur->get_triangles();
//    if (Sur->get_cells_size() == 0) return ;
//    min = triangles[0];
//    max = triangles[0];
//    for(int j = 0 ; j < Sur->get_cells_size()*3; j++){
//        const glm::vec3 & coords = triangles[j];
//        for(int i = 0; i<3; i++){
//            min[i] = std::min(min[i], coords[i]);
//            max[i] = std::max(max[i], coords[i]);
//        }
//        //std::cout<<glm::to_string(coords)<<std::endl;
//    //    cent += coords;
//    }
//}
//template void Viewer::setminmax<float>(SurfTemplate<float> * Sur);
//template void Viewer::setminmax<2>(Surface<2> * Sur);
//template void Viewer::setminmax<4>(Surface<4> * Sur);
//--------------------------------------------------------------------------------
// Не хочу поворачивать все вектора, автоскэйл будет ещё и сбрасывать поворот.
// а ладно scale не будет от них зависеть!

//template <class T> void Viewer::autoscale(SurfTemplate<T> * Sur){
//    setminmax(Sur);
//    glm::vec3 cent = (max + min)*0.5f;
//    scale = glm::distance(cent, max);
//    //cent*= 1./(Sur->get_cells_size()*3.);
//    //for(int j = 0 ; j < Sur->get_cells_size()*3; j++){
//    //    const glm::vec3 & coords = triangles[j];
//    //    dist = std::max(glm::distance(coords,cent),dist);
//    //    //std::cout<<glm::to_string(coords)<<std::endl;
//    //    //cent += coords;
//    //}
//    //std::cout<<scale<<" "<<glm::to_string(pos)<<std::endl;
//    //std::cout<<glm::to_string(min)<<" | "<<glm::to_string(cent)<<std::endl;
//    //std::cout<<glm::to_string(max)<<" | "<<glm::to_string(cent)<<std::endl;
//    //orient =glm::angleAxis(-(float)M_PI*0.5f,glm::vec3(1.f,0.f,0.f));
//    pos = -cent*glm::inverse(orient);
//    //scale = std::max(glm::distance(min, cent), glm::distance(max, cent));
//    //scale = dist;
//    if (scale == 0.f) scale = 1.f;
//
//    pos -= glm::vec3(0.f, 0.f, scale);
//    std::cout<<scale<<" "<<glm::to_string(pos)<<std::endl;
//    reshape(width, height);
//}
void Viewer::automove(){
    glm::vec3 cent = (max + min)*0.5f;
    scale = glm::distance(cent, max);
    pos = -cent*glm::inverse(orient);
    if (scale == 0.f) scale = 1.f;
    pos -= glm::vec3(0.f, 0.f, scale);
    //std::cout<<scale<<" "<<glm::to_string(pos)<<std::endl;
    reshape(width, height);
}
////template void Viewer::autoscale<1>(Surface<1> * Sur);
////template void Viewer::autoscale<2>(Surface<2> * Sur);
//template void Viewer::autoscale<float>(SurfTemplate<float> * Sur);
//--------------------------------------------------------------------------------
//SHADERS
//--------------------------------------------------------------------------------

void ShaderProg::init(){
    sprog = ProgLink(vshader,fshader);
    printf("link end\n");
    AttachAttrs(vattr, "coord", sprog);
    AttachAttrs(nattr, "normal", sprog);
    AttachAttrs(cattr, "color", sprog);
    AttachUniform(mvp_loc, "MVP", sprog);
    AttachUniform(unif_minmax, "minmaxmul", sprog);
    AttachUniform(tex_length, "tex_length",sprog);
    AttachUniform(vmin, "vmin",sprog);
    AttachUniform(vmax, "vmax",sprog);
    checkOpenGLerror();
    printf("shaders end\n");
}
//--------------------------------------------------------------------------------
ShaderProg::ShaderProg(const char * vss, const char* fss){
    checkOpenGLerror();
    printf("shaders begin\n");
    vshader = ShaderComp(GL_VERTEX_SHADER, vss);
    printf("shaders v\n");
    fshader = ShaderComp(GL_FRAGMENT_SHADER, fss);
    printf("shaders f\n");
    this->init();
}
//--------------------------------------------------------------------------------
ShaderProg::~ShaderProg(){
    glDeleteProgram(sprog);
}
//--------------------------------------------------------------------------------
void ShaderProg::extern_load(const char * vsf, const char* fsf){
    checkOpenGLerror();
    printf("shaders begin\n");
    vshader = ShaderLoad(GL_VERTEX_SHADER, vsf);
    fshader = ShaderLoad(GL_FRAGMENT_SHADER, fsf);
    printf("shaders init\n");
    this->init();
}
//--------------------------------------------------------------------------------
GLuint ShaderProg::ShaderLoad(GLenum shader_type, const char * shader_file){
    std::ifstream in(shader_file);
    std::string contents( (std::istreambuf_iterator<char>(in)),
            std::istreambuf_iterator<char>() );
    in.close();
    char * Source  = new char[contents.length() + 1];
    std::strcpy( Source, contents.c_str() );
    GLuint shade = ShaderComp(shader_type, Source);
    delete [] Source;
    return shade;
}
//--------------------------------------------------------------------------------
GLuint ShaderProg::ShaderComp(GLenum shader_type, const char * Source){
    printf("shaders Comp\n");
    GLuint shade = glCreateShader(shader_type);
    printf("shaders cre\n");
    glShaderSource(shade, 1, & Source, NULL);
    printf("shaders comp\n");
    glCompileShader(shade);
    if (shader_type == GL_VERTEX_SHADER) std::cout << "vertex shader \n";
    else std::cout << "fragment shader \n";
    return shade;
}
//--------------------------------------------------------------------------------
// Шейдерная программа
GLuint ShaderProg::ProgLink(GLuint vShader, GLuint fShader){
    // Создаем программу и прикрепляем шейдеры к ней
    GLuint Program = glCreateProgram();
    glAttachShader(Program, vShader);
    glAttachShader(Program, fShader);
    // Линкуем шейдерную программу
    glLinkProgram(Program);
    // Проверяем статус сборки
    int link_ok;
    glGetProgramiv(Program, GL_LINK_STATUS, &link_ok);
    if(!link_ok)
    {
        std::cout << "error attach shaders \n";
        shaderLog(vShader);
        shaderLog(fShader);

    }
    return Program;
}
//--------------------------------------------------------------------------------
void ShaderProg::AttachUniform(GLint & Unif, const char * name, GLuint Program) {
    Unif = glGetUniformLocation(Program, name);
    if(Unif == -1)
    {
        std::cout << "could not bind attrib " << name << std::endl;
        return;
    }
}
//--------------------------------------------------------------------------------
// Функция печати лога шейдера
void ShaderProg::shaderLog(unsigned int shader)
{
    int   infologLen   = 0;
    int   charsWritten = 0;
    char *infoLog;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);

    if(infologLen > 1)
    {
        infoLog = new char[infologLen];
        if(infoLog == NULL)
        {
            std::cout<<"ERROR: Could not allocate InfoLog buffer\n";
            exit(1);
        }
        glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog);
        std::cout<< "InfoLog: " << infoLog << "\n\n\n";
        delete[] infoLog;
    }
}
//--------------------------------------------------------------------------------
// Прикрепление массива аттрибутов
void ShaderProg::AttachAttrs(GLint & Attr ,const char* attr_name,GLuint Program) {
    // Вытягиваем ID атрибута из собранной программы
    Attr = glGetAttribLocation(Program, attr_name);
    if(Attr == -1)
    {
        std::cout << "could not bind attrib " << attr_name << std::endl;
        return;
    }
}
//--------------------------------------------------------------------------------
/*template<int sur_size>*/ void ShaderProg::render(/*Surface<sur_size>*/ Plottable * surf, Viewer * view, Texture * tex ){
    glUseProgram(sprog);
    tex->use_texture(tex_length);
    load_mvp(view->calc_mvp());
    loadclip(view->get_vmin(), view->get_vmax());
    surf->plot(vattr,nattr,cattr,unif_minmax);
    glUseProgram(0);
}
//template void ShaderProg::render<2>(Surface<2> * surf, Viewer * view, Texture * tex );
//template void ShaderProg::render<4>(Surface<4> * surf, Viewer * view, Texture * tex );
//--------------------------------------------------------------------------------
void ShaderProg::loadclip(const glm::vec3 & vi , const glm::vec3 & va){
    glUniform3f( vmax, va.x,va.y,va.z);
    glUniform3f( vmin, vi.x,vi.y,vi.z);
}
//--------------------------------------------------------------------------------
void ShaderProg::load_mvp(const glm::mat4 & MVP){
    glUniformMatrix4fv( mvp_loc, 1, GL_FALSE, glm::value_ptr(MVP));
}

//--------------------------------------------------------------------------------
//TEXTURES
//--------------------------------------------------------------------------------
Texture::Texture(const float* pal, int length){
    tex_len = length;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_1D, textureID);
    glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glBindTexture(GL_TEXTURE_1D, textureID);
    glTexImage1D(GL_TEXTURE_1D, 0, 3, tex_len, 0, GL_RGB, GL_FLOAT, pal);
    glBindTexture(GL_TEXTURE_1D, 0);
}
//--------------------------------------------------------------------------------
Texture::~Texture(){
    glDeleteTextures(1,&textureID);
}
//--------------------------------------------------------------------------------
int Texture::get_length(){
    return tex_len;
}
//--------------------------------------------------------------------------------
//void Texture::load_texture(const float* pal, int length){
//    this->~Texture();
//    Texture(pal, length);
//
//}
//--------------------------------------------------------------------------------
void Texture::use_texture(GLint tli){
    glUniform1f(tli,(float)tex_len);
    glEnable(GL_TEXTURE_1D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, textureID);
}
//--------------------------------------------------------------------------------
