#ifndef COMPONENT_H
#define COMPONENT_H

#include <vector>
#include <typeinfo>

using namespace std;

class GameObject;

class Component
{
private:
    size_t typeHash;
public:
    bool active;
    GameObject* m_owner;

    Component(size_t type ,GameObject* owner) : typeHash(type) ,active(true) ,m_owner(owner)  {}
    virtual ~Component() {}

    virtual void Start() = 0;
    virtual void Update() = 0;
    virtual void UnReg() {}
    size_t GetType() {return typeHash;}
};

class Components /// Manager of Component
{
private:
    GameObject* m_owner;
    vector<Component*> m_components;
public:
    Components(GameObject* owner) : m_owner(owner) {}

    template <typename compType>
    compType* AddComponent() /// Add Component and return the pointer of it
    {
        compType* tComp = new compType(m_owner);
        m_components.push_back(tComp);
        return tComp;
    }

    template <typename compType>
    compType* GetComponent() /// Get Component
    {
        for(auto tCmp : m_components)
            if(tCmp->GetType() == typeid(compType*).hash_code() )
                return (compType*)tCmp;

        return nullptr;
    }

    template <typename compType>
    bool HasComponent()
    {
        for(auto tCmp : m_components)
            if(tCmp->GetType() == typeid(compType*).hash_code() )
                return true;

        return false;
    }

    unsigned int ComponentCount()
    {
        return m_components.size();
    }

    void DestroyComponents()
    {
        for(Component* c : m_components)
            delete c;

        m_components.clear();
    }
};

#endif // COMPONENT_H
