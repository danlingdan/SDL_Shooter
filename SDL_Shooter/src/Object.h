#pragma once
#include "Framework.h"

struct Player
{
	SDL_Texture* texture = nullptr;
	SDL_FPoint position = { 0,0 };
	int width = 0;
	int height = 0;
	int speed = 300; // ÿ���ƶ�300����
	int currentHealth = 5;
	int maxHealth = 5;
	Uint32 coolDown = 300; // �����ȴʱ��300����
	Uint32 lastShootTime = 0; // �ϴ����ʱ��

};

struct ProjectilePlayer
{
	SDL_Texture* texture = nullptr;
	SDL_FPoint position = { 0,0 };
	int width = 0;
	int height = 0;
	int speed = 600; // ÿ���ƶ�600����
	int damage = 1;
};

struct Enemy
{
	SDL_Texture* texture = nullptr;
	SDL_FPoint position = { 0,0 };
	int width = 0;
	int height = 0;
	int speed = 150; // �л��ƶ��ٶ�
	int currentHealth = 2;
	Uint32 coolDown = 2000; // 2�루2000���룩
	Uint32 lastShootTime = 0; // �ϴη���ʱ��
};

struct ProjectileEnemy
{
	SDL_Texture* texture = nullptr;
	SDL_FPoint position = { 0,0 };
	SDL_FPoint direction = { 0,0 }; // �ӵ���������
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
	int currentFrame = 0; // ��ǰ֡����
	int totalFrame = 0; // ��֡��
	Uint32 startTime = 0; // ������ʼʱ��
	Uint32 FPS = 10; // ����֡��
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



























