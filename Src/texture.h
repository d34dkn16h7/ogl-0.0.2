#ifndef TEXTURE_H
#define TEXTURE_H

#include <unordered_map>
#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <vector>
#include <string>

using namespace std;

class Texture
{
private:
    static unordered_map<string,Texture*> _m;
public:
    string
        m_name;

    GLuint tex = 0;

    Texture(string _n,GLuint _tex) : m_name(_n) , tex(_tex) {}

    static Texture* GetTexture( string );
    static void LoadTexture( string,string );
};

#endif // TEXTURE_H
