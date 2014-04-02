#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

//#define DBG_GAMEOBJECT_FIND

#include <stdexcept>
#include <typeinfo>
#include "tools.h"
#include "geometry.h"
#include "component.h"
#include "transform.h"

using namespace std;
using namespace Tools;

class Geometry;
class Physics;
class Component;

class GameObject : public Geometry , public Components , public Transformable
{
private:
    static vector<GameObject*> gameObjects;

    void LoadPrefab(string); /// Load and make prefab by nameToken
public:
    bool isActive;
    Transform transform;

    Physics* physics = nullptr; /// for easy access

    GameObject(string);
    virtual ~GameObject();

    void static LoadFromFile(string);
    void static SaveToFile(string);

    static GameObject* Find(string);

    void Reg();
    void static Reg(GameObject*);

    void UnReg();
    void static UnReg(GameObject*);
};

#endif // GAMEOBJECT_H
