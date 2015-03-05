#include "program.h"
#include "geometry.h"
#include "renderer.h"
#include "tools.h"

using namespace Tools;

vector< GData* > Geometry::gData;

void Geometry::Load(string fSrc,string name) /// Load .obj model
{
    gPtr = FindGDT(name);
    if(gPtr == nullptr)
    {
        gPtr = new GData();
        gPtr->info.modelId = name;
        Token geoToken( File::LoadFile(fSrc) ); /// ch == ' ' || ch == '\n' || ch == '\t' || ch == '/'
        vector<char> rules = {' ' , '\n' ,'\t' , '/'};
        vector<char> splitter = { '/'};
        geoToken.RemakeWithRules(rules,splitter);

        while( geoToken.Next() !=  Token::EndToken )
        {
            if( geoToken == "v" ) gPtr->pVerticle( geoToken.GetNVec3() );
            if( geoToken == "vt" ) gPtr->pTextureCoord( geoToken.GetNVec2() );
            if( geoToken == "f" )
            {
                while( geoToken.CanGNum() )
                {
                    gPtr->pElement( (GLuint)geoToken.GetNi() - 1 );
                    if(geoToken.Peek(1) == "/") /// is next splitter for texture index?
                    {
                        geoToken.Next(); /// Skip the '/' splitter
                        gPtr->pTextureIndex( (geoToken.GetNi() - 1) * 2 );
                    }
                }
            }
        }

        if(geoToken.tokens.size() > 2)
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
    gPtr->MakeData();

    glGenVertexArrays( 1, &gPtr->vao );
    glBindVertexArray( gPtr->vao );

    glGenBuffers( 1, &gPtr->vbo );
    glBindBuffer( GL_ARRAY_BUFFER , gPtr->vbo );
    glBufferData( GL_ARRAY_BUFFER , (gPtr->data.size() * sizeof( gPtr->data[0])), &gPtr->data[0],GL_STATIC_DRAW);

    GLuint vertexAttrib = Program::GetProgramIns("Model")->gAttrib("vertexIn");
    GLuint textureCoordAttrib = Program::GetProgramIns("Model")->gAttrib("textureCoordIn");

    const size_t dataSize = sizeof(GLfloat) * 5;

    glEnableVertexAttribArray( vertexAttrib );
    glVertexAttribPointer( vertexAttrib , 3, GL_FLOAT, GL_FALSE, dataSize ,  0 );

    glEnableVertexAttribArray( textureCoordAttrib );
    glVertexAttribPointer( textureCoordAttrib , 2, GL_FLOAT, GL_FALSE, dataSize , (GLvoid*)( sizeof(GLfloat) * 3 ) );

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
        int vertexIndexer = elementary[i] * 3;
        data.push_back(verticles[vertexIndexer]);
        data.push_back(verticles[vertexIndexer + 1]);
        data.push_back(verticles[vertexIndexer + 2]);

        if (textureIndexer.size() > 0 && textureCoord.size() > 0) /// is indexed
        {
            //cout << " Indexed Coordinates" << endl;
            int textureIndexeri = textureIndexer[i];
            lastIndexed = vec2(textureCoord[textureIndexeri],textureCoord[textureIndexeri + 1]);
        }
        else if( ni < textureCoord.size() ) /// not indexed but has data
        {
            //cout << " Raw Coordinates" << endl;
            lastIndexed = vec2(textureCoord[ni],textureCoord[ni + 1]);
        }
        //else /// Push last indexed values
            //cout << " TextureMapped -> 0:0" << endl;
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

void GData::pTextureCoord(vec2 tcoord)
{
    textureCoord.push_back(tcoord.x);
    textureCoord.push_back(tcoord.y);
}

void GData::pVerticle(vec3 vertex)
{
    verticles.push_back(vertex.x);
    verticles.push_back(vertex.y);
    verticles.push_back(vertex.z);
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
