#include "input.h"
#include "camera.h"
#include "renderer.h"

bool Input::mKeyState[ MOUSE_KEY_COUNT ];
bool Input::mKeyStateRelased[ MOUSE_KEY_COUNT ];
bool Input::mKeyStatePressed[ MOUSE_KEY_COUNT ];

bool Input::keyState[ KEY_COUNT ];
bool Input::keyStateRelased[ KEY_COUNT ];
bool Input::keyStatePressed[ KEY_COUNT ];

vec2 Input::mouseWDelta,Input::lastPos,Input::mouseDelta,Input::mouseWheel,Input::lastMouseWheel;

void Input::Init()
{
    lastPos = MousePos();
    glfwSetKeyCallback( Renderer::gWindow(), Input::Keyboard ); // No need for now.
    glfwSetMouseButtonCallback( Renderer::gWindow() , Input::MouseKeys );
    glfwSetScrollCallback( Renderer::gWindow() , Input::MouseScroll );
}
void Input::Update()
{
    UpdateMouse();
}
void Input::Keyboard(GLFWwindow* w, int key, int scancode, int action, int mods)
{
    keyState[key] = keyStatePressed[key] = ( action == GLFW_PRESS );
    keyStateRelased[key] = ( action == GLFW_RELEASE );
}
void Input::MouseKeys(GLFWwindow* window, int key, int action, int mods)
{
    mKeyState[key] = mKeyStatePressed[key] = ( action == GLFW_PRESS );
    mKeyStateRelased[key] = ( action == GLFW_RELEASE );
}
void Input::MouseScroll(GLFWwindow* w,double x, double y)
{
    mouseWheel += vec2(x,y);
}

void Input::UpdateMouse()
{
    mouseDelta = MousePos() - lastPos;
    lastPos = MousePos();

    mouseWDelta = mouseWheel - lastMouseWheel;
    lastMouseWheel = mouseWheel;
}

bool Input::isMouse(int key)
{
    return mKeyState[key];
}

bool Input::isMousePressed(int key)
{
    bool val = mKeyStatePressed[key];
    mKeyStatePressed[key] = false;
    return val;
}

bool Input::isMouseRelased(int key)
{
    bool val = mKeyStateRelased[key];
    mKeyStateRelased[key] = false;
    return val;
}

bool Input::isKey(int key)
{
    return glfwGetKey(Renderer::gWindow(),key);
}

bool Input::isKeyPressed(int key)
{
    bool val = keyStatePressed[key];
    keyStatePressed[key] = false;
    return val;
}

bool Input::isKeyRelased(int key)
{
    bool val = keyStateRelased[key];
    keyStateRelased[key] = false;
    return val;
}

vec2 Input::MousePos()
{
    double x,y;
    glfwGetCursorPos(Renderer::gWindow() ,&x,&y);
    vec2 val = vec2(x,y);
    return val;
}

vec2 Input::ScreenToWorld2d()
{
    vec2 cpos = MousePos();
    bool xneg = false,yneg = false;
    int hX = Camera::MainCamera->ScreenSize.x / 2,hY = Camera::MainCamera->ScreenSize.y / 2;

    if(cpos.x > hX)
    {
        cpos.x -= hX;
        xneg = true;
    }
    if(cpos.y > hY)
    {
        cpos.y -= hY;
        yneg = true;
    }

    cpos.x = cpos.x / hX;
    cpos.y = cpos.y / hY;
    cpos.y = (1 - cpos.y);

    if(!xneg) cpos.x = (-1 + cpos.x);
    if(yneg) cpos.y = (-1 + cpos.y );

    return cpos;
}

vec3 Input::ScreenToWorld3d()
{
    vec2 s2 = ScreenToWorld2d();
    //vec3 val = vec3(s2,0);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    s2.y = viewport[3] - s2.y;
    float depth;
    glReadPixels(s2.x, s2.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

    vec3 mousePos = glm::unProject(vec3(s2, depth), mat4(1.0), Camera::MainCamera->GetProjection(), vec4(viewport[0], viewport[1], viewport[2], viewport[3]));

    return mousePos;
}
