#pragma once
#include "Framework.h"
#include "Scene.h"
#include "SceneMain.h"
#include "Object.h"
#include "SceneTitle.h"

class Game
{
public:
	static Game& getInstance() {
		static Game instance;
		return instance;
	}


	~Game();
	void run();
	void init();
	void clean();
	void changeScene(Scene* scene);

	void handleEvent(SDL_Event* event);
	void update(float deltaTime);
	void render();

	// getters
	SDL_Window* getWindow() { return window; }
	SDL_Renderer* getRender() { return renderer; }
	int getWindowWidth() { return windowWidth; }
	int getWindowHeight() { return windowHeight; }
	int getFinalScore() { return finalScore; }

	// setters
	void setFinalScore(int score) { finalScore = score; }

	void backgroundUpdate(float deltaTime);
	void renderBackground();

	// 渲染文字函数,返回文本结束位置的坐标点
	SDL_Point renderTextCentered(std::string text, float posY, bool isTitle);
	void renderTextPos(std::string text, int posX, int posY, bool isLeft);
	void renderTextPos(std::string text, int posX, int posY);

	void insertLeaderBoard(int score, std::string name); // 插入排行榜
	std::multimap<int, std::string, std::greater<int>>& getLeaderBoard() { return leaderBoard; } // 返回leaderBoard的引用

	void saveData();
	void loadData();
private:
	Game();
	bool isRunning = true;
	Scene* currentScene = nullptr;
	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;
	int windowWidth = 600;
	int windowHeight = 800;
	bool isFullscreen = false;
	// 最终得分
	int finalScore = 0;

	int FPS = 60; // 目标帧率
	Uint32 frameTime; // 每帧的目标时间（毫秒）
	float deltaTime;   // 两帧之间的时间差（秒）

	// 删除拷贝和赋值构造函数
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	Background nearStars;  // 近处的星星
	Background farStars;   // 远处的星星

	TTF_Font* titleFont;
	TTF_Font* textFont;

	std::multimap<int, std::string, std::greater<int>> leaderBoard; // 存储分数和名字，分数作为键，名字作为值
};
