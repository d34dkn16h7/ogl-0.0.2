#include "game.h"
#include "tools.h"
#include "editor.h"
#include "camera.h"
#include "program.h"
#include "geometry.h"
#include "renderer.h"
#include "gameObject.h"

const string windowName = "openGL 3.2 - 0.0.2";

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
        glPolygonMode( GL_FRONT_AND_BACK, Editor::isSelected( gmo ) ? GL_LINE : GL_FILL);

        prog->SetUniform("modelMatrix",gmo->transform.gMatrix());

        prog->Use(true);

        glBindVertexArray( gmo->gPtr->gVAO() );
        if(lastDrawName != gmo->gPtr->info.modelId) /// lastDraw != thisDraw re-bind data
        {
            edgesCount = gmo->gPtr->gEdgeCount();
            type = gmo->gPtr->gType();
            lastDrawName = gmo->gPtr->info.modelId;
        }
        if(lastTexture != gmo->m_tex->m_name)
        {
            glActiveTexture (GL_TEXTURE0);

            if(gmo->m_tex->tex != 0)
                glBindTexture (GL_TEXTURE_2D, gmo->m_tex->tex );
            else
                glBindTexture (GL_TEXTURE_2D, 0);

            lastTexture = gmo->m_tex->m_name;
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

    RenderObjects();

    glfwSwapBuffers( window );
}

void Renderer::Setup(int w,int h) /// Setup GLFW - GLEW + Window + Shaders , throws on fail
{
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(w, h, windowName.c_str(), 0, 0);
    if(!window)
    {
        Tools::Logger::Error("GLFW -> CreateWindow() Fail 1");

        glfwWindowHint(GLFW_OPENGL_PROFILE,0);
        window = glfwCreateWindow(w, h, windowName.c_str(), 0, 0);
        if(!window)
            Tools::Logger::FatalError("GLFW -> CreateWindow() Fail 2");
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    GLenum glewStatus = glewInit();

    if (glewStatus != GLEW_OK)
        Tools::Logger::FatalError("GLEW -> " + string((char*)glewGetErrorString(glewStatus)));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    cam = new Camera(w,h);
    prog = new Program(Tools::Settings::vertexShaderFileName,Tools::Settings::fragmentShaderFileName,"Model");

    printRendererInfo();
}

void Renderer::printRendererInfo() /// Print main info
{
    int iOpenGLMajor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    int iOpenGLMinor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    int iOpenGLRevision = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
    stringstream strs;
    strs << "Renderer Info" << endl;
    strs << "\t" << "Status: GLFW Version " << glfwGetVersionString() << endl;
    strs << "\t" << "Status: OpenGL Version: " << iOpenGLMajor << '.' << iOpenGLMinor << " Revision " << iOpenGLRevision << endl;
    strs << "\t" << "Status: GLEW Version " << glewGetString(GLEW_VERSION) << endl;

    Tools::Logger::Info(strs.str());
}

void Renderer::RegObject(GameObject *obj) /// Register gameObject to draw
{
    drawObjects.push_back(obj);
}

void Renderer::UnRegObject(GameObject *obj) /// Remove gameObject from draw
{
    for(unsigned int i = 0;i < drawObjects.size(); i++)
        if(drawObjects[i] == obj)
            drawObjects.erase(drawObjects.begin() + i);
}

GLFWwindow* Renderer::gWindow() /// Return GLFWwindow* handler
{
    return window;
}
