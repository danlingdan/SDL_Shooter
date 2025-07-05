#include "SceneMain.h"


SceneMain::SceneMain()
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::update(float deltaTime)
{
	keyboardControl(deltaTime);
	updatePlayerProjectile(deltaTime);
	updateEnemyProjectiles(deltaTime); // 更新敌机子弹
	spawEnemy(); // 生成敌机
	updateEnemies(deltaTime); // 更新敌机
	updatePlayer(deltaTime); // 更新玩家状态
	updateExplosions(deltaTime); // 更新爆炸动画
	updateItem(deltaTime); // 更新物品
	if (isDead) {
		changeSceneDelayed(deltaTime, 3); // 3秒后切换到结束场景
	}
}

void SceneMain::render()
{
	// 渲染玩家子弹
	renderPlayerProjectiles();
	// 渲染敌机子弹
	renderEnemyProjectiles();
	// 渲染玩家
	if (!isDead) {
		SDL_Rect playerRect = { static_cast<int>(player.position.x),
						static_cast<int>(player.position.y),
						player.width,
						player.height };
		SDL_RenderCopy(game.getRender(), player.texture, NULL, &playerRect);
	}


	// 渲染敌机
	rendererEnemies();
	// 渲染爆炸动画
	renderExplosions();
	// 渲染物品
	renderItem();
	// 渲染UI
	renderUI();
}

void SceneMain::handleEvent(SDL_Event* event)
{
}

void SceneMain::init()
{
	// 初始化随机数生成器
	std::random_device rd; // 获取真随机数作为种子
	gen = std::mt19937(rd()); // 用种子初始化美森旋转引擎
	dis = std::uniform_real_distribution<float>(0.0f, 1.0f); //[0,1]均匀分布

	// 初始化音乐并播放
	bgm = Mix_LoadMUS("assets/music/03_Racing_Through_Asteroids_Loop.ogg");
	if (bgm == nullptr) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "BGM Init Error:%s/n",SDL_GetError());
	}
	Mix_PlayMusic(bgm, -1);

	// 读取音效
	sounds["player_shoot"] = Mix_LoadWAV("assets/sound/laser_shoot4.wav");
	sounds["enemy_shoot"] = Mix_LoadWAV("assets/sound/xs_laser.wav");
	sounds["player_explode"] = Mix_LoadWAV("assets/sound/explosion1.wav");
	sounds["enemy_explode"] = Mix_LoadWAV("assets/sound/explosion3.wav");
	sounds["hit"] = Mix_LoadWAV("assets/sound/eff11.wav");
	sounds["get_item"] = Mix_LoadWAV("assets/sound/eff5.wav");

	// 读取ui Health
	uiHealth = IMG_LoadTexture(game.getRender(), "assets/image/Health UI Black.png");

	// 载入字体
	scoreFont = TTF_OpenFont("assets/font/VonwaonBitmap-12px.ttf", 24);

	// 初始化玩家飞机
	player.texture = IMG_LoadTexture(game.getRender(), "assets/image/SpaceShip.png");
	if (player.texture == nullptr) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Load Player Failed:%s/n", SDL_GetError());
	}

	SDL_QueryTexture(player.texture, nullptr, nullptr, &player.width, &player.height);
	player.width = player.width / 4;
	player.height = player.height / 4;
	player.position.x = game.getWindowWidth() / 2 - player.width / 2;
	player.position.y = game.getWindowHeight() - player.height;

	// 初始化玩家子弹模板
	projectilePlayerTemplate.texture = IMG_LoadTexture(game.getRender(), "assets/image/laser-1.png");
	SDL_QueryTexture(projectilePlayerTemplate.texture, nullptr, nullptr,
		&projectilePlayerTemplate.width,
		&projectilePlayerTemplate.height);
	projectilePlayerTemplate.width /= 4;
	projectilePlayerTemplate.height /= 4;

	// 初始化敌机模板
	enemyTemplate.texture = IMG_LoadTexture(game.getRender(), "assets/image/insect-2.png");
	SDL_QueryTexture(enemyTemplate.texture, nullptr, nullptr,
		&enemyTemplate.width,
		&enemyTemplate.height);
	enemyTemplate.width /= 4;
	enemyTemplate.height /= 4;

	// 初始化敌机子弹模板
	projectileEnemyTemplate.texture = IMG_LoadTexture(game.getRender(), "assets/image/bullet-1.png");
	SDL_QueryTexture(projectileEnemyTemplate.texture, nullptr, nullptr,
		&projectileEnemyTemplate.width,
		&projectileEnemyTemplate.height);
	projectileEnemyTemplate.width /= 4;
	projectileEnemyTemplate.height /= 4;

	// 初始化爆炸动画
	explosionTemplate.texture = IMG_LoadTexture(game.getRender(), "assets/effect/explosion.png");
	SDL_QueryTexture(explosionTemplate.texture, nullptr, nullptr, &explosionTemplate.width, &explosionTemplate.height);
	explosionTemplate.totalFrame = explosionTemplate.width / explosionTemplate.height;
	explosionTemplate.width = explosionTemplate.height;

	// 初始化物品模板
	itemLifeTemplate.texture = IMG_LoadTexture(game.getRender(), "assets/image/bonus_life.png");
	SDL_QueryTexture(itemLifeTemplate.texture, NULL, NULL, &itemLifeTemplate.width, &itemLifeTemplate.height);
	itemLifeTemplate.width /= 4;
	itemLifeTemplate.height /= 4;

}

