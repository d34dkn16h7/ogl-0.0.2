#ifndef PROGRAM_H
#define PROGRAM_H

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <stdexcept>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
using namespace glm;

class Program
{
private:
    static vector<Program*> programs;

    bool isReady;

    static GLuint* dProg;
    GLuint
        prog,
        vert,
        frag;
    GLuint CompileShader(GLuint& trg,GLuint type,string shaderSrc);
    void LinkProgram();
public:
    string name;

    Program(string,string,string);
    void Use(bool);

    GLuint gAttrib(string);
    GLuint& GetProgram();

    void SetUniform(const string&,const vec2&);
    void SetUniform(const string&,const vec3&);
    void SetUniform(const string&,const vec4&);
    void SetUniform(const string&,const mat4&);

    static void Use(string);
    static GLuint& GetProgram(string);
    static Program* GetProgramIns(string);
};
#endif // PROGRAM_H
