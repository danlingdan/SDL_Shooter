#pragma once
#include "Framework.h"
#include "Scene.h"
#include "SceneMain.h"

class SceneTitle : public Scene
{
public:
    virtual void init() override;
    virtual void update(float deltaTime) override;
    virtual void render() override;
    virtual void clean() override;
    virtual void handleEvent(SDL_Event* event) override;

private:
    Mix_Music* bgm; // ±≥æ∞“Ù¿÷
    float timer = 0.0f; // º∆ ±∆˜
};