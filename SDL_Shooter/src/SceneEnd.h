#pragma once
#include "Scene.h"


class SceneEnd : public Scene {
     void init();
     void update(float deltaTime);
     void render();
     void clean();
     void handleEvent(SDL_Event* event);

     void removeLastUTF8Char(std::string& str);
private:
    bool isTyping = true;
    std::string name = "";
    float blinkTimer = 1.0f;  // 光标闪烁计时器

    Mix_Music* bgm;

    void renderPhase1();
    void renderPhase2();
};