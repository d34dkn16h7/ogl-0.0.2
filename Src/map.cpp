#include "map.h"
#include "game.h"
#include "tools.h"
#include "gameObject.h"

using namespace Tools;

Map* Map::ins; /// Current map instance

void Map::LoadMap(string fSrc) /// Load and construct gameObjects by .mp file
{
    ins = this;
    Token mapToken( File::LoadFile(fSrc) );
    GameObject *gmo = nullptr;
    data.clear();
    while( mapToken.Next() != "#endToken" )
    {
        if( Settings::isObject( mapToken.Current() )) /// Make object?
        {
            gmo = new GameObject(  mapToken.Current() );
            data.push_back( gmo );
        }
        else if(gmo != nullptr) /// Property if not empty object?
        {
            if(mapToken == "onControl" ) Game::player = gmo;
            else if(mapToken == "pos" ) gmo->transform.uPosition( mapToken.GetNVec3() );
            else if(mapToken == "rot" ) gmo->transform.uRotation( mapToken.GetNVec3() );
            else if(mapToken == "scl" ) gmo->transform.uScale( mapToken.GetNVec3() );
            else if(mapToken == "texture" ) gmo->m_tex.SetTexture( mapToken.Next() );
            else if(mapToken == "staticObject") gmo->DestroyComponents();
        }
    }
}

void Map::SaveMap(string tFile) /// Save map file
{
    std::stringstream mstr;
    for(GameObject* gmo : data)
    {
        mstr << gmo->gPtr->info.modelId << endl;

        Str::AddHashStreamVec3(mstr ,"pos" , gmo->transform.gPosition() );
        Str::AddHashStreamVec3(mstr ,"rot" , gmo->transform.gRotation() );
        Str::AddHashStreamVec3(mstr ,"scl" , gmo->transform.gScale() );

        if(gmo == Game::player) mstr << "\tonControl" << endl;
        if(gmo->ComponentCount() < 1) mstr << "\tstaticObject" << endl;
    }
    File::SaveFile(tFile,mstr.str());
    printf("Map Saved : %s \n\tMap Size : %i\n",tFile.c_str(),data.size());
    //cout << "Map saved : " << tFile << "  :: Map Size : " << data.size() << endl;
}

void Map::Reg(GameObject* gmo) /// Register gameObject to map
{
    data.push_back(gmo);
}

void Map::UnReg(GameObject* gmo) /// Remove gameObject form map
{
    for(unsigned int i = 0 ; i < data.size();i++)
        if(gmo == data[i])
            data.erase(data.begin() + i);
}
