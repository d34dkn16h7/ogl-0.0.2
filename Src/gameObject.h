#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <stdexcept>
#include <typeinfo>
#include "tools.h"
#include "geometry.h"
#include "component.h"
#include "transform.h"

using namespace std;
using namespace Tools;

/*
 * Fix render stuff
 */

class Geometry;
class Physics;
class Component;
class GameObject : public Geometry
{
private:
    vector<Component*> components; /// All components
    void LoadPrefab(string); /// Load and make prefab by nameToken
public:
    bool isActive;
    Physics* physics = nullptr;
    Transform transform; /// Position,Rotation,Scale

    GameObject(string);
    virtual ~GameObject();

    template <typename compType> void AddComponent() /// Add Component to this object
        {components.push_back( new compType(this) );}

    template <typename compType> static void AddComponent(GameObject* gmo) /// Add Component to object
        {gmo->components.push_back( new compType(gmo) );}

    template <typename compType>
    compType* GetComponent() /// Get Component
    {
        for(auto c : components)
            if(c->GetType() == typeid(compType*).hash_code() )
                return (compType*)c;

        return nullptr;
    }

    unsigned int ComponentCount();
    void DestroyComponents(); /// Delete all components
};

#endif // GAMEOBJECT_H
