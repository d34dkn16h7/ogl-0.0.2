#ifndef PHYSICS_H
#define PHYSICS_H

#include <glm/glm.hpp>
#include <vector>
#include "component.h"

using namespace std;
using namespace glm;

struct Force
{
    vec3 totalForce;

    Force(vec3 tf) : totalForce(tf) {}

    virtual bool hasForce()
    {
        return (totalForce.x + totalForce.y + totalForce.z) > 0;
    }
};

struct forcePush: Force
{
    vec3 remainForce;
    float speed;

    forcePush(vec3 v,float s) : Force(v) , remainForce(v) , speed(s) {}

    virtual bool hasForce()
    {
        return (remainForce.x + remainForce.y + remainForce.z) > 0;
    }
};

struct forceTimed : Force
{
    float time;
    forceTimed(vec3 v, float t) :  Force(v) , time(t) {}

    virtual bool hasForce()
    {
        return time > 0;
    }
};

class Collider2d;
class GameObject;

class Physics : public Component
{
private:
    static vector<Physics*> physics; /// List of registered objects

    vec3 constForce; /// Const aplied force

    bool canPush, /// Can push?
         canBePushed, /// Can be pushed?
         isGrounded; /// Is grounded? Setted by collider in update

    Collider2d* collider; /// Collider referance

    vector<forcePush> forcePList; /// List of registered objects
public:
    bool isStatic = false;
    Physics(GameObject*);
    virtual ~Physics(); /// Call UnReg()
    void Start();

    void Jump(); /// Player Func move it later
    void Move(vec2);
    void Move(vec3); /// Move by vec3 if not colliding
    void Push(vec3 totalForce, float speed = .1f );
    void AddConstantForce(vec3);

    void Update(); /// Update this object
    static void UpdateAll(); /// Send update to all registered objects

    static void Reg(Physics*); /// Add this object to update vector
    static void UnReg(Physics*); /// Remove this object from update vector
};

#endif // PHYSICS_H
