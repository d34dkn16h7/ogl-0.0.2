#ifndef COLLIDER_H
#define COLLIDER_H

#include <glm/glm.hpp>
#include <vector>
#include "tools.h"
#include "component.h"

namespace xSideN
{
    enum xSide
    {
        None,Both,Left,Right
    };
}
namespace ySideN
{
    enum ySide
    {
        None,Both,Top,Bottom
    };
}

using namespace std;
using namespace glm;
using namespace Tools;
using namespace ySideN;
using namespace xSideN;

struct ColliderHit;
class GameObject;
class Collider2d : public Component
{
private:
    static vector<Collider2d*> colliders;

    Rect mRect;
public:
    xSide xsd;
    ySide ysd;

    Rect rect;

    Collider2d(GameObject*);
    Collider2d(GameObject* , Rect);
    ~Collider2d();

    void Start() {}
    void Update() {}

    void sSize(Rect);
    void sOffset(vec3);
    static GameObject* Get(vec3);
    static vector<GameObject*> GetAll(vec3);

    vector<ColliderHit> Intersect();
    vector<ColliderHit> Intersect( vec3 );
    static vector<ColliderHit> Intersect( Collider2d* );
    static vector<ColliderHit> Intersect( Collider2d* , vec3);


    vector<ColliderHit> UpdateStatus();
};

struct ColliderHit
{
    vec2 dist;
    Collider2d* collider;
    ColliderHit() : collider(nullptr) {}
    ColliderHit(Collider2d* c,vec2 d = vec2(0,0) ) : collider(c) , dist(d) {}
};

#endif // COLLIDER_H