void SceneMain::clean()
{
	// 清理子弹
	for (auto& projectile : projectilesPlayer) {
		if (projectile != nullptr) {
			delete projectile;
		}
	}
	projectilesPlayer.clear();

	// 清理敌机
	for (auto& enemy : enemies) {
		if (enemy != nullptr) {
			delete enemy;
		}
	}
	enemies.clear();

	// 清理敌机子弹
	for (auto& projectile : projectilesEnemy) {
		if (projectile != nullptr) {
			delete projectile;
		}
	}
	projectilesEnemy.clear();

	// 清理爆炸动画
	for (auto& explosion : explosions) {
		if (explosion != nullptr) {
			delete explosion;
		}
	}
	explosions.clear();

	// 清理物品
	for (auto& item : items) {
		if (item != nullptr) {
			delete item;
		}
	}
	items.clear();

	// 清理音效
	for (auto sound : sounds) {
		if (sound.second != nullptr) {
			Mix_FreeChunk(sound.second);
		}
	}
	sounds.clear();

	// 清理bgm
	if (bgm != nullptr) {
		Mix_HaltMusic();
		Mix_FreeMusic(bgm);
	}

	// 清理ui
	if (uiHealth != nullptr) {
		SDL_DestroyTexture(uiHealth);
	}

	// 清理字体
	if (scoreFont != nullptr) {
		TTF_CloseFont(scoreFont);
	}

	// 清理纹理
	if (player.texture != nullptr) {
		SDL_DestroyTexture(player.texture);
	}
	if (projectilePlayerTemplate.texture != nullptr) {
		SDL_DestroyTexture(projectilePlayerTemplate.texture);
	}
	if (enemyTemplate.texture != nullptr) {
		SDL_DestroyTexture(enemyTemplate.texture);
	}
	if (projectileEnemyTemplate.texture != nullptr) {
		SDL_DestroyTexture(projectileEnemyTemplate.texture);
	}
	if (explosionTemplate.texture != nullptr) {
		SDL_DestroyTexture(explosionTemplate.texture);
	}
	if (itemLifeTemplate.texture != nullptr) {
		SDL_DestroyTexture(itemLifeTemplate.texture);
	}
}

