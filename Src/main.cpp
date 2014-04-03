#include "game.h"

/*
Global Lists:
    Renderer   -> drawList (geo)
    Geometry   -> pointer to main data (geo)
    Collider   -> colliders (colliders with gmo owners)
    GameObject -> gameObjects (all objects)
*/

/*
To Do:
    physics - (pre-pre-pre alpha level) - Done!
    prefabs - (very basic) - Just need to store it
    geometry store - Done!
    gui - freshly started - canceled for now
    colliders -> ----!
*/

int main()
{
    return ( new Game() )->Run();
}
