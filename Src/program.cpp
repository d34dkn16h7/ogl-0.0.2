#include "tools.h"
#include "program.h"

GLuint* Program::dProg = nullptr; /// Default program,first loaded
vector<Program*> Program::programs;

Program::Program(string vShaderSrc ,string fShaderSrc ,string progName)
{
    name = progName;
    isReady = CompileShader(vert,GL_VERTEX_SHADER,vShaderSrc) &&
              CompileShader(frag,GL_FRAGMENT_SHADER,fShaderSrc);

    if(isReady)
    {
        LinkProgram();
        if(dProg == nullptr)
            dProg = &prog;
        programs.push_back(this);
    }
    else
        throw runtime_error("Shader Compile Error");
}

GLuint Program::CompileShader(GLuint& trg,GLuint type,string shaderSrc)
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

void Program::LinkProgram()
{
    prog = glCreateProgram();
    glAttachShader( prog, vert);
    glAttachShader( prog, frag);
    glBindFragDataLocation( prog, 0, "outColor" ); // Hard-coded? :(
    glLinkProgram(prog);
}

void Program::SetUniform(const string& name,const vec2& val)
{
    Use(true);
    const GLchar* attName = name.c_str();
    GLint uniform = glGetUniformLocation(prog, attName);

    if(uniform == -1)
        Tools::Logger::FatalError("Can't access uniform2 " + name);

    glUniform2f(uniform , val.x,val.y);
    Use(false);
}

void Program::SetUniform(const string& name,const vec3& val)
{
    Use(true);
    const GLchar* attName = name.c_str();
    GLint uniform = glGetUniformLocation(prog, attName);

    if(uniform == -1)
        Tools::Logger::FatalError("Can't access uniform3 " + name);

    glUniform3f(uniform , val.x,val.y,val.z);
    Use(false);
}

void Program::SetUniform(const string& name,const vec4& val)
{
    Use(true);
    const GLchar* attName = name.c_str();
    GLint uniform = glGetUniformLocation(prog, attName);

    if(uniform == -1)
        Tools::Logger::FatalError("Can't access uniform4 " + name);

    glUniform4f(uniform , val.r,val.g,val.b,val.a);
    Use(false);
}

void Program::SetUniform(const string& name,const mat4& matrix)
{
    Use(true);
    const GLchar* attName = name.c_str();
    GLint uniform = glGetUniformLocation(prog, attName);

    if(uniform == -1)
        Tools::Logger::FatalError("Can't access uniformM4 " + name);

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

void Program::Use(string progName) /// Static-Func Use progName
{
    glUseProgram(GetProgram(progName));
}

GLuint& Program::GetProgram()
{
    return prog;
}

GLuint& Program::GetProgram(string progName)
{
    for(Program* p : programs)
        if(p->name == progName)
            return p->prog;

    return (*dProg);
}

Program* Program::GetProgramIns(string progName)
{
    for(Program* p : programs)
        if(p->name == progName)
            return p;

    return nullptr;
}
