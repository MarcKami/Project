#pragma once
#include "Sprite.hh"
#include "InputManager.hh"

struct Aim
{
	int aimX, aimY, xPos, yPos;
};


class Bullet
{
public:
	int v = 50, r = 2; 
	double dirX, dirY;
	bool kill = false;
	Aim m_aim;
	Sprite s_bullet;

	Bullet(Aim);

	void Update();

	void Draw();

	void checkLimits();
};


class Enemy //Se llama enemy porque Rock y Ufo comparten todos los atributos, solo que ufo ataca (revisar si tmbien podria servir para player)
{
public:
	ObjectID id;
	int v, r, score;
	double dir[2];
	float mod = 0.f, prevMod = 0.6f;
	Sprite s_enemy;
	bool kill = false;

	Enemy(ObjectID id, int vEnemigo);

	Enemy(ObjectID id, int vEnemigo, Transform e_trans, int size);
	
	void Movement();

	void Attack();

	void checkLimits();

	void Update();

	void Draw();

	void ChangeSpeed(int);

	void Collision(std::list<Bullet>);
};

class Player
{
public:
	Player(int k);
	int life, r = 25;
	double v = 0.f, xDir, yDir;
	Aim p_aim;
	Sprite s_player;
	float delay = 0.2f, counter = 0.f, inmune = 1.f, i_count = 0.f;
	bool shooting = false, damaged = false;
	double pi = 3.14159265358979323846;
	void Update(); 
	Aim GetAim();
	bool Shoot();
	void Collision(std::list<Enemy>, std::list<Bullet>);

private:
	void HandleInput();
	void checkLimits();
	void Movement();
};








/*
void GenEnemy(int c, int v) {
for (int i = 0; i < c; i++) {
v_enemies.push_back(Enemy(ObjectID::ROCK_L, v));
}
}
*/
