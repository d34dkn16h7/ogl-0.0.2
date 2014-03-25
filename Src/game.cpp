#include "map.h"
#include "game.h"
#include "input.h"
#include "editor.h"
#include "physics.h"
#include "renderer.h"
#include "gameObject.h"

#include "camera.h"
Game* Game::ins;
Physics* Game::p_onControl = nullptr;
GameObject* Game::onControl;
GameObject* Game::player;
bool Game::isOpen,Game::isEditor;
float Game::deltaTime,Game::lastTime,Game::Speed;

int Game::Run()
{
    Speed = 15;
    isEditor = true;
    editor->SetTargetMap(map);
    map->LoadMap( Settings::mapFile );
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
#include <string>
Game::Game() : map( new Map() ) , editor( new Editor() )
{
    ins = this;
    Tools::Settings::LoadSettings();
    isOpen = Renderer::Setup(1024,576);
    Tools::Settings::LoadFiles();
    Input::Init();
}

void Game::Update() /// Update all
{
    if( Input::isKeyPressed(GLFW_KEY_HOME))
        isEditor = !isEditor;

    Input::Update();
    Physics::UpdateAll();

    if(isEditor) editor->Update();

    input();
}

void Game::input() /// Player input - remove it later
{
    if(onControl != nullptr)
    {
        if(p_onControl == nullptr)
            p_onControl = onControl->GetComponent<Physics>();

        if(p_onControl != nullptr)
        {
            //if(Input::isKey('W')) p_onControl->Move(Speed * deltaTime * Up);
            //if(Input::isKey('S')) p_onControl->Move(Speed * deltaTime * Down);
            if(Input::isKey('A')) p_onControl->Move(Speed * deltaTime * Left);
            if(Input::isKey('D')) p_onControl->Move(Speed * deltaTime * Right);
            if(Input::isKeyPressed(GLFW_KEY_SPACE)) p_onControl->Jump();
        }
    }
    if(Input::isKeyRelased(GLFW_KEY_END))
        map->SaveMap(Settings::mapFile);

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
