#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Camera;
class Program;
class Geometry;
class GameObject;

class Renderer
{
private:
    static GLFWwindow* window;
    static Camera *cam;
    static Program *prog;
    static vector<GameObject*> drawObjects;
public:
    static bool Setup(int,int);
    static void PrintRendererInfo();

    static void RenderAll();
    static void RenderObjects();
    static void DrawLegacy(); /// Not Working

    static void RegObject(GameObject*);
    static void UnRegObject(GameObject*);

    static GLFWwindow* gWindow();
};

#endif // RENDERER_H
