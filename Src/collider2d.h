#ifndef COLLIDER_H
#define COLLIDER_H

#include <glm/glm.hpp>
#include <vector>
#include "tools.h"
#include "component.h"

#define SIDE_NONE 0
#define SIDE_ALL 1

#define SIDE_LEFT 2
#define SIDE_RIGHT 3
#define SIDE_TOP 4
#define SIDE_BOTTOM 5

using namespace std;
using namespace glm;
using namespace Tools;

struct ColliderHit;
class GameObject;
class Collider2d : public Component
{
private:
    static vector<Collider2d*> colliders;

    Rect mRect;
public:
    unsigned int xside,yside;

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
    Collider2d* collider = nullptr;
    ColliderHit() : collider(nullptr) {}
    ColliderHit(Collider2d* c,vec2 d = vec2(0,0) ) : dist(d), collider(c) {}
};

#endif // COLLIDER_H
