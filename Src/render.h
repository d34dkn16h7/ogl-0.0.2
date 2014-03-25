#ifndef RENDER_H
#define RENDER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "geometry.h"
#include "component.h"

class Render : public Component
{
    Geometry* geometry;
    GLuint texture;
};

#endif // RENDER_H
