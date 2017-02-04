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


GameScene::GameScene(void) : m_player(1){
	m_background = { { 0, 0, W.GetWidth(), W.GetHeight() }, ObjectID::BG_01 };
}

GameScene::~GameScene(void){
}

void GameScene::OnEntry(void) {
	name[0] = 'A'; name[1] = 'A'; name[2] = 'A';
	p_data.m_score = 0;
	stage = 0; 
	namePos = 0;
	m_player.life = data.life;
	m_player.s_player.transform = { int(W.GetWidth()*.5f),int(W.GetWidth()*.5f),50,50 };
	for (int i = 0; i < data.cEn; i++) 	v_enemies.push_back(Enemy(ObjectID::ROCK_L, data.vEn));
}

void GameScene::OnExit(void) {
	v_enemies.erase(v_enemies.begin(), v_enemies.end());
	e_bullets.erase(e_bullets.begin(), e_bullets.end());
	p_bullets.erase(p_bullets.begin(), p_bullets.end());
}

void GameScene::Update(void) {
	if (v_enemies.empty()) { // Enemy increment on every stage
		stage++;
		for (int i = 0; i < int(data.cEn + (stage * data.iEn)); i++) v_enemies.push_back(Enemy(ObjectID::ROCK_L, data.vEn));
	}
	if (IM.IsKeyDown<KEY_BUTTON_ESCAPE>()) SM.SetCurScene<MenuScene>(menu, p_default);
	if (m_player.life > 0) { // Alive
		m_player.Update(); // Player Update
		m_player.Collision(v_enemies, e_bullets); // Detect collisions Player
		for (std::list<Enemy>::iterator it = v_enemies.begin(); it != v_enemies.end();) { // Kill Enemies
			if (it->kill) it = v_enemies.erase(it);
			else ++it;
		}
		for (std::list<Bullet>::iterator it = p_bullets.begin(); it != p_bullets.end();) { // Kill Player Bullets
			if (it->kill) it = p_bullets.erase(it);
			else ++it;
		}
		for (std::list<Bullet>::iterator it = e_bullets.begin(); it != e_bullets.end();) { // Kill Enemy Bullets
			if (it->kill) it = e_bullets.erase(it);
			else ++it;
		}
		for (Enemy &it : v_enemies) { // Enemy Updates
			it.Update();
			it.ChangeSpeed(p_data.m_score);
			it.prevMod = it.mod;
			it.Collision(p_bullets); // Detect collisions Enemy
			if (it.kill) {
				switch (it.id)
				{
				case ObjectID::ROCK_L:
					v_enemies.push_back(Enemy(ObjectID::ROCK_M1, it.v, it.s_enemy.transform, 50));
					v_enemies.push_back(Enemy(ObjectID::ROCK_M2, it.v, it.s_enemy.transform, 50));
					p_data.m_score += it.score;
					break;
				case ObjectID::ROCK_M1:
					v_enemies.push_back(Enemy(ObjectID::ROCK_S1, it.v, it.s_enemy.transform, 25));
					v_enemies.push_back(Enemy(ObjectID::ROCK_S2, it.v, it.s_enemy.transform, 25));
					p_data.m_score += it.score;
					break;
				case ObjectID::ROCK_M2:
					v_enemies.push_back(Enemy(ObjectID::ROCK_S1, it.v, it.s_enemy.transform, 25));
					v_enemies.push_back(Enemy(ObjectID::ROCK_S2, it.v, it.s_enemy.transform, 25));
					p_data.m_score += it.score;
					break;
				case ObjectID::ROCK_S1:
					//v_enemies.push_back(Enemy(ObjectID::ROCK_XS1, it.v, it.s_enemy.transform, 10));
					//v_enemies.push_back(Enemy(ObjectID::ROCK_XS2, it.v, it.s_enemy.transform, 10));
					p_data.m_score += it.score;
					break;
				case ObjectID::ROCK_S2:
					//v_enemies.push_back(Enemy(ObjectID::ROCK_XS1, it.v, it.s_enemy.transform, 10));
					//v_enemies.push_back(Enemy(ObjectID::ROCK_XS2, it.v, it.s_enemy.transform, 10));
					p_data.m_score += it.score;
					break;
				case ObjectID::ROCK_XS1:
					p_data.m_score += it.score;
					break;
				case ObjectID::ROCK_XS2:
					p_data.m_score += it.score;
					break;
				case ObjectID::UFO_L:
					v_enemies.push_back(Enemy(ObjectID::UFO_S, it.v, it.s_enemy.transform, 15));
					v_enemies.push_back(Enemy(ObjectID::UFO_S, it.v, it.s_enemy.transform, 15));
					p_data.m_score += it.score;
					break;
				case ObjectID::UFO_S:
					p_data.m_score += it.score;
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
		for (auto & it : p_bullets) it.Update(); // Player Bullets Updates
		for (auto & it : e_bullets) it.Update(); // Enemy Bullets Updates
	}
	else { // Dead	Ranking input
		if (IM.IsKeyDown<KEY_BUTTON_ENTER>() && namePos < 2) namePos++;
		else if (IM.IsKeyDown<KEY_BUTTON_ENTER>() && namePos == 2) {
			p_data.name = name;
			SM.SetCurScene<RankingScene>(rank, p_data);
		}
		if (IM.GetKey() != "!" )name[namePos] = *IM.GetKey();
	}
}


void GameScene::Draw(void) {
	m_background.Draw(); // Render background
	if (m_player.life > 0) {
		GUI::DrawTextSolid<FontID::CANDY>("Lifes ",
		{ int(W.GetWidth()*.07f), int(W.GetHeight()*.9f), 1, 1 },
		{ 255, 255, 255 });
		GUI::DrawTextSolid<FontID::CANDY>(std::to_string(m_player.life), // Player lifes
		{ int(W.GetWidth()*.14f), int(W.GetHeight()*.9f), 1, 1 },
		{ 255, 255, 255 });
		GUI::DrawTextSolid<FontID::CANDY>(std::to_string(p_data.m_score), // Score
		{ W.GetWidth() >> 1, int(W.GetHeight()*.1f), 1, 1 },
		{ 255, 255, 255 });
		m_player.s_player.DrawPlayer(); // Render Player
		for (auto & it : v_enemies) it.Draw(); // Render Enemies
		for (auto & it : p_bullets) it.Draw(); // Render Player Bullets
		for (auto & it : e_bullets) it.Draw(); // Render Enemy Bullets
	}
	else {
		GUI::DrawTextSolid<FontID::TITTLE>("GAME OVER",
		{ W.GetWidth() >> 1, int(W.GetHeight()*.2f), 1, 1 },
		{ 115, 0, 180 });
		GUI::DrawTextBlended<FontID::CANDY>("NAME",
		{ int(W.GetWidth()*.2f), int(W.GetHeight()*.45f), 2, 2 },
		{ 255, 255, 255 });
		GUI::DrawTextBlended<FontID::CANDY>("SCORE",
		{ int(W.GetWidth()*.55f), int(W.GetHeight()*.45f), 2, 2 },
		{ 255, 255, 255 });
		GUI::DrawTextBlended<FontID::CANDY>(std::string(name),
		{ int(W.GetWidth()*.2f), int(W.GetHeight()*.55f), 3, 3 },
		{ 255, 255, 255 });
		GUI::DrawTextBlended<FontID::CANDY>(std::to_string(p_data.m_score),
		{ int(W.GetWidth()*.55f), int(W.GetHeight()*.55f), 3, 3 },
		{ 255, 255, 255 });
	}
}


