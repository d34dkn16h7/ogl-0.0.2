#include "transform.h"

void Transform::MakeMatrix()
{
    mMatrix = translate(mat4(1.0),mPosition);
    mMatrix *= rotate(mat4(1.0),mRotation.x , vec3(0,1,0));
    mMatrix *= rotate(mat4(1.0),mRotation.y , vec3(1,0,0));
    mMatrix *= scale(mat4(1.0),mScale);

    if(parent != nullptr)
        parent->GenerateMatrix();
}