void SceneMain::keyboardControl(float deltaTime)
{
	// 飞机移动
	auto keyboardState = SDL_GetKeyboardState(NULL);
	if (keyboardState[SDL_SCANCODE_W]) {
		player.position.y -= deltaTime * player.speed; // 基于时间的移动
	}
	if (keyboardState[SDL_SCANCODE_S]) {
		player.position.y += deltaTime * player.speed; 
	}
	if (keyboardState[SDL_SCANCODE_A]) {
		player.position.x -= deltaTime * player.speed;
	}
	if (keyboardState[SDL_SCANCODE_D]) {
		player.position.x += deltaTime * player.speed;
	}

	// 限制飞机的移动范围
	if (player.position.x < 0) {
		player.position.x = 0;
	}
	if (player.position.x > game.getWindowWidth() - player.width) {
		player.position.x = game.getWindowWidth() - player.width;
	}
	if (player.position.y < 0) {
		player.position.y = 0;
	}
	if (player.position.y > game.getWindowHeight() - player.height) {
		player.position.y = game.getWindowHeight() - player.height;
	}

	// 控制子弹发射
	if (keyboardState[SDL_SCANCODE_J]) {
		auto currentTime = SDL_GetTicks();
		if (currentTime - player.lastShootTime >= player.coolDown) {
			shootPlayer();
			player.lastShootTime = currentTime;
		}
	}
}

void SceneMain::shootPlayer()
{
	// 创建新子弹
	auto projectile = new ProjectilePlayer(projectilePlayerTemplate);
	// 定位子弹在飞机顶部中央
	projectile->position.x = player.position.x + player.width / 2 - projectile->width / 2;
	projectile->position.y = player.position.y;
	// 添加到活动子弹列表
	projectilesPlayer.push_back(projectile);
	// 播放音效
	Mix_PlayChannel(0, sounds["player_shoot"], 0);
}

void SceneMain::updatePlayerProjectile(float deltaTime)
{
	int margin = 32; // 子弹超出屏幕到外边界的距离
	for (auto it = projectilesPlayer.begin(); it != projectilesPlayer.end();) {
		auto projectile = *it;
		// 更新子弹位置
		projectile->position.y -= projectile->speed * deltaTime;
		// 检查子弹是否超出屏幕
		if (projectile->position.y + margin <= 0) {
			delete projectile;
			it = projectilesPlayer.erase(it);
		}
		else {
			bool hit = false;
			for (auto enemy : enemies) {
				SDL_Rect enemyRect = {
					static_cast<int>(enemy->position.x),
					static_cast<int>(enemy->position.y),
					enemy->width,
					enemy->height
				};
				SDL_Rect projectileRect = {
					static_cast<int>(projectile->position.x),
					static_cast<int>(projectile->position.y),
					projectile->width,
					projectile->height
				};
				if (SDL_HasIntersection(&enemyRect, &projectileRect)) {
					enemy->currentHealth -= projectile->damage;
					delete projectile;
					it = projectilesPlayer.erase(it);
					hit = true;
					Mix_PlayChannel(-1, sounds["hit"], 0);
					break;
				}
			}
			if (hit == false) {
				++it;
			}
		}
	}
}

void SceneMain::renderPlayerProjectiles()
{
	for (auto projectile : projectilesPlayer) {
		SDL_Rect projectileRect = {
			static_cast<int>(projectile->position.x),
			static_cast<int>(projectile->position.y),
			projectile->width,
			projectile->height
		};
		SDL_RenderCopy(game.getRender(), projectile->texture, nullptr, &projectileRect);
	}
}

void SceneMain::spawEnemy()
{
	// 使用随机数生成敌人(约每秒1个=1/60帧每个)
	if (dis(gen) > 1 / 60.0f) {
		return;
	}
	else
	{
		// 创建新敌机
		Enemy* enemy = new Enemy(enemyTemplate);
		// 利用随机数决定敌机水平分布位置
		enemy->position.x = dis(gen) * (game.getWindowWidth() - enemy->width);
		// 垂直位置设置（屏幕上方不可见）
		enemy->position.y = -enemy->height;
		// 添加到活动敌机队列
		enemies.push_back(enemy);
	}

}

