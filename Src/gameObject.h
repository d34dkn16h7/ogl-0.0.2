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

class Geometry;
class Physics;
class Component;

class GameObject : public Geometry , public Components , public Transformable
{
private:
    static vector<GameObject*> gameObjects;

    void LoadPrefab(string); /// Load and make prefab by nameToken
public:
    bool active;

    Physics* physics = nullptr; /// for easy access

    GameObject(string);
    virtual ~GameObject();

    static void LoadFromFile(string);
    static void SaveToFile(string);

    static GameObject* Find(string);

    void Reg(); /// Reg (this)
    static void Reg(GameObject*);

    void UnReg(); /// UnReg (this)
    static void UnReg(GameObject*);
};

#endif // GAMEOBJECT_H
