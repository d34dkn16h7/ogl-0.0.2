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
    static unordered_map<string,Texture> _m;
public:
    GLuint tex = 0;
    string m_path,m_name;

    void LoadTexture( string,string );
    void SetTexture( string );
};

#endif // TEXTURE_H
