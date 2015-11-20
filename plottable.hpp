#ifndef PLOTTABLE
#define PLOTTABLE

#define IND 0
#define POS 1
#define NRM 2
#define CLR 3
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>
#include <vector>
#include <iostream>
#include "shaderprog.hpp"
class ShaderProg;
//------------------------------------------------------------------------------
class Plottable{
    protected:
        GLuint VAO;
        GLuint VBO[4];
        GLint vattr, nattr, cattr;
        Plottable();
        ~Plottable();
    public:
        virtual void plot(ShaderProg * spr)=0;
        //virtual void plot_index(GLint vattr, GLint nattr, GLint cattr, GLint unif_minmax) const=0;
};
//------------------------------------------------------------------------------
class Axis: public Plottable{
    GLint unif_minmax;
    public:
    Axis():Plottable(){};
    void load_on_device();
    //Закрепим значения атрибутов и т.п.
    void AttachToShader(ShaderProg * spr) ;
    void plot(ShaderProg * spr);
};
//------------------------------------------------------------------------------
template <class T> class SurfTemplate: public Plottable {
    protected:
        GLint unif_minmax;
        glm::vec3 minmaxmul;
        std::vector<glm::vec3> triangles;
        std::vector<T> appends;
        std::vector<unsigned int> select;
        SurfTemplate<T>(): auto_select(true), Plottable(){};
        ~SurfTemplate<T>(){};
    public:
        bool auto_select;
        //------------------------------------------------------------------------------
        void plot(ShaderProg * spr);
        //------------------------------------------------------------------------------
        float min();
        float max();
        //------------------------------------------------------------------------------
        void rangemove(float shift, bool l);
        void extendrange(float factor);
        //------------------------------------------------------------------------------
        void refill_select();
        //------------------------------------------------------------------------------
        virtual const glm::vec3 * get_triangles() =0;
        //------------------------------------------------------------------------------
        virtual int get_cells_size() const =0;
        //------------------------------------------------------------------------------
        virtual void autoset_minmax()=0;
        //------------------------------------------------------------------------------
        virtual void attach_shader(ShaderProg * spr)=0;
        //------------------------------------------------------------------------------
};
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
//SurfTemplate<double>;
//------------------------------------------------------------------------------
#endif //PLOTTABLE
