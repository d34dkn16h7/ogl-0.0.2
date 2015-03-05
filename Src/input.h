#ifndef INPUT_H
#define INPUT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#define MOUSE_KEY_COUNT 3
#define KEY_COUNT 999

using namespace glm;

class Input
{

    static vec2 lastPos;
    static vec2 lastMouseWheel;
    static vec2 mouseWheel;

    static bool keyState[KEY_COUNT];
    static bool keyStateRelased[KEY_COUNT];
    static bool keyStatePressed[KEY_COUNT];

    static bool mKeyState[MOUSE_KEY_COUNT];
    static bool mKeyStateRelased[MOUSE_KEY_COUNT];
    static bool mKeyStatePressed[MOUSE_KEY_COUNT];
    //func
    static void UpdateMouse();
public:
    static void MouseKeys(GLFWwindow*,int,int,int);
    static void MouseScroll(GLFWwindow*,double,double);
    static void Keyboard(GLFWwindow*,int,int,int,int);

    static vec2 mouseDelta;
    static vec2 mouseWDelta;

    static void Init();
    static void Update();


    static bool isMouse(int);
    static bool isMousePressed(int);
    static bool isMouseRelased(int);

    static bool isKey(int);
    static bool isKeyPressed(int);
    static bool isKeyRelased(int);

    static vec2 MousePos();
    static vec2 ScreenToWorld2d();
    static vec3 ScreenToWorld3d();
};

#endif // INPUT_H
