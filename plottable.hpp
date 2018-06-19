#ifndef PLOTTABLE
#define PLOTTABLE

#define IND 3
#define POS 0
#define CLR 1
#define NRM 2
#include <GL/glew.h>
# define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>
#include <vector>
#include <iostream>
#include <memory>
#include "shaderprog.hpp"
#include "viewer_template.hpp"
class Viewer3D;
class ShaderProg;
class Texture;
class Texture3D;
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
class VolumeTemplate: public Plottable {
  protected:
    std::unique_ptr<Texture3D> data;
    GLint unif_step, unif_size, unif_raystep, unif_cminmaxmul,
          unif_density, unif_brightness, unif_opacity, unif_ftype,
          unif_origin, unif_cubemin, unif_cubemax;
    float raystep, density,brightness,opacity;
    float cubemin[3], cubemax[3];
    glm::vec3 cminmaxmul;
    int ftype;
  public:
    VolumeTemplate();
    ~VolumeTemplate(){}
    void set_density(float dens);
    float get_density();
    void set_brightness(float bright);
    float get_brightness();
    void set_opacity(float op);
    float get_opacity();
    void set_raystep(float rs);
    float get_raystep();
    void set_ftype(int ft);
    int get_ftype();
    float get_ranges(int i);
    void get_auto_box(float * bb_min, float * bb_max);
    void attach_shader(ShaderProg * spr);
    void plot(ShaderProg * spr);
    void load_on_device();
    void reload_cube();
    //------------------------------------------------------------------------------
    void rangemove(float shift,bool l);
    //------------------------------------------------------------------------------
    void extendrange(float factor);
    //------------------------------------------------------------------------------
    void set_range(float lower, float upper);
    //------------------------------------------------------------------------------
    float min();
    //------------------------------------------------------------------------------
    float max();
    //------------------------------------------------------------------------------
    void adjust_cube(const Viewer3D & V);
    //------------------------------------------------------------------------------
    virtual void get_origin(float & x, float & y, float & z) = 0;
    virtual void get_sizes(float & x, float & y, float & z) = 0;
    virtual void get_steps(float & x, float & y, float & z) = 0;
    virtual void autoset_minmax() = 0;
    virtual void reload_texture() = 0;
    virtual void bbox(float * bmin, float * bmax) const = 0;
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
