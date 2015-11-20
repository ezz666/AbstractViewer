#include "plottable.hpp"
//------------------------------------------------------------------------------
// Plottable
//------------------------------------------------------------------------------
Plottable::Plottable(){
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(4,&VBO[0]);
    glBindVertexArray(0);
};
//------------------------------------------------------------------------------
Plottable::~Plottable(){
    if (VBO[0]){
        std::cout<<"delete buffers"<<std::endl;
        glDeleteBuffers(4,VBO);
    }
    if (VAO) glDeleteVertexArrays(1, &VAO);
}
//------------------------------------------------------------------------------
// Axis
//------------------------------------------------------------------------------
void Axis::load_on_device(){
    int NTR = 3;
    //glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    //glGenBuffers(4,&VBO[0]);
    const glm::vec3 tri[6] = {  glm::vec3(0,0,0), glm::vec3(1,0,0), glm::vec3(0,0,0),
        glm::vec3(0,1,0), glm::vec3(0,0,0), glm::vec3(0,0,1) };
    const glm::vec3 norm[6] = {  glm::vec3(1,1,1), glm::vec3(1,1,1), glm::vec3(1,1,1),
        glm::vec3(1,1,1), glm::vec3(1,1,1), glm::vec3(1,1,1) };
    float ap[6] = {0.f,0.f,1.f,1.f,2.f,2.f};
    unsigned int indices[6] = {0,1, 2,3, 4,5};//Мы не можем сделать по-другому : цвет различается значит все индексы разные
    glBindBuffer(GL_ARRAY_BUFFER, VBO[POS]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * NTR*2, tri, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[NRM]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * NTR*2, norm, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[CLR]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * NTR*2, ap, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[IND]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned int), indices, GL_STATIC_DRAW );
    glBindVertexArray(0);
}
//------------------------------------------------------------------------------
void Axis::AttachToShader(ShaderProg * spr) {
    glBindVertexArray(VAO);
    spr->AttachUniform(unif_minmax,"minmaxmul");
    spr->AttachAttr(nattr,"normal");
    spr->AttachAttr(cattr,"color");
    spr->AttachAttr(vattr,"coord");
    if (unif_minmax != -1)
        glUniform3f(unif_minmax, 0.f, 2.f, 0.5f);
    if (vattr != -1){
        glEnableVertexAttribArray(vattr);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[POS]);
        glVertexAttribPointer(vattr, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
    if (nattr != -1){
        glEnableVertexAttribArray(nattr);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[NRM]);
        glVertexAttribPointer(nattr, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
    if (cattr != -1) {
        glEnableVertexAttribArray(cattr);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[CLR]);
        glVertexAttribPointer(cattr, 1, GL_FLOAT, GL_FALSE, 0, 0);
    }
    glBindVertexArray(0);
}
//------------------------------------------------------------------------------
void Axis::plot(ShaderProg * spr) {
    int Ntr = 3;
    AttachToShader(spr);
    glBindVertexArray(VAO);
    //glDrawElements(GL_LINES,Ntr*2, GL_UNSIGNED_INT, (void *)0);
    glDrawElementsInstanced(GL_LINES,Ntr*2, GL_UNSIGNED_INT, (void *)0,1);
    glBindVertexArray(0);
    //glDisableVertexAttribArray(vattr);
    //glDisableVertexAttribArray(cattr);
    //glDisableVertexAttribArray(nattr);
    //glBindBuffer(GL_ARRAY_BUFFER,0);
}
//------------------------------------------------------------------------------
// SurfTemplate<T>
//------------------------------------------------------------------------------
template<class T> void SurfTemplate<T>::plot(ShaderProg * spr) {
    int Ntr = select.size();
    attach_shader(spr); // Возможно оверкил делать это каждый раз
    glBindVertexArray(VAO);
    //glDrawElements(GL_TRIANGLES, Ntr, GL_UNSIGNED_INT, (void *)0);
    glDrawElementsInstanced(GL_TRIANGLES, Ntr, GL_UNSIGNED_INT, (void *)0, 1);
    glBindVertexArray(0);
    //glDisableVertexAttribArray(vattr);
    //glDisableVertexAttribArray(cattr);
    //glDisableVertexAttribArray(nattr);
    //glBindBuffer(GL_ARRAY_BUFFER,0);
}
//------------------------------------------------------------------------------
template<class T> float SurfTemplate<T>::min(){
    return minmaxmul[0];
}
//------------------------------------------------------------------------------
template<class T> float SurfTemplate<T>::max(){
    return minmaxmul[1];
}
//------------------------------------------------------------------------------
template<class T> void SurfTemplate<T>::rangemove(float shift,bool l){
    float &min = minmaxmul[0];
    float &max = minmaxmul[1];
    float len = max - min;
    if (l) min -= shift * len;
    else max += shift * len;
}
//------------------------------------------------------------------------------
template<class T> void SurfTemplate<T>::extendrange(float factor){
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
template<class T> void SurfTemplate<T>::refill_select(){
    int NTR = get_cells_size();
    select.clear();
    select.resize(NTR*3);
    for(auto i=0; i < select.size(); i++) select[i] = i;
}
//------------------------------------------------------------------------------