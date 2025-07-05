#include "SceneTitle.h"

void SceneTitle::init()
{
    //���벢���ű�������
    bgm = Mix_LoadMUS("assets/music/06_Battle_in_Space_Intro.ogg");
    if (bgm == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load music: %s", Mix_GetError());
    }
    Mix_PlayMusic(bgm, -1);
}

void SceneTitle::update(float deltaTime)
{
    timer += deltaTime;
    if (timer > 1.0f) {
        timer -= 1.0f;
    }
}

void SceneTitle::render()
{
    //��Ⱦ��������
    std::string titleText = "SDL̫��ս��";
    game.renderTextCentered(titleText, 0.4, true);

    //��Ⱦ��ͨ����
    if (timer < 0.5f) {
        std::string instructions = "�� J ����ʼ��Ϸ";
        game.renderTextCentered(instructions, 0.8, false);
    }
}

void SceneTitle::clean()
{
    // ����������
    if (bgm != nullptr) {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
    }
}

void SceneTitle::handleEvent(SDL_Event* event)
{
    if (event->type == SDL_KEYDOWN) {
        if (event->key.keysym.scancode == SDL_SCANCODE_J) {
            auto sceneMain = new SceneMain();
            game.changeScene(sceneMain);
        }
    }
}
