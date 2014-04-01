#include "game.h"
#include "tools.h"
#include "editor.h"
#include "camera.h"
#include "program.h"
#include "geometry.h"
#include "renderer.h"
#include "gameObject.h"


const string windowName = "oGL 3.2 -0.0.2";

GLFWwindow* Renderer::window;
Camera* Renderer::cam;
Program* Renderer::prog;
vector<GameObject*> Renderer::drawObjects;

void Renderer::RenderObjects() /// Render all objects
{
    unsigned int edgesCount = 0;
    GLenum type = GL_TRIANGLES;
    string lastTexture = "null";
    string lastDrawName = "null";
    prog->SetUniform("cameraMatrix",cam->GetMatrix());
    for(GameObject* gmo : drawObjects)
    {
        glPolygonMode( GL_FRONT_AND_BACK, Game::ins->editor->isSelected( gmo ) ? GL_LINE : GL_FILL);

        prog->SetUniform("modelMatrix",gmo->transform.gMatrix());

        prog->Use(true);

        glBindVertexArray( gmo->gPtr->gVAO() );
        if(lastDrawName != gmo->gPtr->info.modelId) /// lastDraw != thisDraw re-bind data
        {
            edgesCount = gmo->gPtr->gEdgeCount();
            type = gmo->gPtr->gType();
            lastDrawName = gmo->gPtr->info.modelId;
        }
        if(lastTexture != gmo->m_tex.m_name)
        {
            glActiveTexture (GL_TEXTURE0);

            if(gmo->m_tex.tex != 0)
                glBindTexture (GL_TEXTURE_2D, gmo->m_tex.tex );
            else
                glBindTexture (GL_TEXTURE_2D, 0);

            lastTexture = gmo->m_tex.m_name;
        }
        glDrawArrays(type,0,edgesCount);
        glBindVertexArray( 0 );
    }
    /// Clear bindings
    prog->Use(false);
    glBindTexture (GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void Renderer::RenderAll() /// Call all render functions
{
    glClearColor(0,0,0,1); // (1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(Tools::Settings::isModernGL)
        RenderObjects();
    else
        DrawLegacy();

    glfwSwapBuffers( window );
}

bool Renderer::Setup(int w,int h) /// Setup GLFW - GLEW + Window + Shaders
{
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);

    if(Tools::Settings::isModernGL)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    window = glfwCreateWindow(w, h, windowName.c_str(), 0, 0);
    if(!window)
    {
        glfwWindowHint(GLFW_OPENGL_PROFILE,0);
        window = glfwCreateWindow(w, h, windowName.c_str(), 0, 0);
        if(!window)
        {
            cout << "glfwCreateWindow() Fail" << endl;
            return false;
        }
        cout << "GLFW_OPENGL_PROFILE -> 0" << endl;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK)
    {
        cout << "GLEW Error : " << glewGetErrorString(glewStatus) << endl;
        return false;
    }
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    cam = new Camera(w,h);
    if(Tools::Settings::isModernGL)
        prog = new Program(Tools::Settings::vertexShaderFileName,Tools::Settings::fragmentShaderFileName,"Model");

    PrintRendererInfo();

    return true;
}

void Renderer::PrintRendererInfo() /// Print main info
{
    int iOpenGLMajor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    int iOpenGLMinor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    int iOpenGLRevision = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
    cout << "Status: GLFW Version " << glfwGetVersionString() << endl;
    cout << "Status: OpenGL Version: " << iOpenGLMajor << '.' << iOpenGLMinor << " Revision " << iOpenGLRevision << endl;
    cout << "Status: GLEW Version " << glewGetString(GLEW_VERSION) << endl;
}

void Renderer::RegObject(GameObject *obj) /// Register gameObject to draw
{
    drawObjects.push_back(obj);
}

void Renderer::UnRegObject(GameObject *obj) /// Remove gameObject from draw
{
    for(unsigned int i = 0;i < drawObjects.size();i++)
        if(drawObjects[i] == obj)
            drawObjects.erase(drawObjects.begin() + i);
}

GLFWwindow* Renderer::gWindow() /// Return GLFWwindow* handler
{
    return window;
}

void Renderer::DrawLegacy() /// NOT WORKING
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
    for(GameObject* gmo : drawObjects)
    {
        glLoadIdentity();
        //glTranslatef(0,0,0);
        vec3 pos = gmo->transform.gPosition();
        glTranslatef(pos.x,pos.y,pos.z);
        pos = Camera::MainCamera->transform.gPosition();
        glTranslatef(pos.x,pos.y,pos.z);

        glBegin(GL_TRIANGLE_STRIP);

        for(unsigned int i = 0;i < gmo->gPtr->verticles.size();i += 3)
        {
            glColor3f( gmo->gPtr->verticles[i],gmo->gPtr->verticles[i + 1],gmo->gPtr->verticles[i + 2]);
            glVertex3f( gmo->gPtr->verticles[i],gmo->gPtr->verticles[i + 1],gmo->gPtr->verticles[i + 2]);
        }

        glEnd();
    }
}
