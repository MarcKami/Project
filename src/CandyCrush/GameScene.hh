/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#pragma once
#include "Grid.hh"
#include "GameObjects.hh"
#include "Scene.hh"
#include <vector>
#include <list>

// GameScene class with the main gameplay mode
class GameScene : public Scene {
public:
	explicit GameScene();
	~GameScene() override;
	void OnEntry(void) override;
	void OnExit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	Player m_player;
	Sprite m_background;
	std::list<Enemy> v_enemies;
	std::list<Bullet> p_bullets;
	std::list<Bullet> e_bullets;
	int m_score{ 0 };
};