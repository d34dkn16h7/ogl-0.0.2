#include "camera.h"
#include "game.h"

Camera* Camera::MainCamera;

Camera::Camera(int x,int y) :
    mCamType(CameraType::Perspective),fow(45), aspectRatio( (float)x / (float)y ), ScreenSize( vec2(x,y) )
{
    MainCamera = this;
    transform.aPosition(vec3(0,0,15));

    GenerateMatrix();
}

void Camera::GenerateMatrix()
{
    look = GetLook(lookTarget);
    projection = GetProjection();
    camera = projection * look * transform.gMatrix();
}

void Camera::SetCameraType(CameraType val)
{
    mCamType = val;
    GenerateMatrix();
}
//SET
void Camera::uLookTarget(vec3 val)
{
    lookTarget = val;
    GenerateMatrix();
}

void Camera::aLookTarget(vec3 val)
{
    lookTarget += val;
    GenerateMatrix();
}
//GET
CameraType Camera::GetCameraType()
{
    return mCamType;
}

vec3 Camera::gLookTarget()
{
    return lookTarget;
}

const mat4& Camera::GetMatrix()
{
    return (camera);
}

mat4 Camera::Matrix()
{
    return camera;
}

mat4 Camera::GetLook(vec3 mPos)
{
    return lookAt(transform.gPosition() * 2.0f,transform.gPosition() + mPos, vec3(0,1,0));
}

mat4 Camera::GetProjection()
{
    return  mCamType == CameraType::Orthographic ?
            ortho<float>( -ScreenSize.x / 2, ScreenSize.x / 2, -ScreenSize.y / 2, ScreenSize.y / 2,-500, 500) :
            perspective<float>(fow, aspectRatio, 0.1, 1000.0);
}

float Camera::GetAspectRatio()
{
    return aspectRatio;
}
