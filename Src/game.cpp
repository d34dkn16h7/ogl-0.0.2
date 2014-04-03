#include "game.h"
#include "input.h"
#include "editor.h"
#include "physics.h"
#include "renderer.h"
#include "gameObject.h"

#include "camera.h"
Physics* Game::p_onControl = nullptr;
GameObject* Game::onControl;
GameObject* Game::player;
bool Game::isOpen;
float Game::deltaTime,Game::lastTime;

static vec3 Zero(0,0,0);
static vec3 Up(0,1.0001,0) , Down = -Up;
static vec3 Left(-1.0001,0,0) , Right = -Left;
static vec3 Forward(0,0,-1.0001) , Backward = -Forward;

Game::Game()
{
    Tools::Settings::LoadSettings();
    isOpen = Renderer::Setup(1024,576);
    Tools::Settings::LoadFiles();
    Input::Init();
}

int Game::Run()
{
    GameObject::LoadFromFile(Settings::mapFile);
    lastTime = glfwGetTime();
    onControl = player;
    while( (isOpen && !Input::isKeyRelased(GLFW_KEY_ESCAPE)) && !glfwWindowShouldClose( Renderer::gWindow() ) )
    {
        glfwPollEvents();
        Update();
        Renderer::RenderAll();
        Timer();
    }
    glfwDestroyWindow( Renderer::gWindow() );
    glfwTerminate();
    return 0;
}


void Game::Update() /// Update all
{
    Input::Update();

    Editor::Update();
    Physics::UpdateAll();

    player_Input();
}

float Speed  = 15;
void Game::player_Input() /// Player input - remove it later
{
    if(onControl != nullptr)
    {
        if(p_onControl == nullptr)
        {
            if(onControl->HasComponent<Physics>())
                p_onControl = onControl->GetComponent<Physics>();
            else
                p_onControl = onControl->AddComponent<Physics>();
        }

        if(p_onControl != nullptr)
        {
            if(Input::isKey('W')) p_onControl->Move(Speed * deltaTime * Up);
            if(Input::isKey('S')) p_onControl->Move(Speed * deltaTime * Down);
            if(Input::isKey('A')) p_onControl->Move(Speed * deltaTime * Left);
            if(Input::isKey('D')) p_onControl->Move(Speed * deltaTime * Right);

            if(Input::isKeyPressed(GLFW_KEY_SPACE)) p_onControl->Jump();
        }
    }
    if(Input::isKeyRelased(GLFW_KEY_END))
        GameObject::SaveToFile(Settings::mapFile);

    if(Input::isKeyRelased(GLFW_KEY_F1))
    {
        Camera* c = Camera::MainCamera;
        c->transform.uScale(vec3(15,15,15));
        c->SetCameraType(CameraType::Orthographic);
    }
    if(Input::isKeyRelased(GLFW_KEY_F2))
    {
        Camera* c = Camera::MainCamera;
        c->transform.uScale(vec3(1,1,1));
        c->SetCameraType(CameraType::Perspective);
    }
}

void Game::Timer() /// Update deltaTime
{
    deltaTime = ( glfwGetTime() - lastTime );
    lastTime = glfwGetTime();
}
