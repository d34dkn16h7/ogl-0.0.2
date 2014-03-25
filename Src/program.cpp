#include "tools.h"
#include "program.h"

GLuint* Program::dProg = nullptr; /// Default Program aka First Loaded
vector<Program*> Program::programs; /// Program vector

Program::Program(string vShaderSrc ,string fShaderSrc ,string progName) /// Construct Shaders
{
    name = progName;
    isReady = CompileShader(vert,GL_VERTEX_SHADER,vShaderSrc) &&
              CompileShader(frag,GL_FRAGMENT_SHADER,fShaderSrc);
    if(isReady)
    {
        LinkProgram();
        if(dProg != nullptr)
            dProg = &prog;
        programs.push_back(this);
    }
    else
        throw runtime_error("Shader Compile Error");
}

GLuint Program::CompileShader(GLuint& trg,GLuint type,string shaderSrc) /// Compile Shader
{
    trg = glCreateShader(type);
    string source =  Tools::File::LoadFile(shaderSrc);
    const char* src = source.c_str();
    glShaderSource(trg,1,&src,NULL);
    glCompileShader(trg);
    GLint status;
    glGetShaderiv(trg,GL_COMPILE_STATUS,&status);
    return status;
}

void Program::LinkProgram() /// Link Program
{
    prog = glCreateProgram();
    glAttachShader( prog, vert);
    glAttachShader( prog, frag);
    glBindFragDataLocation( prog, 0, "outColor" ); // Hard-coded? Why?
    glLinkProgram(prog);
}

void Program::SetUniform(const string& name,const vec2& val) /// Set Uniform for vector42
{
    Use(true);
    const GLchar* attName = name.c_str();
    GLint uniform = glGetUniformLocation(prog, attName);
    if(uniform == -1)
    {
        cout << "Can't acces uniform " << name << endl;
        throw runtime_error("Fatal Error");
    }

    glUniform2f(uniform , val.x,val.y);
    Use(false);
}

void Program::SetUniform(const string& name,const vec3& val) /// Set Uniform for vector43
{
    Use(true);
    const GLchar* attName = name.c_str();
    GLint uniform = glGetUniformLocation(prog, attName);
    if(uniform == -1)
    {
        cout << "Can't acces uniform " << name << endl;
        throw runtime_error("Fatal Error");
    }
    glUniform3f(uniform , val.x,val.y,val.z);
    Use(false);
}

void Program::SetUniform(const string& name,const vec4& val) /// Set Uniform for vector4
{
    Use(true);
    const GLchar* attName = name.c_str();
    GLint uniform = glGetUniformLocation(prog, attName);
    if(uniform == -1)
    {
        cout << "Can't acces uniform " << name << endl;
        throw runtime_error("Fatal Error");
    }
    glUniform4f(uniform , val.r,val.g,val.b,val.a);
    Use(false);
}

void Program::SetUniform(const string& name,const mat4& matrix) /// Set Uniform for Matrix4
{
    Use(true);
    const GLchar* attName = name.c_str();
    GLint uniform = glGetUniformLocation(prog, attName);
    if(uniform == -1)
    {
        cout << "Can't acces uniform " << name << endl;
        throw runtime_error("Fatal Error");
    }
    glUniformMatrix4fv(uniform, 1, false, value_ptr(matrix));
    Use(false);
}

GLuint Program::gAttrib(string attribName)
{
    return glGetAttribLocation( prog , attribName.c_str() );
}

void Program::Use(bool val) /// Member-Func Use
{
    if(val)
        glUseProgram(prog);
    else
        glUseProgram(0);
}

void Program::Use(bool val , string progName) /// Static-Func Use progName
{
    Program* p = GetProgramIns(progName);
    if(val && p != nullptr)
        glUseProgram(p->prog);
    else
        glUseProgram(0);
}

GLuint& Program::GetProgram() /// Member-Func Get Program
{
    return prog;
}

GLuint& Program::GetProgram(string progName) /// Static-Func Get Program
{
    for(Program* p : programs)
        if(p->name == progName)
            return p->prog;

    return (*dProg);
}

Program* Program::GetProgramIns(string progName) /// Static-Func Get Program Instance
{
    for(Program* p : programs)
        if(p->name == progName)
            return p;

    return nullptr;
}
