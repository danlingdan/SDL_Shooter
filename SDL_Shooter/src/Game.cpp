#include "Game.h"



void Game::saveData()
{
	// 保存得分榜的数据
	std::ofstream file("assets/save.dat");
	if (!file.is_open()) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open save file");
		return;
	}
	for (const auto& entry : leaderBoard) {
		file << entry.first << " " << entry.second << std::endl;
	}
}

void Game::loadData()
{
	// 加载得分榜的数据
	std::ifstream file("assets/save.dat");
	if (!file.is_open()) {
		SDL_Log("Failed to open save file");
		return;
	}
	leaderBoard.clear();
	int score;
	std::string name;
	while (file >> score >> name) {
		leaderBoard.insert({ score, name });
	}
}

Game::Game() {

}

Game::~Game() {
	saveData();
	clean();
}

void Game::run() {
	while (isRunning)
	{
		auto frameStart = SDL_GetTicks(); // 记录帧开始时间

		SDL_Event event;
		handleEvent(&event);
		update(deltaTime);
		render();

		auto frameEnd = SDL_GetTicks(); // 记录帧结束时间
		auto diff = frameEnd - frameStart; // 计算帧处理时间

		// 帧率限制和deltaTime计算
		if (diff < frameTime) {
			SDL_Delay(frameTime - diff); // 如果处理太快，延迟一下
			deltaTime = frameTime / 1000.0f; // 转换为秒
		}
		else {
			deltaTime = diff / 1000.0f; // 如果处理较慢，使用实际时间
		}
	}
}

void Game::init() {

	frameTime = 1000 / FPS;

	// SDL主系统初始化
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL Init Error:%s/n", SDL_GetError());
		isRunning = false;
	}

	// SDL图像初始化
	if (IMG_Init(IMG_INIT_PNG) != (IMG_INIT_PNG)) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL Imag Init Error:%s/n", SDL_GetError());
		isRunning = false;
	}

	// SDL音频初始化
	if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) != (MIX_INIT_MP3 | MIX_INIT_OGG)) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL Mixer Init Error:%s/n", SDL_GetError());
		isRunning = false;
	}

	// 打开音频设备
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL Audio Init Error:%s/n", SDL_GetError());
		isRunning = false;
	}

	// 设置音效channel数量
	Mix_AllocateChannels(32);

	// 设置音量
	Mix_VolumeMusic(MIX_MAX_VOLUME / 6);
	Mix_Volume(-1, MIX_MAX_VOLUME / 8);

	// SDL字体初始化
	if (TTF_Init() != 0) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL TTF Init Error:%s/n", SDL_GetError());
		isRunning = false;
	}

	// 创建窗口
	window = SDL_CreateWindow("SDL Shoot Game By 华", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL Window Init Error:%s/n", SDL_GetError());
		isRunning = false;
	}

	// 创建渲染器
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL Render Init Error:%s/n", SDL_GetError());
		isRunning = false;
	}
	// 设置逻辑分辨率
	SDL_RenderSetLogicalSize(renderer, windowWidth, windowHeight);

	// 初始化背景卷轴
	nearStars.texture = IMG_LoadTexture(renderer, "assets/image/Stars-A.png");
	SDL_QueryTexture(nearStars.texture, NULL, NULL, &nearStars.width, &nearStars.height);
	nearStars.height /= 2;
	nearStars.width /= 2;

	farStars.texture = IMG_LoadTexture(renderer, "assets/image/Stars-B.png");
	SDL_QueryTexture(farStars.texture, NULL, NULL, &farStars.width, &farStars.height);
	farStars.height /= 2;
	farStars.width /= 2;

	// 载入字体
	titleFont = TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 64);
	textFont = TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 32);
	if (titleFont == nullptr || textFont == nullptr) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF_OpenFont: %s\n", TTF_GetError());
		isRunning = false;
	}

	// 载入得分
	loadData();

	currentScene = new SceneTitle();
	currentScene->init();

}

void Game::clean() {

	if (currentScene != nullptr) {
		currentScene->clean();
		delete currentScene;
	}
	if (nearStars.texture != nullptr) {
		SDL_DestroyTexture(nearStars.texture);
	}
	if (farStars.texture != nullptr) {
		SDL_DestroyTexture(farStars.texture);
	}

	// 清理音频
	Mix_CloseAudio();
	Mix_Quit();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

}

