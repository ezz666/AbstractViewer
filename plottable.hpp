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
//------------------------------------------------------------------------------
class Plottable{
    protected:
        GLuint VAO;
        GLuint VBO[4];
        Plottable():VBO(){};
        ~Plottable(){
            if (VBO[0]){
                std::cout<<"delete buffers"<<std::endl;
                glDeleteBuffers(4,VBO);
            }
            if (VAO) glDeleteVertexArrays(1, &VAO);
        }
    public:
        virtual void plot(GLint vattr, GLint nattr, GLint cattr, GLint unif_minmax) const=0;
        //virtual void plot_index(GLint vattr, GLint nattr, GLint cattr, GLint unif_minmax) const=0;
};
//------------------------------------------------------------------------------
class Axis: public Plottable{
    public:
    void load_on_device(){
        int NTR = 3;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(4,&VBO[0]);
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
    //Закрепим значения атрибутов и т.п.
    void AttachToShader(GLint vattr, GLint nattr, GLint cattr,GLint unif_minmax) const{
        glBindVertexArray(VAO);
        if (unif_minmax != -1) 
            glUniform3f(unif_minmax, 0.f, 2.f, 0.5f);
        if (cattr != -1) {
            glEnableVertexAttribArray(cattr);
            glBindBuffer(GL_ARRAY_BUFFER, VBO[CLR]);
            glVertexAttribPointer(cattr, 1, GL_FLOAT, GL_FALSE, 0, 0);
        }
        if (nattr != -1){
            glEnableVertexAttribArray(nattr);
            glBindBuffer(GL_ARRAY_BUFFER, VBO[NRM]);
            glVertexAttribPointer(nattr, 3, GL_FLOAT, GL_FALSE, 0, 0);
        }
        if (vattr != -1){
            glEnableVertexAttribArray(vattr);
            glBindBuffer(GL_ARRAY_BUFFER, VBO[POS]);
            glVertexAttribPointer(vattr, 3, GL_FLOAT, GL_FALSE, 0, 0);
        }
        glBindVertexArray(0); 
    }
    void plot(GLint vattr, GLint nattr, GLint cattr,GLint unif_minmax) const{
        int Ntr = 3;
        AttachToShader(vattr, nattr, cattr, unif_minmax);
        glBindVertexArray(VAO);
        //glDrawElements(GL_LINES,Ntr*2, GL_UNSIGNED_INT, (void *)0); 
        glDrawElementsInstanced(GL_LINES,Ntr*2, GL_UNSIGNED_INT, (void *)0,1); 
        glBindVertexArray(0); 
        //glDisableVertexAttribArray(vattr);
        //glDisableVertexAttribArray(cattr);
        //glDisableVertexAttribArray(nattr);
        glBindBuffer(GL_ARRAY_BUFFER,0);
    }
};
//------------------------------------------------------------------------------
template <class T> class SurfTemplate: public Plottable {
    protected:
        glm::vec3 minmaxmul;
        std::vector<glm::vec3> triangles;
        std::vector<T> appends;
        std::vector<unsigned int> select;
        SurfTemplate<T>(): auto_select(true){};
        ~SurfTemplate<T>(){};
    public:
        bool auto_select;
        //------------------------------------------------------------------------------
        void plot(GLint vattr, GLint nattr, GLint cattr, GLint unif_minmax) const{
            int Ntr = select.size();
            attach_shader(vattr, nattr, cattr, unif_minmax); // Возможно оверкил делать это каждый раз
            glBindVertexArray(VAO);
            //glDrawElements(GL_TRIANGLES, Ntr, GL_UNSIGNED_INT, (void *)0); 
            glDrawElementsInstanced(GL_TRIANGLES, Ntr, GL_UNSIGNED_INT, (void *)0, 1); 
            glBindVertexArray(0);
            //glDisableVertexAttribArray(vattr);
            //glDisableVertexAttribArray(cattr);
            //glDisableVertexAttribArray(nattr);
            glBindBuffer(GL_ARRAY_BUFFER,0);
        }
        //------------------------------------------------------------------------------
        float min(){
            return minmaxmul[0];
        }
        //------------------------------------------------------------------------------
        float max(){
            return minmaxmul[1];
        }
        //------------------------------------------------------------------------------
        void rangemove(float shift,bool l){
            float &min = minmaxmul[0];
            float &max = minmaxmul[1];
            float len = max - min;
            if (l) min -= shift * len;
            else max += shift * len;
        }
        //------------------------------------------------------------------------------
        void extendrange(float factor){
            float &min = minmaxmul[0];
            float &max = minmaxmul[1];
            float &mul = minmaxmul[2];
            float len_2 = (max - min)*0.5f,
                  center = (min + max)* 0.5f;
            min = center - len_2*factor;
            max = center + len_2*factor;
            mul = (min != max)? 1.f/(max  - min): 1.f;
        }
        void refill_select(){
            int NTR = get_cells_size();
            select.clear();
            select.resize(NTR*3);
            for(auto i=0; i < select.size(); i++) select[i] = i;
        }
        //------------------------------------------------------------------------------
        virtual const glm::vec3 * get_triangles() =0;
        //------------------------------------------------------------------------------
        virtual int get_cells_size() const =0;
        //------------------------------------------------------------------------------
        virtual void autoset_minmax()=0;
        //------------------------------------------------------------------------------
        virtual void attach_shader(GLint vattr, GLint nattr, GLint cattr, GLint unif_minmax) const=0;
        //------------------------------------------------------------------------------
};
//SurfTemplate<float>;
//SurfTemplate<double>;
//------------------------------------------------------------------------------
#endif //PLOTTABLE
