#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

using namespace glm;

class Transformable;

class Transform
{
private:
    Transformable* parent = nullptr;

    vec3 mPosition = vec3(0,0,0);
    vec3 mRotation = vec3(0,0,0);
    vec3 mScale = vec3(1,1,1);

    mat4 mMatrix;
    void MakeMatrix();
public:
    vec3 forward = vec3(0,0,1),backward = -forward;
    vec3 left = vec3(-1,0,0),right = -left;
    vec3 up = vec3(0,1,0),down = -up;

    Transform()
        {MakeMatrix();}
    Transform(Transformable* t) : parent(t)
        {MakeMatrix();}

    /// Add
    void aPosition(const vec3& val)
        {mPosition += val;MakeMatrix();}
    void aPosition(const vec2& val)
        {aPosition(vec3(val.x,val.y,0));}

    void aRotation(const vec3& val)
        {mRotation += val;MakeMatrix();}
    void aScale(const vec3& val)
        {mScale += val;MakeMatrix();}

    /// Set
    void uPosition(const vec3& val)
        {mPosition = val;MakeMatrix();}
    void uRotation(const vec3& val)
        {mRotation = val;MakeMatrix();}
    void uScale(const vec3& val)
        {mScale = val;MakeMatrix();}

    /// Get
    vec3 gPosition() const
        {return mPosition;}
    vec3 gRotation() const
        {return mRotation;}
    vec3 gScale() const
        {return mScale;}
    mat4 gMatrix() const
        {return mMatrix;}
};

class Transformable
{
private:
public:
    Transform transform;

    Transformable() : transform(this) {}

    virtual void MoveUpdate() {}
    virtual void RotationUpdate() {}
    virtual void ScaleUpdate() {}
    virtual void GenerateMatrix() {}
};

#endif // TRANSFORM_H
