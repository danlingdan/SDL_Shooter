#include "Framework.h"
#include "Game.h"
#include "Scene.h"
#include "SceneMain.h"

int main(int argc, char* argv[]) {

#ifdef _WIN32
	// 隐藏控制台窗口
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
#endif

	Game& game = Game::getInstance();
	game.init();
	game.run();
	return 0;
}