void SceneMain::updateEnemies(float deltaTime)
{
	auto currentTime = SDL_GetTicks();
	for (auto it = enemies.begin(); it != enemies.end();) {
		auto enemy = *it;
		// 更新敌机位置
		enemy->position.y += enemy->speed * deltaTime;

		// 判断敌机是否飞出屏幕底部
		if (enemy->position.y > game.getWindowHeight()) {
			delete enemy;
			it = enemies.erase(it);
		}
		else {
			// 检查冷却时间，决定是否发射敌机子弹
			if (currentTime - enemy->lastShootTime > enemy->coolDown) {
				shootEnemy(enemy);
				enemy->lastShootTime = currentTime;
			}
			if (enemy->currentHealth <= 0) {
				enemyExplode(enemy);
				it = enemies.erase(it);
			} else {
				++it;
			}
		}
	}
}

void SceneMain::rendererEnemies()
{
	for (auto enemy : enemies) {
		SDL_Rect enemyRect = {
			static_cast<int>(enemy->position.x),
			static_cast<int>(enemy->position.y),
			enemy->width,
			enemy->height
		};
		SDL_RenderCopy(game.getRender(), enemy->texture, nullptr, &enemyRect);
	}
}

void SceneMain::renderEnemyProjectiles()
{
	for (auto projectile : projectilesEnemy) {
		SDL_Rect projectileRect = {
			static_cast<int>(projectile->position.x),
			static_cast<int>(projectile->position.y),
			projectile->width,
			projectile->height
		};
	

	// 计算敌机子弹旋转角度(从弧到角度)
		float angle = atan2(projectile->position.y, projectile->position.x);

	// 渲染敌机子弹
		// 使用带旋转参数的渲染函数
		SDL_RenderCopyEx(game.getRender(), projectile->texture, NULL, &projectileRect, angle, NULL, SDL_FLIP_NONE);
	}
}

void SceneMain::updateEnemyProjectiles(float deltaTime)
{
	auto margin = 32; // 屏幕外额外容量
	for (auto it = projectilesEnemy.begin(); it != projectilesEnemy.end();) {
		auto projectile = *it;

		// 由方向向量计算敌机子弹位置
		projectile->position.x += projectile->speed * projectile->direction.x * deltaTime;
		projectile->position.y += projectile->speed * projectile->direction.y * deltaTime;

		// 检查敌机子弹是否飞出屏幕(四边都需要检查)
		if (projectile->position.y > game.getWindowHeight() + margin ||
			projectile->position.x > game.getWindowWidth() + margin ||
			projectile->position.y < -margin ||
			projectile->position.x < -margin) {
			delete projectile;
			it = projectilesEnemy.erase(it);
		}
		else {
			SDL_Rect projectileRect = {
				static_cast<int>(projectile->position.x),
				static_cast<int>(projectile->position.y),
				player.width,
				player.height
			};
			SDL_Rect playerRect = {
				static_cast<int>(player.position.x),
				static_cast<int>(player.position.y),
				player.width,
				player.height
			};
			if (SDL_HasIntersection(&projectileRect, &playerRect) && isDead == false) {
				player.currentHealth -= projectile->damage;
				delete projectile;
				it = projectilesEnemy.erase(it);
				Mix_PlayChannel(-1, sounds["hit"], 0);
			}
			else {
				++it;
			}
		}
	}
}

void SceneMain::shootEnemy(Enemy* enemy)
{
	// 创建新的敌机子弹
	auto projectile = new ProjectileEnemy(projectileEnemyTemplate);

	// 设置子弹初始位置（从敌机中央发射）
	projectile->position.x = enemy->position.x + enemy->width / 2 - projectile->width / 2;
	projectile->position.y = enemy->position.y + enemy->height / 2 - projectile->height / 2;

	// 计算敌机子弹飞向玩家方向
	projectile->direction = getDirection(enemy);

	// 添加到活动敌机子弹队列
	projectilesEnemy.push_back(projectile);

	// 播放音效
	Mix_PlayChannel(-1, sounds["enemy_shoot"], 0);
}

