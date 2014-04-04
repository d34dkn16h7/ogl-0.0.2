#include <typeinfo>
#include "physics.h"
#include "collider2d.h"
#include "gameObject.h"
#include "component.h"

vector<Physics*> Physics::physics; /// List of registered objects

Physics::Physics(GameObject* owner) :
    Component(typeid(this).hash_code() , owner) , constForce( vec3(0,0,0) ) , isGrounded(false)
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

void Physics::Move(vec2 val)
{
    Move(vec3(val.x,val.y,0));
}

void Physics::Move(vec3 val) /// Move by val if not colliding
{
    vec3 cPos = m_owner->transform.gPosition() + val;

    if(collider != nullptr)
    {
        vector<ColliderHit> cl = collider->Intersect(cPos);

        if(cl.size() == 0)
        {
            m_owner->transform.uPosition(cPos);
            isGrounded = false;
        }
        else
        {
            Collider2d *tCld = cl[0].collider;
            vec2 d = cl[0].dist;
            d.x = 0;
            for(ColliderHit c : cl)
            {
                if(gPositive(c.dist.y) < gPositive(d.y))
                {
                    tCld = c.collider;
                    d.y = c.dist.y;
                }
            }

            vec3 v = m_owner->transform.gPosition();
            Rect r( tCld->rect );
            r.Scale( tCld->m_owner->transform.gScale());
            r.AddOffset( tCld->m_owner->transform.gPosition());
            m_owner->transform.uPosition( vec3(v.x,r.yma + .001f + m_owner->transform.gScale().y,v.z) );
            isGrounded = true;
            //m_owner->transform.aPosition(d * .5f);
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

        if(!isGrounded && collider->yside == SIDE_NONE && !isStatic && tForce == constForce)
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
