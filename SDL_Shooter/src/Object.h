#pragma once
#include "Framework.h"

struct Player
{
	SDL_Texture* texture = nullptr;
	SDL_FPoint position = { 0,0 };
	int width = 0;
	int height = 0;
	int speed = 300; // 每秒移动300像素
	int currentHealth = 5;
	int maxHealth = 5;
	Uint32 coolDown = 300; // 射击冷却时间300毫秒
	Uint32 lastShootTime = 0; // 上次射击时间

};

struct ProjectilePlayer
{
	SDL_Texture* texture = nullptr;
	SDL_FPoint position = { 0,0 };
	int width = 0;
	int height = 0;
	int speed = 600; // 每秒移动600像素
	int damage = 1;
};

struct Enemy
{
	SDL_Texture* texture = nullptr;
	SDL_FPoint position = { 0,0 };
	int width = 0;
	int height = 0;
	int speed = 150; // 敌机移动速度
	int currentHealth = 2;
	Uint32 coolDown = 2000; // 2秒（2000毫秒）
	Uint32 lastShootTime = 0; // 上次发射时间
};

struct ProjectileEnemy
{
	SDL_Texture* texture = nullptr;
	SDL_FPoint position = { 0,0 };
	SDL_FPoint direction = { 0,0 }; // 子弹方向向量
	int width = 0;
	int height = 0;
	int speed = 400;
	int damage = 1;
};

struct Explosion
{
	SDL_Texture* texture = nullptr;
	SDL_FPoint position = { 0,0 };
	int width = 0;
	int height = 0;
	int currentFrame = 0; // 当前帧索引
	int totalFrame = 0; // 总帧数
	Uint32 startTime = 0; // 动画开始时间
	Uint32 FPS = 10; // 动画帧率
};

enum class ItemType
{
	Life,
	Shield,
	Time
};

struct Item
{
	SDL_Texture* texture = nullptr;
	SDL_FPoint position = { 0,0 };
	SDL_FPoint direction = { 0,0 }; 
	int width = 0;
	int height = 0;
	int speed = 200;
	int bounceCount = 3;
	ItemType type = ItemType::Life;
};

struct Background {
	SDL_Texture* texture = nullptr;
	SDL_FPoint position = { 0, 0 };
	float offset = 0;
	int width = 0;
	int height = 0;
	int speed = 30;
};



