SDL_FPoint SceneMain::getDirection(Enemy* enemy)
{
	// 计算从敌机中心到玩家中心的向量
	auto x = (player.position.x + player.width / 2) - (enemy->position.x + enemy->width / 2);
	auto y = (player.position.y + player.height / 2) - (enemy->position.y + enemy->height / 2);

	// 计算向量长度
	auto length = sqrt(x * x + y * y);
	
	// 计算单位向量
	x /= length;
	y /= length;

	return SDL_FPoint{x,y};
}

void SceneMain::updatePlayer(float deltaTime)
{
	if (isDead == true) {
		return;
	}
	if (player.currentHealth <= 0) {
		// 游戏结束
		auto currentTime = SDL_GetTicks();
		isDead = true;
		auto explosion = new Explosion(explosionTemplate);
		explosion->position.x = player.position.x + player.width / 2 - explosion->width / 2;
		explosion->position.y = player.position.y + player.height / 2 - explosion->height / 2;
		explosion->startTime = currentTime;
		explosions.push_back(explosion);
		Mix_PlayChannel(-1, sounds["player_explode"], 0);
		game.setFinalScore(score);
		return;
	}
	for (auto enemy : enemies) {
		SDL_Rect enemyRect = {
			static_cast<int>(enemy->position.x),
			static_cast<int>(enemy->position.y),
			enemy->width,
			enemy->height
		};
		SDL_Rect playerRect = {
			static_cast<int>(player.position.x),
			static_cast<int>(player.position.y),
			player.width,
			player.height
		};
		if (SDL_HasIntersection(&enemyRect, &playerRect)) {
			player.currentHealth -= 1;
			enemy->currentHealth = 0;
		}
	}
}

void SceneMain::enemyExplode(Enemy* enemy)
{
	auto currentTime = SDL_GetTicks();
	auto explosion = new Explosion(explosionTemplate);
	explosion->position.x = enemy->position.x + enemy->width / 2 - explosion->width / 2;
	explosion->position.y = enemy->position.y + enemy->height / 2 - explosion->height / 2;
	explosion->startTime = currentTime;
	explosions.push_back(explosion);

	Mix_PlayChannel(-1, sounds["enemy_explode"], 0);
	score += 10;  // 击败敌机获得10分
	// 添加50%概率掉落物品
	if (dis(gen) < 0.5f) {
		dropItem(enemy);
	}

	delete enemy;
}

void SceneMain::updateExplosions(float deltaTime)
{
	auto currentTime = SDL_GetTicks();
	for (auto it = explosions.begin(); it != explosions.end();) {
		auto explosion = *it;
		// 根据时间差和帧率计算当前帧索引
		explosion->currentFrame = (currentTime - explosion->startTime) * explosion->FPS / 1000;
		// 检查动画是否播放完毕
		if (explosion->currentFrame >= explosion->totalFrame) {
			delete explosion;
			it = explosions.erase(it);
		} else {
			++it;
		}
	}
}

void SceneMain::renderExplosions()
{
	for (auto explosion : explosions) {
		// 定义矩形（图上区域）
		SDL_Rect src = { explosion->currentFrame * explosion->width, 0, explosion->width, explosion->height };
		// 定义目标矩形（屏幕上的位置）
		SDL_Rect dst = {
			static_cast<int>(explosion->position.x),
			static_cast<int>(explosion->position.y),
			explosion->width,
			explosion->height
		};
		// 绘制当前帧
		SDL_RenderCopy(game.getRender(), explosion->texture, &src, &dst);
	}
}

void SceneMain::dropItem(Enemy* enemy)
{
	auto item = new Item(itemLifeTemplate);
	item->position.x = enemy->position.x + enemy->width / 2 - item->width / 2;
	item->position.y = enemy->position.y + enemy->height / 2 - item->height / 2;

	// 随机方向
	float angle = dis(gen) * 2 * M_PI;
	item->direction.x = cos(angle);
	item->direction.y = sin(angle);

	items.push_back(item);
}

