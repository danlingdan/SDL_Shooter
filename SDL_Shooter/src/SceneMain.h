#pragma once

#include "Framework.h"
#include "SceneMain.h"
#include "SceneTitle.h"
#include "SceneEnd.h"
#include "Game.h"


class SceneMain : public Scene
{
public:
	SceneMain();
	~SceneMain();

	void update(float deltaTime) override;
	void render() override;
	void handleEvent(SDL_Event* event) override;
	void init() override;
	void clean() override;

	void keyboardControl(float deltaTime);

	void shootPlayer();
	void updatePlayerProjectile(float deltaTime);
	void renderPlayerProjectiles();

	void spawEnemy();
	void updateEnemies(float deltaTime);
	void rendererEnemies();

	void renderEnemyProjectiles();
	void updateEnemyProjectiles(float deltaTime);
	void shootEnemy(Enemy* enemy);
	SDL_FPoint getDirection(Enemy* enemy);

	void updatePlayer(float deltaTime);
	void enemyExplode(Enemy* enemy);

	void updateExplosions(float deltaTime);
	void renderExplosions();

	void dropItem(Enemy* enemy);
	void updateItem(float deltaTime);
	void playerGetItem(Item* item);
	void renderItem();

	void renderUI();

	void changeSceneDelayed(float deltaTime, float delay);
private:
	Player player;

	bool isDead = false;

	// 随机数成员
	std::mt19937 gen; // 随机数生成器
	std::uniform_real_distribution<float> dis; // 随机数分布器

	// 创建子弹模板
	ProjectilePlayer projectilePlayerTemplate;
	// 存储玩家子弹的列表
	std::list<ProjectilePlayer*> projectilesPlayer;

	// 创建敌机模板
	Enemy enemyTemplate;
	// 存储活动敌机的列表
	std::list<Enemy*> enemies;

	// 创建敌机子弹模板
	ProjectileEnemy projectileEnemyTemplate;
	// 存储活动敌机子弹的列表
	std::list<ProjectileEnemy*> projectilesEnemy;

	// 创建爆炸动画模板
	Explosion explosionTemplate;
	// 存储活动爆炸动画的队列
	std::list<Explosion*> explosions;

	// 创建掉落物模板
	Item itemLifeTemplate;
	// 存储活动掉落物的队列
	std::list<Item*> items;

	// bgm和音效
	Mix_Music* bgm;
	std::map<std::string, Mix_Chunk*> sounds; // 存储音效

	// UI
	SDL_Texture* uiHealth;

	// 得分记录
	TTF_Font* scoreFont;
	int score = 0;

	// 计时器成员
	float timerEnd = 0.0f;
};