void Game::changeScene(Scene* scene) {
	if (currentScene != nullptr)
	{
		currentScene->clean();
		delete currentScene;
	}
	currentScene = scene;
	currentScene->init();
}

SDL_Point Game::renderTextCentered(std::string text, float posY, bool isTitle)
{
	SDL_Color color = { 255, 255, 255, 255 };
	SDL_Surface* surface;
	if (isTitle) {
		surface = TTF_RenderUTF8_Solid(titleFont, text.c_str(), color);
	}
	else {
		surface = TTF_RenderUTF8_Solid(textFont, text.c_str(), color);
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	int y = static_cast<int>((getWindowHeight() - surface->h) * posY);
	SDL_Rect rect = { getWindowWidth() / 2 - surface->w / 2,
					 y,
					 surface->w,
					 surface->h };
	SDL_RenderCopy(renderer, texture, NULL, &rect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	return { rect.x + rect.w, y };  // 返回文本末尾的坐标
}

void Game::renderTextPos(std::string text, int posX, int posY)
{
	SDL_Color color = { 255, 255, 255, 255 };
	SDL_Surface* surface = TTF_RenderUTF8_Solid(textFont, text.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_Rect rect = { posX, posY, surface->w, surface->h };
	SDL_RenderCopy(renderer, texture, NULL, &rect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

void Game::renderTextPos(std::string text, int posX, int posY, bool isLeft)
{
	SDL_Color color = { 255, 255, 255, 255 };
	SDL_Surface* surface = TTF_RenderUTF8_Solid(textFont, text.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_Rect rect;
	if (isLeft) {
		rect = { posX, posY, surface->w, surface->h };
	}
	else {
		rect = { getWindowWidth() - posX - surface->w, posY, surface->w, surface->h };
	}
	SDL_RenderCopy(renderer, texture, NULL, &rect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

void Game::insertLeaderBoard(int score, std::string name)
{
	leaderBoard.insert({ score, name });
	if (leaderBoard.size() > 8) {
		leaderBoard.erase(--leaderBoard.end());
	}
}

void Game::handleEvent(SDL_Event* event) {
	while (SDL_PollEvent(event))
	{
		if (event->type == SDL_QUIT)
		{
			isRunning = false;
		}
		if (event->type == SDL_KEYDOWN) {
			if (event->key.keysym.scancode == SDL_SCANCODE_F4) {
				isFullscreen = !isFullscreen;
				if (isFullscreen) {
					SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
				}
				else {
					SDL_SetWindowFullscreen(window, 0);
				}
			}
		}
		currentScene->handleEvent(event);
	}
}

void Game::update(float deltaTime) {
	backgroundUpdate(deltaTime);
	currentScene->update(deltaTime);
}

void Game::render() {
	// 清空
	SDL_RenderClear(renderer);
	// 渲染星空背景
	renderBackground();

	currentScene->render();
	// 显示更新
	SDL_RenderPresent(renderer);

}

void Game::backgroundUpdate(float deltaTime)
{
	nearStars.offset += nearStars.speed * deltaTime;
	if (nearStars.offset >= 0)
	{
		nearStars.offset -= nearStars.height;
	}

	farStars.offset += farStars.speed * deltaTime;
	if (farStars.offset >= 0) {
		farStars.offset -= farStars.height;
	}
}

void Game::renderBackground()
{
	// 渲染远处的星星
	for (int posY = static_cast<int>(farStars.offset); posY < getWindowHeight(); posY += farStars.height) {
		for (int posX = 0; posX < getWindowWidth(); posX += farStars.width) {
			SDL_Rect ds = { posX, posY, farStars.width, farStars.height };
			SDL_RenderCopy(renderer, farStars.texture, NULL, &ds);
		}
	}
	// 渲染近处的星星
	for (int posY = static_cast<int>(nearStars.offset); posY < getWindowHeight(); posY += nearStars.height)
	{
		for (int posX = 0; posX < getWindowWidth(); posX += nearStars.width)
		{
			SDL_Rect dstRect = { posX, posY, nearStars.width, nearStars.height };
			SDL_RenderCopy(renderer, nearStars.texture, nullptr, &dstRect);
		}
	}
}
















