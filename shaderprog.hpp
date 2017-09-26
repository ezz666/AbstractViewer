#ifndef SHADERPROG_HPP
#define SHADERPROG_HPP
#include "viewer_template.hpp"
#include "plottable.hpp"
class Viewer;
class Plottable;
class ShaderProg;
//--------------------------------------------------------------------------------
// TEXRTURES
//--------------------------------------------------------------------------------
class Texture{
    protected:
        GLuint textureID;
        GLuint samplerID;
        GLint tex_loc;
        GLenum TexTarget;
        int tex_len;
    public:
        Texture():tex_loc(-1),TexTarget(GL_TEXTURE0){};
        Texture(GLenum _TexTarget):TexTarget(_TexTarget){};
        Texture(const float * pal, int length, GLenum _TexTarget=GL_TEXTURE0);
        ~Texture();
        int get_length();

        //void load_texture(const float * pal, int length);
        void use_texture(ShaderProg *spr, const char * name, GLenum texture = GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
        void linear();
        void nearest();
};
class Texture3D: public Texture{
    protected:
        int xsz, ysz, zsz;
        //Texture3D(const float* pal, int length, GLenum TexTarget = GL_TEXTURE0);
    public:
        Texture3D(const float* pal, int xsz, int ysz, int zsz, GLenum _TexTarget = GL_TEXTURE0);
        ~Texture3D(){};
        void load(const float* pal, int xsz, int ysz, int zsz);
        int get_xsz();
        int get_ysz();
        int get_zsz();
        void use_texture(ShaderProg *spr, const char * name, GLenum texture = GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
};
//--------------------------------------------------------------------------------
// FRAMEBUFFERS
//--------------------------------------------------------------------------------
class FrameBuffer{
    protected:
        GLuint frame_buffer, render_buffer,depth_buffer;
        GLenum type;
        int _width, _height;
    public:
        FrameBuffer(int w, int h, int _type= (int)GL_RGBA);
        ~FrameBuffer();
        void resize(int w, int h);
        int width(){return _width;};
        int height(){return _height;};
        void bind_draw();
        void bind_read();
        //int pixels_num(){return _width*_height;};
        void relax();
};
//--------------------------------------------------------------------------------
// SHADERS
//--------------------------------------------------------------------------------
class ShaderProg{
    protected:
        GLuint vshader, fshader, sprog;
        //GLint vattr, nattr, cattr, mvp_loc, it_mvp_loc,
        //      unif_minmax, /*tex_length,*/ vmin, vmax;
        static GLuint ShaderExternLoad(GLenum shader_type, const char * shader_file);
        static GLuint ShaderLoad(GLenum shader_type, const char * shader_source);
        static GLuint ShaderComp(GLenum shader_type, const char * shader_source);
        static GLuint ProgLink(GLuint vShader, GLuint fShader);
        static void _AttachUniform(GLint & Unif, const char * name, GLuint Program);
        static void _AttachAttr(GLint & Attr ,const char* attr_name,GLuint Program);
        void init();
    public:
        void AttachUniform(GLint & Unif, const char * name);
        void AttachAttr(GLint & Attr ,const char* attr_name);
        static void shaderLog(unsigned int shader);
        ShaderProg(){};
        ShaderProg(const char * vertex_shader_source, const char * fragment_shader_source);
        void extern_load(const char * vertex_shader_file, const char * fragment_shader_file);
        void load(const char * vertex_shader_file, const char * fragment_shader_file);
        ///*template<int sur_size>*/ void render(/*Surface<sur_size>*/Plottable * surf, Viewer * view, Texture* tex);
        void start();
        void stop();
        ///*template<int sur_size>*/ void render(/*Surface<sur_size>*/Plottable * surf, Viewer * view);
        //void load_mvp(const glm::mat4 & MVP, const glm::mat4 & itMVP);
        //void loadclip(const glm::vec3 & vi, const glm::vec3 & va);
        ~ShaderProg();
};
#endif //SHADERPROG_HPP
