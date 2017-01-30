/******************************************************************
* Copyright (C) 2016 Jordi Serrano Berbel <jsberbel95@gmail.com> *
* This can not be copied, modified and/or distributed without    *
* express permission of the copyright owner.                     *
******************************************************************/

#include "GUI.hh"
#include "System.hh"
#include "Logger.hh"
#include "GameScene.hh"
#include "MenuScene.hh"
#include "RankingScene.hh"

using namespace Logger;

#define CELL_WIDTH 80
#define CELL_HEIGHT 80


GameScene::GameScene(void) : m_player(1){
	m_background = { { 0, 0, W.GetWidth(), W.GetHeight() }, ObjectID::BG_01 };
}

GameScene::~GameScene(void){
}

void GameScene::OnEntry(void) {
	m_player.life = dif;
	m_player.s_player.transform = { int(W.GetWidth()*.5f),int(W.GetWidth()*.5f),50,50 };
	for (int i = 0; i < cEn; i++) {
		v_enemies.push_back(Enemy(ObjectID::ROCK_L, 1));
	}
}

void GameScene::OnExit(void) {
	v_enemies.erase(v_enemies.begin(), v_enemies.end());
	e_bullets.erase(e_bullets.begin(), e_bullets.end());
	p_bullets.erase(p_bullets.begin(), p_bullets.end());
}

void GameScene::Update(void) {
	if (IM.IsKeyDown<KEY_BUTTON_ESCAPE>()) SM.SetCurScene<MenuScene>(0,0,0,0);
	if (m_player.life > 0) { // Alive
		m_player.Update(); // Player Update
		m_player.Collision(v_enemies, e_bullets); // Detect collisions Player
		for (std::list<Enemy>::iterator it = v_enemies.begin(); it != v_enemies.end();) {
			if (it->kill) {
				it = v_enemies.erase(it);
			}
			else {
				++it;
			}
		}
		for (std::list<Bullet>::iterator it = p_bullets.begin(); it != p_bullets.end();) {
			if (it->kill) {
				it = p_bullets.erase(it);
			}
			else {
				++it;
			}
		}
		for (std::list<Bullet>::iterator it = e_bullets.begin(); it != e_bullets.end();) {
			if (it->kill) {
				it = e_bullets.erase(it);
			}
			else {
				++it;
			}
		}
		for (Enemy &it : v_enemies) { // Enemy Updates
			it.Update();
			it.ChangeSpeed(m_score);
			it.prevMod = it.mod;
			it.Collision(p_bullets); // Detect collisions Enemy
			if (it.kill) {
				switch (it.id)
				{
				case ObjectID::ROCK_L:
					v_enemies.push_back(Enemy(ObjectID::ROCK_M1, it.v, it.s_enemy.transform, 50));
					v_enemies.push_back(Enemy(ObjectID::ROCK_M2, it.v, it.s_enemy.transform, 50));
					m_score += it.score;
					break;
				case ObjectID::ROCK_M1:
					v_enemies.push_back(Enemy(ObjectID::ROCK_S1, it.v, it.s_enemy.transform, 25));
					v_enemies.push_back(Enemy(ObjectID::ROCK_S2, it.v, it.s_enemy.transform, 25));
					m_score += it.score;
					break;
				case ObjectID::ROCK_M2:
					v_enemies.push_back(Enemy(ObjectID::ROCK_S1, it.v, it.s_enemy.transform, 25));
					v_enemies.push_back(Enemy(ObjectID::ROCK_S2, it.v, it.s_enemy.transform, 25));
					m_score += it.score;
					break;
				case ObjectID::ROCK_S1:
					v_enemies.push_back(Enemy(ObjectID::ROCK_XS1, it.v, it.s_enemy.transform, 10));
					v_enemies.push_back(Enemy(ObjectID::ROCK_XS2, it.v, it.s_enemy.transform, 10));
					m_score += it.score;
					break;
				case ObjectID::ROCK_S2:
					v_enemies.push_back(Enemy(ObjectID::ROCK_XS1, it.v, it.s_enemy.transform, 10));
					v_enemies.push_back(Enemy(ObjectID::ROCK_XS2, it.v, it.s_enemy.transform, 10));
					m_score += it.score;
					break;
				case ObjectID::ROCK_XS1:
					m_score += it.score;
					break;
				case ObjectID::ROCK_XS2:
					m_score += it.score;
					break;
				case ObjectID::UFO_L:
					v_enemies.push_back(Enemy(ObjectID::UFO_S, it.v, it.s_enemy.transform, 15));
					v_enemies.push_back(Enemy(ObjectID::UFO_S, it.v, it.s_enemy.transform, 15));
					m_score += it.score;
					break;
				case ObjectID::UFO_S:
					m_score += it.score;
					break;
				default:
					break;
				}
				
			}
			// haacer un if como el del shoot de player para los disparos de los UFO, ke aaun se han de configurar
		}
		if (m_player.shooting) { // Player Shooting, pulling bullets
			p_bullets.push_back(Bullet(m_player.GetAim()));
			m_player.shooting = false;
		}
		for (auto & it : p_bullets) { // Player Bullets Updates
			it.Update();
		}
		for (auto & it : e_bullets) { // Enemy Bullets Updates
			it.Update();
		}
	}
	else { // Dead
		SM.SetCurScene<MenuScene>(0,0,0,0);
	}	
}


void GameScene::Draw(void) {
	m_background.Draw(); // Render background
	GUI::DrawTextSolid<FontID::CANDY>("Lifes ",
	{ int(W.GetWidth()*.07f), int(W.GetHeight()*.9f), 1, 1 },
	{ 255, 255, 255 });
	GUI::DrawTextSolid<FontID::CANDY>(std::to_string(m_player.life), //Player lifes
	{ int(W.GetWidth()*.14f), int(W.GetHeight()*.9f), 1, 1 },
	{ 255, 255, 255 });
	GUI::DrawTextSolid<FontID::CANDY>(std::to_string(m_score), //Score
	{ W.GetWidth() >> 1, int(W.GetHeight()*.1f), 1, 1 },
	{ 255, 255, 255 });
	m_player.s_player.DrawPlayer(); // Render Player
	for (auto & it : v_enemies) { // Render Enemies
		it.Draw();
	}
	for (auto & it : p_bullets) { // Render Player Bullets
		it.Draw();
	}
	for (auto & it : e_bullets) { // Render Enemy Bullets
		it.Draw();
	}
}


