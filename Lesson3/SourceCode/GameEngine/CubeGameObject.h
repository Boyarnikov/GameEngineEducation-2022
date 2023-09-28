#pragma once

#include "GameObject.h"
#include "InputHandler.h"

#include <cmath>

class CubeGameObject : public GameObject
{
public:
    CubeGameObject(float st);

    virtual void Update(float t, float dt);

    virtual void Event(float t, float dt, EInputCommand e);

protected:
    float start_t = 0.f;
};


class JumpCube final : public CubeGameObject {
public:
    JumpCube(float st);
    void Update(float t, float dt) override;
private:
    float start_pos[3] = { std::nanf(""), std::nanf(""), std::nanf("") };
};


class ControlCube final : public CubeGameObject {
public: 
    ControlCube(float st);
    virtual void Event(float t, float dt, EInputCommand e) override;
};


class MoveCube final : public CubeGameObject {
public: 
    MoveCube(float st);
    void Update(float t, float dt) override;
private: 
    float start_pos[3] = { std::nanf(""), std::nanf(""), std::nanf("") };
};