#include <typeinfo>
#include "physics.h"
#include "collider2d.h"
#include "gameObject.h"
#include "component.h"

vector<Physics*> Physics::physics; /// List of registered objects

Physics::Physics(GameObject* owner) :
    Component(typeid(this).hash_code() , owner) , constForce( vec3(0,0,0) )
{
    m_owner->physics = this;
    collider = m_owner->AddComponent<Collider2d>();
    Reg(this);
    Start();
}

Physics::~Physics() /// Unregister from update list
{
    UnReg(this);
}

void Physics::Start() {}

void Physics::Jump()
{
    //if(collider->c2 == Side::Both || collider->c2 == Side::Bottom )
    Push( vec3(0,5.5f,0), .005f );
}

float gPositive(float f)
{
    return f > 0 ? f : -f;
}

void Physics::Move(vec3 val) /// Move by val if not colliding
{
    vec3 cPos = m_owner->transform.gPosition() + val;

    if(collider != nullptr)
    {
        vector<ColliderHit> cl = collider->Intersect(cPos);

        /*bool xMinus,xPlus,yMinus,yPlus;
        xMinus = xPlus = yMinus = yPlus = false;
        for(Collider* c : cl)
        {
            if(c->c1 == Side::Right)
                xPlus = true;
            else if(c->c1 == Side::Left)
                xMinus = true;

            if(c->c2 == Side::Top)
                yPlus = true;
            else if(c->c2 == Side::Bottom)
                yMinus = true;
        }

        cout <<"xMinus " <<  xMinus << endl;
        cout <<"xPlus " << xPlus << endl;*/

        if(cl.size() == 0)
            m_owner->transform.uPosition(cPos);
        else
        {
            vec2 d = cl[0].dist;
            for(ColliderHit c : cl)
            {
                if(gPositive(c.dist.x) < d.x)
                    d.x = c.dist.x;

                if(gPositive(c.dist.y) < d.y)
                    d.y = c.dist.y;
            }

            /*
            if(owner->nameToken == "player")
            {
                cout << d.x << ":" << d.y << endl;
            }*/

            if(gPositive(cPos.x) < gPositive(d.x))
                d.y = 0;
            if(gPositive(cPos.y) < gPositive(d.y))
                d.y = 0;

            m_owner->transform.aPosition( vec3(d.x,d.y,0) );
        }
    }
    else /// Don't have collider so just move
        m_owner->transform.uPosition(cPos);
}

void Physics::Push(vec3 totalForce, float speed) /// Empty
{
    forcePList.push_back( forcePush( totalForce, speed) );
}

void Physics::AddConstantForce(vec3 force) /// Add force to constForce
{
    constForce += force;
}

void Physics::Update() /// Update this object
{
    if(m_owner->active && active)
    {
        vec3 tForce = constForce;

        for(unsigned int i = 0;i < forcePList.size();i++)
        {
            forcePush* fp = &forcePList[i];
            vec3 currForce = (fp->totalForce * fp->speed);
            fp->remainForce -= currForce;
            tForce += currForce;

            if(!fp->hasForce())
                forcePList.erase(forcePList.begin() + i);
        }

        if(collider->ysd == ySideN::None && !isStatic && tForce == constForce)
            tForce = vec3(0,-.005f,0);

        /// Apply force!
        if(tForce != vec3(0,0,0))
            Move(tForce);
    }
}

void Physics::UpdateAll() /// Send update to all registered objects
{
    for(Physics* p : physics)
        p->Update();
}

void Physics::Reg(Physics* val) /// Add this object to update vector
{
    physics.push_back(val);
}

void Physics::UnReg(Physics* val)/// Remove this object from update vector
{
    for(unsigned int i = 0;i < physics.size();i++)
        if(physics[i] == val)
            physics.erase(physics.begin() + i);
}
