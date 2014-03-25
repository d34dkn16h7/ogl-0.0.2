#ifndef MAP_H
#define MAP_H

#include <glm/glm.hpp>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using namespace glm;

class GameObject;
class Map
{
private:
public:
    vector<GameObject*> data;
    static Map* ins; /// Current map instance
    void LoadMap( string); /// Load and construct gameObjects by .mp file
    void SaveMap( string); /// Save map file
    void Reg( GameObject*); /// Register gameObject to map
    void UnReg( GameObject*); /// Remove gameObject form map
};

#endif // MAP_H
