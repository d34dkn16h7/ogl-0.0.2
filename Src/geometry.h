#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <fstream>
#include <vector>
#include <string>

#include "texture.h"

using namespace std;
using namespace glm;

enum LoadSatus
{
    Loaded,Fail,AlreadyLoaded
};

struct ModelInfo
{
    string modelId;
    int vertexC,textureCoordC;
    int elementC,textureIndexerC;
};

class GData
{
public:
    ModelInfo info;

    vector<GLfloat> data;

    vector<GLuint> textureIndexer;
    vector<GLfloat> textureCoord;

    vector<GLuint> elementary;
    vector<GLfloat> verticles;

    unsigned int elementCount;
    GLuint vbo,vao,ebo;
    GLuint type = GL_TRIANGLES;

    void MakeData();
    void printData();
    void printInfo();

    void pVerticle(vec3);
    void pTextureCoord(vec2);

    void pElement(GLuint);
    void pTextureIndex(GLuint);

    unsigned int gEdgeCount() const;
    GLuint gVBO() const;
    GLuint gVAO() const;
    GLuint gEBO() const;
    GLuint gType() const;
};

class Geometry
{
private:
    static vector< GData* > gData;

    GData* FindGDT(string);

    void UniteData();
    void LinkData();
public:
    Texture* m_tex;
    string fileToken;
    string nameToken;
    GData* gPtr = nullptr;

    void Load(string,string);
};
#endif // GEOMETRY_H
