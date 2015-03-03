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
    vec3 mScale    = vec3(1,1,1);

    mat4 mMatrix;
    void MakeMatrix();
public:
    const vec3
        forward  = vec3(0,0,1),
        right    = vec3(1,0,0),
        up       = vec3(0,1,0),
        backward = -forward,
        left     = -right,
        down     = -up;

    Transform()
        {MakeMatrix();}
    Transform(Transformable* t) : parent(t)
        {MakeMatrix();}

    /// Add
    void aScale(const vec3&);
    void aPosition(const vec3&);
    void aRotation(const vec3&);

    /// Set
    void uScale(const vec3&);
    void uPosition(const vec3&);
    void uRotation(const vec3&);

    /// Get
    mat4 gMatrix() const;
    vec3 gScale() const;
    vec3 gPosition() const;
    vec3 gRotation() const;
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
