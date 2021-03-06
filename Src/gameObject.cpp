#include "gameObject.h"
#include "renderer.h"
#include "collider2d.h"
#include "physics.h"
#include "tools.h"
#include "game.h"
#include <iostream>

#define DBG

const static string DataDir = "Data/";
vector<GameObject*> GameObject::gameObjects;

void Log_DBG(string);

GameObject::GameObject(string name) : Components(this) , active(true)
{
    Reg();
    nameToken = name;
    LoadPrefab( Settings::gPrefabPath(nameToken) );
    Renderer::RegObject(this);
}

GameObject::~GameObject()
{
    UnReg();
    Renderer::UnRegObject(this);
    DestroyComponents();
}

GameObject* GameObject::Find(string name)
{
    for(unsigned int i = 0 ; i < gameObjects.size(); i++)
    {
        if(name == gameObjects[i]->nameToken)
            return gameObjects[i];
    }

    Tools::Logger::Warning("No '" + name + "' found.");

    return nullptr;
}

void GameObject::Reg()
{
    Reg(this);
}
void GameObject::UnReg()
{
    UnReg(this);
}
void GameObject::Reg(GameObject *gmo)
{
    gameObjects.push_back( gmo );
}
void GameObject::UnReg(GameObject *gmo)
{
    for(unsigned int i = 0 ; i < gameObjects.size(); i++)
        if(gmo == gameObjects[i])
            gameObjects.erase(gameObjects.begin() + i);
}

void GameObject::LoadFromFile(string fSrc)
{
    Token mapToken( File::LoadFile(fSrc) );
    GameObject *gmo = nullptr;
    gameObjects.clear();
    while( mapToken.Next() != "#endToken" )
    {
        if( Settings::isObject( mapToken.Current() )) /// Make object?
        {
            gmo = new GameObject(  mapToken.Current() );
            gameObjects.push_back( gmo );
        }
        else if(gmo != nullptr) /// Property if not empty object?
        {
            if(mapToken == "onControl" ) {Game::player = gmo;gmo->nameToken = "player";}
            else if(mapToken == "pos" ) gmo->transform.uPosition( mapToken.GetNVec3() );
            else if(mapToken == "rot" ) gmo->transform.uRotation( mapToken.GetNVec3() );
            else if(mapToken == "scl" ) gmo->transform.uScale( mapToken.GetNVec3() );
            else if(mapToken == "texture" ) gmo->m_tex = Texture::GetTexture( mapToken.Next() );
            else if(mapToken == "staticObject") gmo->physics->isStatic = true;//gmo->DestroyComponents();
        }
    }
}

void GameObject::SaveToFile(string tFile)
{
    std::stringstream mstr;
    for(GameObject* gmo : gameObjects)
    {
        mstr << gmo->gPtr->info.modelId << endl;

        Str::AddHashStreamVec3(mstr ,"pos" , gmo->transform.gPosition() );
        Str::AddHashStreamVec3(mstr ,"rot" , gmo->transform.gRotation() );
        Str::AddHashStreamVec3(mstr ,"scl" , gmo->transform.gScale() );

        if(gmo == Game::player) mstr << "\tonControl" << endl;
        if(gmo->ComponentCount() < 1) mstr << "\tstaticObject" << endl;
    }
    File::SaveFile(tFile,mstr.str());

    printf("Map Saved : %s \n\tMap Size : %i\n",tFile.c_str(),gameObjects.size());
}

void GameObject::LoadPrefab(string prefPath) /// Load and make prefab by nameToken
{
    StoredToken token( prefPath , nameToken);
    Collider2d* tCollider = nullptr;

    while(token.Next() != Token::EndToken)
    {
        if(token == "model")
            Load( token.Next() , nameToken);

        if(token == "texture" && Settings::loadTextures)
            if(token.Peek(1) != "null")
                m_tex = Texture::GetTexture( token.Next() );

        if(token == "physics") /// Also adds Collider2d
            AddComponent<Physics>();

        if(token == "rot")
            transform.uRotation(token.GetNVec3());

        if(token == "scale")
            transform.uScale(token.GetNVec3());

        if(token == "csize")
            if( (tCollider = GetComponent<Collider2d>()) != nullptr)
                tCollider->sSize(Rect(token.GetNVec3()));

        if(token == "coffset")
            if( (tCollider = GetComponent<Collider2d>()) != nullptr)
                tCollider->sOffset(token.GetNVec3());
    }
}
