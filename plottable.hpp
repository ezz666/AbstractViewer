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
        virtual ~Plottable();
    public:
        virtual void plot(ShaderProg * spr)=0;
        //virtual void plot_index(GLint vattr, GLint nattr, GLint cattr, GLint unif_minmax) const=0;
};
//------------------------------------------------------------------------------
class Axis: public Plottable{
    GLint unif_minmax;
    ~Axis(){};
    public:
    Axis():Plottable(){};
    void load_on_device();
    //Закрепим значения атрибутов и т.п.
    void AttachToShader(ShaderProg * spr) ;
    void plot(ShaderProg * spr);
};
//------------------------------------------------------------------------------
class SurfTemplate: public Plottable {
    protected:
        GLint unif_minmax;
        glm::vec3 minmaxmul;
        std::vector<glm::vec3> triangles;
        std::vector<float> appends;
        std::vector<unsigned int> select;
        SurfTemplate(): Plottable(), auto_select(true){};
        ~SurfTemplate(){};
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
#endif //PLOTTABLE
