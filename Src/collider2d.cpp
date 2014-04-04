#include <typeinfo>
#include "collider2d.h"
#include "gameObject.h"
/*
 * Only x and y
 */
vector<Collider2d*> Collider2d::colliders;

Collider2d::Collider2d(GameObject* own) :
    Collider2d(own , Rect(1,1) ) {}

Collider2d::Collider2d(GameObject* own , Rect r) :
    Component(typeid(this).hash_code() , own) , xside(SIDE_NONE) , yside(SIDE_NONE)
{
    mRect = rect = r;
    colliders.push_back(this);
}

Collider2d::~Collider2d()
{
    for(unsigned int i = 0; i < colliders.size(); i++)
        if(colliders[i] == this)
            colliders.erase(colliders.begin() + i);
}

void Collider2d::sSize(Rect r)
{
    mRect = rect = r;
}

void Collider2d::sOffset(vec3 offset)
{
    rect.AddOffset(offset);
}

GameObject* Collider2d::Get(vec3 pos) /// GetAll.first()
{
    vector<GameObject*> val = GetAll(pos);
    if(!val.empty())
        return val.front();
    else
        return nullptr;
}

vector<GameObject*> Collider2d::GetAll(vec3 pos) /// Get all colliders who intesect with vec3
{
    vector<GameObject*> val;
    for(Collider2d* c : colliders)
    {
        Rect r( c->rect );
        r.Scale( c->m_owner->transform.gScale());
        r.AddOffset( c->m_owner->transform.gPosition());

        if( pos.x > r.xmi && pos.x < r.xma )
            if( pos.y > r.ymi && pos.y < r.yma )
                val.push_back(c->m_owner);
    }

    return val;
}

vector<ColliderHit> Collider2d::Intersect() /// Wrapper to Intersect( Collider2d* )
{
    return Intersect(this);
}

vector<ColliderHit> Collider2d::UpdateStatus()
{
    vector<ColliderHit> val;
    for(Collider2d* c : colliders)
    {
        if(nullptr == c) /// Stop colliding with yourself!
            continue;
    }

    return val;
}

vector<ColliderHit> Collider2d::Intersect( Collider2d* target ) /// Wrapper to Intersect( Collider2d* , vec3 )
{
    return Intersect( target, target->m_owner->transform.gPosition() );
}

vector<ColliderHit> Collider2d::Intersect(vec3 nPos) /// Wrapper to Intersect( Collider2d* , vec3 )
{
    return Intersect(this , nPos);
}

bool isIn(float val,float minF,float maxF) /// Done
{
    return val >= minF && val <= maxF;
}

bool isIn(vec2 val,vec2 val2) /// WIP
{
    return false;
}

/// Very WIP
vector<ColliderHit> Collider2d::Intersect( Collider2d* target , vec3 uPos ) /// Intersect
{
    vector<ColliderHit> val;
    Rect r(target->rect);
    r.Scale(target->m_owner->transform.gScale());
    r.AddOffset(uPos);

    target->xside = target->yside = SIDE_NONE;

    for(Collider2d* c : colliders)
    {
        if(target == c) /// Stop colliding with yourself!
            continue;

        target->xside = target->yside = SIDE_NONE;

        Rect cr( c->rect);
        cr.Scale(c->m_owner->transform.gScale());
        cr.AddOffset(c->m_owner->transform.gPosition());

        bool right  = isIn(r.xmi,cr.xmi,cr.xma);
        bool left   = isIn(r.xma,cr.xmi,cr.xma);
        bool top    = isIn(r.ymi,cr.ymi,cr.yma);
        bool bottom = isIn(r.yma,cr.ymi,cr.yma);

        bool yBoth  = top && bottom;
        bool xBoth  = right && left;

        if(top || bottom )
            if (right || left)
            {
                vec2 d(0,0);

                if(xBoth)
                    d.x = 0;
                else if(left)
                    d.x = cr.xmi - r.xma;
                else
                    d.x = cr.xma - r.xmi;

                if(yBoth)
                    d.y = 0;
                else if(top)
                    d.y = cr.yma - r.ymi;
                else
                    d.y = cr.ymi - r.yma;

                val.push_back( ColliderHit(c,d) );
                target->xside = xBoth ? SIDE_ALL : (right ? SIDE_RIGHT : SIDE_LEFT);
                target->yside = yBoth ? SIDE_ALL : (top ? SIDE_TOP : SIDE_BOTTOM);
            }
    }

    return val;
}
