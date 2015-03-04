#include "../tools.h"
#include "../texture.h"

using namespace Tools;

//Token* Settings::setting;
bool Settings::loadTextures;
bool Settings::TextureLoadModern = false;
string Settings::mapFile;
string Settings::vertexShaderFileName;
string Settings::fragmentShaderFileName;
vector<ObjectInfo> Settings::objectInfos;

void Settings::LoadSettings()
{
    Token setting( File::LoadFile("Data/settings") );

    while(setting.Next() != "#endToken")
    {
        if( setting == "#textureMode" )
            loadTextures = setting.Next() == "true" ? true : false;

        if( setting == "#textureModern" )
            TextureLoadModern = setting.Next() == "true" ? true : false;

        if( setting == "vertex" )
            vertexShaderFileName = setting.Next();

        if( setting == "fragment" )
            fragmentShaderFileName = setting.Next();

        if( setting == "#obj" )
            objectInfos.push_back( ObjectInfo( setting.Next() ,setting.Next() ) );

        if( setting == "map")
            mapFile = setting.Next();
    }
}

void Settings::LoadFiles()
{
    Token setting( File::LoadFile("Data/settings") );
    while(setting.Next() != "#endToken")
    {
        if( setting == "#texture" )
        {
            string path = setting.Next();
            string name = setting.Next();
            Texture::LoadTexture(path,name);
        }
    }
}

bool Settings::isObject(string n)
{
    for(ObjectInfo& ob : objectInfos)
        if(n == ob.name)
            return true;

    return false;
}

string Settings::gPrefabPath(string n)
{
    for(ObjectInfo& ob : objectInfos)
        if(n == ob.name)
            return ob.prefabPath;

    return "";
}
