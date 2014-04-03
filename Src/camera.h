#ifndef CAMERA_H
#define CAMERA_H

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "transform.h"

using namespace std;
using namespace glm;

enum CameraType
{
    Orthographic,Perspective
};

class Camera : public Transformable
{
private:
    CameraType mCamType;
    int fow;
    float aspectRatio;

    vec3 lookTarget = vec3(0,0,-.1f);

    mat4 look;
    mat4 camera;
    mat4 projection;
public:
    vec2 ScreenSize;
    static Camera* MainCamera;
    Camera(int,int);

    void GenerateMatrix();
    void SetCameraType(CameraType);

    void uLookTarget(vec3);
    void aLookTarget(vec3);

    vec3 gLookTarget();

    CameraType GetCameraType();
    mat4 Matrix();
    mat4 GetLook(vec3);
    mat4 GetProjection();
    float GetAspectRatio();
    const mat4& GetMatrix();
};

#endif // CAMERA_H
