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
class GameObject : public Geometry , public Components
{
private:
    static vector<GameObject*> gameObjects;
    void LoadPrefab(string); /// Load and make prefab by nameToken
public:
    bool isActive;
    Physics* physics = nullptr;
    Transform transform; /// Position,Rotation,Scale

    GameObject(string);
    virtual ~GameObject();

    void Reg();
    void UnReg();
    void static Reg(GameObject*);
    void static UnReg(GameObject*);

    void static LoadFromFile(string);
    void static SaveToFile(string);

    static GameObject* Find(string);
};

#endif // GAMEOBJECT_H
