#include "gameObject.h"
#include "renderer.h"
#include "collider2d.h"
#include "physics.h"
#include "tools.h"
#include "map.h"
#include <iostream>

const static string DataDir = "Data/";

GameObject::GameObject(string name) : isActive(true)
{
    nameToken = name;
    LoadPrefab( Settings::gPrefabPath(nameToken) );
    Renderer::RegObject(this);
}

GameObject::~GameObject()
{
    Map::ins->UnReg(this);
    Renderer::UnRegObject(this);
    DestroyComponents();
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
                m_tex.SetTexture( token.Next() );

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

unsigned int GameObject::ComponentCount()
{
    return components.size();
}

void GameObject::DestroyComponents() /// Delete all components
{
    for(Component* c : components)
        delete c;

    components.clear();
}
