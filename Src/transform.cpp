#include "transform.h"
#include "tools.h"

void Transform::MakeMatrix()
{
    auto m4 = mat4(1.0);
    mMatrix = translate(m4,mPosition);
    mMatrix *= rotate(m4,mRotation.x , vec3(0,1,0));
    mMatrix *= rotate(m4,mRotation.y , vec3(1,0,0));
    mMatrix *= scale(m4,mScale);

    if(parent != nullptr)
        parent->GenerateMatrix();
}
/// ADD
void Transform::aScale(const vec3& val)
{
    mScale += val;
    MakeMatrix();
}
void Transform::aRotation(const vec3& val)
{
    mRotation += val;
    MakeMatrix();
}
void Transform::aPosition(const vec3& val)
{
    mPosition += val;
    MakeMatrix();
}
/// SET
void Transform::uScale(const vec3& val)
{
    mScale = val;
    MakeMatrix();
}
void Transform::uPosition(const vec3& val)
{
    mPosition = val;
    MakeMatrix();
}
void Transform::uRotation(const vec3& val)
{
    mRotation = val;
    MakeMatrix();
}
/// GET
mat4 Transform::gMatrix() const
{
    return mMatrix;
}
vec3 Transform::gScale() const
{
    return mScale;
}
vec3 Transform::gPosition() const
{
    return mPosition;
}
vec3 Transform::gRotation() const
{
    return mRotation;
}

void Transform::Log()
{
    stringstream strs;
    strs << "transform dump" << endl;

    Tools::Str::AddHashStreamVec3(strs,"Position ->",mPosition);
    Tools::Str::AddHashStreamVec3(strs,"Rotation ->",mRotation);
    Tools::Str::AddHashStreamVec3(strs,"Scale    ->",mScale);

    Tools::Logger::Info(strs.str());
}
