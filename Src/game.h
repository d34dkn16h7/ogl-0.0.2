#ifndef GAME_H
#define GAME_H

class Editor;
class Physics;
class GameObject;

class Game
{
public:
    static GameObject* onControl;
    static GameObject* player;
    static Physics* p_onControl;

    static float deltaTime,lastTime;
    Game();
    int Run();
    void Update();
    void Timer();

    void player_Input(); /// Remove later
};

#endif // GAME_H
