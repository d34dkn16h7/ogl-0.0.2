#ifndef GAME_H
#define GAME_H

#include <glm/glm.hpp>

using namespace glm;

class Map;
class Editor;
class Physics;
class GameObject;
class Game
{
public:
    static Game* ins;
    static GameObject* onControl;
    static GameObject* player;
    static Physics* p_onControl;
    static float deltaTime,lastTime,Speed;
    static bool isOpen,isEditor;
    Editor* editor;
    Game();
    int Run();
    void Update();
    void Timer();

    void player_Input(); /// Remove later
};

#endif // GAME_H
