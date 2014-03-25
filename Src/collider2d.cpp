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
    Component(typeid(this).hash_code() , own)
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
        r.Scale( c->owner->transform.gScale());
        r.AddOffset( c->owner->transform.gPosition());

        if( pos.x > r.xmi && pos.x < r.xma )
            if( pos.y > r.ymi && pos.y < r.yma )
                val.push_back(c->owner);
    }

    return val;
}

vector<Collider2d*> Collider2d::Intersect() /// Wrapper to Intersect( Collider2d* )
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

vector<Collider2d*> Collider2d::Intersect( Collider2d* target ) /// Wrapper to Intersect( Collider2d* , vec3 )
{
    return Intersect( target, target->owner->transform.gPosition() );
}

vector<Collider2d*> Collider2d::Intersect(vec3 nPos) /// Wrapper to Intersect( Collider2d* , vec3 )
{
    return Intersect(this , nPos);
}

bool isIn(float val,float minF,float maxF) /// Done
{
    return val > minF && val < maxF;
}

bool isIn(vec2 val,vec2 val2) /// WIP
{
    return false;
}

/// Very WIP
vector<Collider2d*> Collider2d::Intersect( Collider2d* target , vec3 uPos ) /// Intersect
{
    vector<Collider2d*> val;
    Rect r(target->rect);
    r.Scale(target->owner->transform.gScale());
    r.AddOffset(uPos);


    target->xsd = xSideN::None;
    target->ysd = ySideN::None;

    for(Collider2d* c : colliders)
    {
        if(target == c) /// Stop colliding with yourself!
            continue;

        Rect cr( c->rect);
        cr.Scale(c->owner->transform.gScale());
        cr.AddOffset(c->owner->transform.gPosition());

        bool yBoth  = r.yma > cr.yma && r.ymi < cr.ymi;
        bool xBoth  = r.xma > cr.xma && r.xmi < cr.xmi;
        bool right  = isIn(r.xmi,cr.xmi,cr.xma);
        bool left   = isIn(r.xma,cr.xmi,cr.xma);
        bool top    = isIn(r.ymi,cr.ymi,cr.yma);
        bool bottom = isIn(r.yma,cr.ymi,cr.yma);

        if(top || bottom )
            if (right || left)
            {
                val.push_back(c);
                target->xsd = xBoth ? xSideN::Both : (right ? xSideN::Right : xSideN::Left);
                //target->ysd = yBoth ? ySideN::Both : (top ? ySideN::Top : ySideN::Bottom);
                target->ysd = top == bottom ? ySideN::Both : (top ? ySideN::Top : ySideN::Bottom);
            }
    }

    return val;
}
