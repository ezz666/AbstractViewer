#ifndef PLOTTABLE
#define PLOTTABLE

#define IND 3
#define POS 0
#define CLR 1
#define NRM 2
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>
#include <vector>
#include <iostream>
#include "shaderprog.hpp"
class ShaderProg;
class Texture;
//--------------------------------------------------------------------------------
// Vertex array object (VAO)
//--------------------------------------------------------------------------------
class VertexArray{
    private:
        GLuint AO, EBO; // vertex array object, Element Buffer obkect
        std::vector<GLuint> BOs; // Buffer objects
        std::vector<GLint> attrs; // attributes position
    public:
        VertexArray();
        ~VertexArray();
        void add_buffer();
        void load_data(int pos, int size, const void * data);
        void load_indices(int size, const void * data);
        GLuint & get_BO(int pos);
        void bind();
        GLint & get_attr(int pos);
        void enable_attr(int pos, int num, GLenum type);
        void release();
};
//------------------------------------------------------------------------------
class Plottable{
    protected:
        //GLuint VAO;
        //GLuint VBO[4];
        VertexArray VAO;
        //GLint vattr, nattr, cattr;
        Plottable():VAO(){};
        virtual ~Plottable(){};
    public:
        virtual void plot(ShaderProg * spr)=0;
        //virtual void plot_index(GLint vattr, GLint nattr, GLint cattr, GLint unif_minmax) const=0;
};
//------------------------------------------------------------------------------
class Axis: public Plottable{
    GLint unif_minmax;
    public:
    ~Axis(){};
    Axis():Plottable(){
        VAO.add_buffer();
        VAO.add_buffer();
        VAO.add_buffer();
    };
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
class PaletteBox: public Plottable{
    Texture * tex;
    glm::ivec2 xymin, xymax;
    bool vertical;
    GLint unif_minmax;
    public:
    PaletteBox(Texture * _tex = nullptr, const glm::ivec2 _min = glm::ivec2(-1), const glm::ivec2 _max = glm::ivec2(1));
    void load_on_device();
    bool get_vertical();
    void set_texture(Texture * tex);
    void switch_vertical();
    void set_vertical(bool new_vert);
    void AttachToShader(ShaderProg * spr);
    void plot(ShaderProg * spr);
    void set_xyminmax(const int * newxymin, const int * newxymax);
    void get_xyminmax(int * newxymin, int * newxymax);
};
//------------------------------------------------------------------------------
#endif //PLOTTABLE
