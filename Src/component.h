#ifndef COMPONENT_H
#define COMPONENT_H

#include <typeinfo>
#include <string>
#include <vector>

using namespace std;

class GameObject;
class Component
{
private:
    size_t typeHash;
public:
    GameObject* owner;

    Component(size_t type ,GameObject* own) : typeHash(type) , owner(own) {}
    virtual ~Component() {}

    virtual void Start() = 0;
    virtual void Update() = 0;
    virtual void UnReg() {}
    size_t GetType()
        {return typeHash;}
};

#endif // COMPONENT_H
