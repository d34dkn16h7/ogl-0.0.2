#include "program.h"
#include "geometry.h"
#include "renderer.h"
#include "tools.h"

using namespace Tools;

vector< GData* > Geometry::gData;

const string
    vertices = "v",
    textureV = "vt",
    elemVert = "f";

void Geometry::Load(string fSrc,string name) /// Load .obj model
{
    gPtr = FindGDT(name);
    if(gPtr == nullptr)
    {
        gPtr = new GData();
        gPtr->info.modelId = name;
        Token geoT( File::LoadFile(fSrc) ); /// ch == ' ' || ch == '\n' || ch == '\t' || ch == '/'
        vector<char> rules = {' ' , '\n' ,'\t' , '/'};
        vector<char> splitter = { '/'};
        geoT.RemakeWithRules(rules,splitter);

        while( geoT.Next() !=  Token::EndToken )
        {
            if( geoT == vertices ) gPtr->pVerticle( geoT.GetNVec3() );
            if( geoT == textureV ) gPtr->pTextureCoord( geoT.GetNVec2() );
            if( geoT == elemVert )
                while( geoT.CanGNum() )
                {
                    gPtr->pElement( (GLuint)geoT.GetNi() - 1 );
                    if(geoT.Peek(1) == "/") /// is next splitter for texture index?
                    {
                        geoT.Next(); /// Skip the '/' splitter
                        gPtr->pTextureIndex( (geoT.GetNi() - 1) * 2 );
                    }
                }
        }

        if(geoT.tokens.size() > 2)
        {
            gData.push_back(gPtr);

            LinkData();
        }
        else
            Tools::Logger::Error("Empty or corrupted model file : " + name);
    }
}

GData* Geometry::FindGDT(string sStr) /// Find a model ptr
{
    for(GData* val : gData)
        if(val->info.modelId == sStr)
            return val;

    return nullptr;
}

void Geometry::LinkData() /// Link VAO + VBO + EBO
{
    const size_t dataSize = sizeof(GLfloat) * 5;
    GLuint vertexAttrib = Program::GetProgramIns("Model")->gAttrib("vertexIn");
    GLuint textureCoordAttrib = Program::GetProgramIns("Model")->gAttrib("textureCoordIn");

    gPtr->MakeData();
    {
        glGenVertexArrays( 1, &gPtr->vao );
        glBindVertexArray( gPtr->vao );

        glGenBuffers( 1, &gPtr->vbo );
        glBindBuffer( GL_ARRAY_BUFFER , gPtr->vbo );
        glBufferData( GL_ARRAY_BUFFER , (gPtr->data.size() * sizeof( gPtr->data[0])), &gPtr->data[0],GL_STATIC_DRAW);

        glEnableVertexAttribArray( vertexAttrib );
        glVertexAttribPointer( vertexAttrib , 3, GL_FLOAT, GL_FALSE, dataSize ,  0 );

        glEnableVertexAttribArray( textureCoordAttrib );
        glVertexAttribPointer( textureCoordAttrib , 2, GL_FLOAT, GL_FALSE, dataSize , (GLvoid*)( sizeof(GLfloat) * 3 ) );
    }
    gPtr->verticles.clear();
    gPtr->textureCoord.clear();
    gPtr->elementary.clear();
    gPtr->data.clear();
}

// GData
void GData::MakeData()
{
    unsigned int ni = 0;
    vec2 lastIndexed(0,0);
    elementCount = elementary.size();

    for(unsigned int i = 0;i < elementary.size();i++)
    {
        int vIndexer = elementary[i] * 3;
        int tIndexer = textureIndexer[i];

        data.push_back(verticles[vIndexer]);
        data.push_back(verticles[vIndexer + 1]);
        data.push_back(verticles[vIndexer + 2]);

        if (textureIndexer.size() > 0 && textureCoord.size() > 0) /// is indexed
            lastIndexed = vec2(textureCoord[tIndexer],textureCoord[tIndexer + 1]);
        else if( ni < textureCoord.size() ) /// not indexed but has data
            lastIndexed = vec2(textureCoord[ni],textureCoord[ni + 1]);

        data.push_back(lastIndexed.x);
        data.push_back(lastIndexed.y);

        ni += 2;
    }

    info.vertexC = verticles.size() / 3;
    info.elementC = elementary.size();
    info.textureCoordC = textureCoord.size() / 2;
    info.textureIndexerC = textureIndexer.size();

    printInfo();
    //printData();
}

void GData::printInfo()
{
    stringstream strs;

    strs << "OBJ DUMP" << endl;
    strs << "\t" << "Verticles : " << info.vertexC << endl;
    strs << "\t" << "Vertex Elements : " << info.elementC << endl;
    strs << "\t" << "Indexed Texture Coordinates : " << info.textureIndexerC << endl;
    strs << "\t" << "Texture Coordinates : " << info.textureCoordC;

    Tools::Logger::Info(strs.str());
}

void GData::printData()
{
    stringstream strs;
    strs << "OBJ DATA DUMP" << endl;
    for (unsigned int i = 0; i < data.size();i += 5)
    {
        Tools::Str::AddHashStreamVec3(strs,"Vertex",vec3(data[i],data[i + 1],data[i + 2]));
        Tools::Str::AddHashStreamVec2(strs,"VertexTexture",vec2(data[i + 3],data[i + 4]));
    }
    Tools::Logger::Info(strs.str());
}

void GData::pTextureCoord(vec2 v)
{
    textureCoord.push_back(v.x);
    textureCoord.push_back(v.y);
}

void GData::pVerticle(vec3 v)
{
    verticles.push_back(v.x);
    verticles.push_back(v.y);
    verticles.push_back(v.z);
}

void GData::pElement(GLuint val)
{
    elementary.push_back ( val );
}

void GData::pTextureIndex(GLuint val)
{
    textureIndexer.push_back ( val );
}

unsigned int GData::gEdgeCount() const
{
    return elementCount;
}

GLuint GData::gType() const
{
    return type;
}

GLuint GData::gVBO() const
{
    return vbo;
}

GLuint GData::gVAO() const
{
    return vao;
}

GLuint GData::gEBO() const
{
    return ebo;
}
