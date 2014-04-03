#include "input.h"
#include "camera.h"
#include "editor.h"
#include "physics.h" // remove this later
#include "collider2d.h" // remove this later
#include "component.h"
#include "gameObject.h"

EditMode Editor::mode;
bool Editor::isMultyEdit;
vector<GameObject*> Editor::selection;

void Editor::Update()
{
    Transform* t = &Camera::MainCamera->transform;
    isMultyEdit = Input::isKey(GLFW_KEY_LEFT_CONTROL);

    if(Input::isMouse(0))
        MoveCam();

    if(Input::isKeyPressed(GLFW_KEY_E))
        UpdateSelection( GameObject::Find("player") );

    /// Why not GUI?
    if(Input::isKeyPressed(GLFW_KEY_1) )
        mode = EditMode::PositionEdit;
    if(Input::isKeyPressed(GLFW_KEY_2) )
        mode = EditMode::ScaleEdit;
    if(Input::isKeyPressed(GLFW_KEY_3) )
        mode = EditMode::RotationEdit;   /// Empty
    if(Input::isKeyPressed(GLFW_KEY_4) )
        mode = EditMode::ColorEdit;   /// Empty

    if(Camera::MainCamera->GetCameraType() == CameraType::Perspective && Input::mouseWDelta.y != 0) // No cam z move -> later y instead of z
    {
        vec3 lTarget = Camera::MainCamera->gLookTarget();
        t->aPosition( (lTarget.z < 0 ? t->forward : t->backward) * (Input::mouseWDelta.y * -.4f) );
    }
}

void Editor::Edit()
{
    vec2 val = Input::mouseDelta;
    float camZ = Camera::MainCamera->transform.gPosition().z;

    switch (mode)
    {
    case EditMode::PositionEdit:
        if(camZ < 0)
            camZ = (-camZ);
        val *= (camZ * .002f);
        aaPosition(vec3(val.x,-val.y,0));
        break;
    case EditMode::ScaleEdit:
        val *= .01f;
        if( Input::isKey(GLFW_KEY_LEFT_SHIFT) )
        {
            float fac = (val.x + val.y) / 2;
            aaScale( vec3(fac,fac,0) );
        }
        else
            aaScale( vec3(val.x , val.y , 0) );
        break;
    case EditMode::ColorEdit:
    case EditMode::RotationEdit:
    default:
        break;
    }
}

void Editor::PutObject() /// Crate new gameObject(cube)
{
    if(!isMultyEdit)
        ClearSelection();

    GameObject* edit = new GameObject("cube");
    edit->isActive = false;
    vec3 nPos = vec3 ( Camera::MainCamera->transform.gPosition() );
    nPos.z = 0;
    nPos.x /= 15;
    nPos.y /= 15;
    edit->transform.uPosition(nPos);
    UpdateSelection(edit);
}

void Editor::DeleteObject() /// Delete selected objects
{
    for(GameObject* gmo : selection)
        delete gmo;

    selection.clear();
}

void Editor::SelectObjects()
{
    vec3 gPos = vec3 ( Camera::MainCamera->transform.gPosition());

    if(!isMultyEdit)
        ClearSelection();

    UpdateSelections( Collider2d::GetAll(gPos) );
}

void Editor::UpdateSelections(const vector<GameObject*> val) /// Unite new vector with selected vector
{
     for(GameObject* gmo : val)
        UpdateSelection( gmo );
}

void Editor::UpdateSelection(GameObject* obj) /// Add object to selected vector
{
    if(obj == nullptr) return;

    if(!isSelected(obj))
    {
        obj->isActive = false;
        selection.push_back(obj);
    }
    else
    {
        obj->isActive = true;
        RemoveSelection(obj);
    }
}

void Editor::RemoveSelection(GameObject* obj) /// Deselect
{
    for(unsigned int i = 0;i < selection.size();i++)
        if(selection[i] == obj)
        {
            obj->isActive = true;
            selection.erase(selection.begin() + i);
        }
}

void Editor::ClearSelection() /// Reset Selection
{
    for(GameObject* gmo : selection)
        gmo->isActive = true;

    selection.clear();
}

void Editor::aaPosition(vec3 val) /// Add to selections
{
    for(GameObject* gmo : selection)
    {
        if( Input::isKey(GLFW_KEY_LEFT_SHIFT) )
        {
            Physics* p = gmo->GetComponent<Physics>();
            if(p != nullptr)
                p->Move(val);
        }
        else
            gmo->transform.aPosition(val);
    }
}

void Editor::aaScale(vec3 val) /// Add to selections
{
    for(GameObject* gmo : selection)
        gmo->transform.aScale(val);
}

void Editor::MoveCam()
{
    Camera* c = Camera::MainCamera;
    Transform* t = &c->transform;
    float orthoSpeed = 1,perspSpeed = .001f * t->gPosition().z;
    float speed = c->GetCameraType() == CameraType::Orthographic ? orthoSpeed : perspSpeed ;
    vec2 val = Input::mouseDelta;
    t->aPosition((t->left * val.x * speed) + (t->up * val.y * speed));
}

bool Editor::isSelected(GameObject* val) /// Is object selected
{
    for(GameObject* gmo : selection)
        if(gmo == val)
            return true;

    return false;
}

GameObject* Editor::GetSelection(unsigned int index)
{
    return (selection.size()+1 >= index ? selection[index] : nullptr);
}
