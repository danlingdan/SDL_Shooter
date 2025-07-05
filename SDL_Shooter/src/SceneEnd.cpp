#include "SceneEnd.h"
#include "Game.h"

void SceneEnd::init()
{
    // �����������
    bgm = Mix_LoadMUS("assets/music/06_Battle_in_Space_Intro.ogg");
    if (!bgm) {
        SDL_LogError(SDL_LOG_PRIORITY_ERROR, "Failed to load music: %s", Mix_GetError());
    }
    Mix_PlayMusic(bgm, -1);

    if (!SDL_IsTextInputActive()) {
        SDL_StartTextInput();
    }
    if (!SDL_IsTextInputActive()) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL InPutError:%s/d", SDL_GetError());
    }
}

void SceneEnd::update(float deltaTime)
{
    blinkTimer -= deltaTime;
    if (blinkTimer <= 0) {
        blinkTimer += 1.0f;
    }
}

void SceneEnd::render()
{
    if (isTyping) {
        renderPhase1();
    }
    else {
        renderPhase2();
    }
}

void SceneEnd::clean()
{
    if (SDL_IsTextInputActive()) {
        SDL_StopTextInput();
    }
    if (bgm != nullptr) {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
    }
}

void SceneEnd::handleEvent(SDL_Event* event)
{
    if (isTyping) {
        if (event->type == SDL_TEXTINPUT) {
            name += event->text.text;
        }
        if (event->type == SDL_KEYDOWN) {
            if (event->key.keysym.scancode == SDL_SCANCODE_RETURN) {
                isTyping = false; // �л�״̬
                SDL_StopTextInput();
                if (name == "") {
                    name = "������"; // ������û���������֣�ʹ��Ĭ������
                }
                game.insertLeaderBoard(game.getFinalScore(), name); // ����ҷ����������а�
            }
            if (event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
                removeLastUTF8Char(name);
            }
        }
    }
    else {
        if (event->type == SDL_KEYDOWN) {
            if (event->key.keysym.scancode == SDL_SCANCODE_J) {
                auto sceneMain = new SceneMain(); // �����µ�����Ϸ����
                game.changeScene(sceneMain); // �л�������Ϸ����
            }
        }
    }
}

void SceneEnd::removeLastUTF8Char(std::string& str)
{
    if (str.empty()) return;

    auto lastchar = str.back();
    if ((lastchar & 0b10000000) == 0b10000000) { // �����ַ��ĺ����ֽ�
        str.pop_back();
        while ((str.back() & 0b11000000) != 0b11000000) { // �ж��Ƿ��������ַ��ĵ�һ���ֽ�
            str.pop_back();
        }
    }
    str.pop_back();
}

void SceneEnd::renderPhase1()
{
    auto score = game.getFinalScore();
    std::string scoreText = "��ĵ÷��ǣ�" + std::to_string(score);
    std::string gameOver = "Game Over";
    std::string instrutionText = "������������֣����س���ȷ�ϣ�";
    game.renderTextCentered(scoreText, 0.1, false);
    game.renderTextCentered(gameOver, 0.4, true);
    game.renderTextCentered(instrutionText, 0.6, false);

    if (name != "") {
        SDL_Point p = game.renderTextCentered(name, 0.8, false);
        if (blinkTimer < 0.5) {
            game.renderTextPos("_", p.x, p.y);
        }
    }
    else {
        if (blinkTimer < 0.5) {
            game.renderTextCentered("_", 0.8, false);
        }
    }
}

void SceneEnd::renderPhase2()
{
    game.renderTextCentered("�÷ְ�", 0.05, true);
    auto posY = 0.2 * game.getWindowHeight();
    auto i = 1;
    for (auto item : game.getLeaderBoard()) {
        std::string name = std::to_string(i) + ". " + item.second;
        std::string score = std::to_string(item.first);
        game.renderTextPos(name, 100, posY);
        game.renderTextPos(score, 100, posY, false);
        posY += 45;
        i++;
    }
    if (blinkTimer < 0.5) {
        game.renderTextCentered("�� J �����¿�ʼ��Ϸ", 0.85, false);
    }
}