void SceneMain::updateItem(float deltaTime)
{
	for (auto it = items.begin(); it != items.end();) {
		auto item = *it;
		// 更新位置
		item->position.x += item->direction.x * item->speed * deltaTime;
		item->position.y += item->direction.y * item->speed * deltaTime;

		// 边缘反弹
		if (item->position.x < 0 && item->bounceCount>0) {
			item->direction.x = -item->direction.x;
			item->bounceCount--;
		}
		if (item->position.y < 0 && item->bounceCount>0) {
			item->direction.y = -item->direction.y;
			item->bounceCount--;
		}
		if (item->position.x+item->width >game.getWindowWidth() && item->bounceCount>0) {
			item->direction.x = -item->direction.x;
			item->bounceCount--;
		}
		if (item->position.y + item->height > game.getWindowHeight() && item->bounceCount > 0) {
			item->direction.y = -item->direction.y;
			item->bounceCount--;
		}
		// 如果超出屏幕范围则删除
		if (item->position.x + item->width < 0 ||
			item->position.x > game.getWindowWidth() ||
			item->position.y + item->height < 0 ||
			item->position.y > game.getWindowHeight()) {
			delete item;
			it = items.erase(it);
		}
		else {
			// 检测物品与玩家的碰撞
			SDL_Rect itemRect = {
				static_cast<int>(item->position.x),
				static_cast<int>(item->position.y),
				item->width,
				item->height
			};
			SDL_Rect playerRect = {
				static_cast<int>(player.position.x),
				static_cast<int>(player.position.y),
				player.width,
				player.height
			};

			if (SDL_HasIntersection(&itemRect, &playerRect))
			{
				playerGetItem(item);
				delete item;
				it = items.erase(it);
			}
			else {
				++it;
			}
		}
	}
}

void SceneMain::playerGetItem(Item* item)
{
	score += 5;  // 拾取物品获得5分
	if (item->type == ItemType::Life) {
		player.currentHealth += 1;
		if (player.currentHealth > player.maxHealth) {
			player.currentHealth = player.maxHealth;
		}
	}
	Mix_PlayChannel(-1, sounds["get_item"], 0);
}

void SceneMain::renderItem()
{
	for (auto& item : items) {
		SDL_Rect itemRect = {
			static_cast<int>(item->position.x),
			static_cast<int>(item->position.y),
			item->width,
			item->height
		};
		SDL_RenderCopy(game.getRender(), item->texture, nullptr, &itemRect);
	}
}

void SceneMain::renderUI()
{
	// 渲染血量
	int x = 10;
	int y = 10;
	int size = 32;
	int offset = 40;
	SDL_SetTextureColorMod(uiHealth, 100, 100, 100); // 颜色减淡
	for (int i = 0; i < player.maxHealth; i++)
	{
		SDL_Rect rect = { x + i * offset, y, size, size };
		SDL_RenderCopy(game.getRender(), uiHealth, NULL, &rect);
	}
	SDL_SetTextureColorMod(uiHealth, 255, 255, 255); // reset color
	for (int i = 0; i < player.currentHealth; i++)
	{
		SDL_Rect rect = { x + i * offset, y, size, size };
		SDL_RenderCopy(game.getRender(), uiHealth, NULL, &rect);
	}

	// 渲染得分
	auto text = "SCORE:" + std::to_string(score);
	SDL_Color color = { 255, 255, 255, 255 };  // 白色
	SDL_Surface* surface = TTF_RenderUTF8_Solid(scoreFont, text.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(game.getRender(), surface);
	SDL_Rect rect = { game.getWindowWidth() - 10 - surface->w, 10, surface->w, surface->h };
	SDL_RenderCopy(game.getRender(), texture, NULL, &rect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

void SceneMain::changeSceneDelayed(float deltaTime, float delay)
{
	timerEnd += deltaTime;
	if (timerEnd > delay) {
		auto sceneEnd = new SceneEnd();
		game.changeScene(sceneEnd);
	}
}

























