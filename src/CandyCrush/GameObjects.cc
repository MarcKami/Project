#include "TimeManager.hh"
#include "GameObjects.hh"
#include "Logger.hh"
#include <math.h>


Player::Player(int k) : life(k) {
	s_player.transform = { int(W.GetWidth() *.5f),int(W.GetHeight() *.5f), 50, 50 };
	s_player.objectID = ObjectID::PLAYER;
	s_player.angle = 0.f;
}


void Player::HandleInput() {

	if (IM.IsMouseHold<MOUSE_BUTTON_LEFT>()) {
		double angle = atan2(IM.GetMouseCoords().y - s_player.transform.y, IM.GetMouseCoords().x - s_player.transform.x);
		s_player.angle = 90+((angle * 180) / pi);
		if (counter >= delay) {
			shooting = true;
			counter = 0.f;
		}
		Logger::Println("Counter: ", counter, " || Delta: ", TM.GetDeltaTime(), " || Shooting: ", shooting);
	}

	//SI no funciona la rotacion, poner if en keyHold de a y d controlando los grados pasados de 360 o 0
	if (IM.IsKeyHold<KEY_BUTTON_ENTER>()) {
		if (counter >= delay) {
			shooting = true;
			counter = 0.f;
		}
	}

	if (IM.IsKeyHold<'a'>()) { 
		if (s_player.angle > 360) s_player.angle = 1;
		else if (s_player.angle < 0) s_player.angle = 359;
		s_player.angle -= 3.f;
	} 
	if (IM.IsKeyHold<'d'>()) { 
		if (s_player.angle > 360) s_player.angle = 1;
		else if (s_player.angle < 0) s_player.angle = 359;
		s_player.angle += 3.f;
	} 
	if (IM.IsKeyHold<KEY_BUTTON_SPACE>()) {
		xDir = sin(s_player.angle*pi / 180)*6;
		yDir = -cos(s_player.angle*pi / 180)*6;
		//Logger::Println("xDir: ", xDir, " || yDir: ", yDir, " || angle:", s_player.angle);
		if (v < 1.5){ v += .015f; }
	}
	else { if (v > 0) v -= .01f; }

	if (IM.IsKeyUp<KEY_BUTTON_LSHIFT>()) {
		s_player.transform = { rand() % int(W.GetWidth()),rand() % int(W.GetHeight()),50,50 };
		v = 0;
	}
}



void Player::checkLimits() {
	if (s_player.transform.x <= 0) s_player.transform.x = int(W.GetWidth()-1);
	if (s_player.transform.x >= int(W.GetWidth())) s_player.transform.x = 1;
	if (s_player.transform.y <= 0) s_player.transform.y = int(W.GetHeight()-1);
	if (s_player.transform.y >= int(W.GetHeight()))s_player.transform.y = 1;
}

void Player::Movement() {
	s_player.transform.x += xDir*v;
	s_player.transform.y += yDir*v;

	checkLimits();	
}

Aim Player::GetAim() {
	p_aim.aimX = (1000 * sin(s_player.angle*pi / 180)) + s_player.transform.x;
	p_aim.aimY = (1000 * -cos(s_player.angle*pi / 180)) + s_player.transform.y;
	//p_aim.aimX = IM.GetMouseCoords().x;
	//p_aim.aimY = IM.GetMouseCoords().y;
	//Logger::Println("aimX: ", p_aim.aimX, " || aimY", p_aim.aimY);
	p_aim.xPos = s_player.transform.x + (s_player.transform.rect.w / 2);
	p_aim.yPos = s_player.transform.y + (s_player.transform.rect.h / 2);
	return p_aim;
}

bool Player::Shoot() {

	return shooting = true;
}

void Player::Collision(std::list<Enemy> enemies, std::list<Bullet> e_bull) {
	if (!damaged) {
		for each (Enemy b in enemies)
		{
			int dx = s_player.transform.x - b.s_enemy.transform.x;
			int dy = s_player.transform.y - b.s_enemy.transform.y;
			int distance = sqrt(dx * dx + dy * dy);
			if (distance < r + b.r) { //Collision detected
				b.~Enemy();
				life--;
				damaged = true;
				s_player.transform = { int(W.GetWidth()*.5f),int(W.GetWidth()*.5f),50,50 };
				v = 0;
			}
		}
		for each (Bullet b in e_bull)
		{
			int dx = s_player.transform.x - b.s_bullet.transform.x;
			int dy = s_player.transform.y - b.s_bullet.transform.y;
			int distance = sqrt(dx * dx + dy * dy);
			if (distance < r + b.r) { //Collision detected
				b.~Bullet();
				life--;
				damaged = true;
				s_player.transform = { int(W.GetWidth()*.5f),int(W.GetWidth()*.5f),50,50 };
				v = 0;
			}
		}
	}
	else {
		i_count += TM.GetDeltaTime();
		if (i_count >= inmune) {
			damaged = false;
			i_count = 0.f;
		}
	}
};


void Player::Update() {
	counter += TM.GetDeltaTime();
	HandleInput();
	Movement();	
}


Enemy::Enemy(ObjectID id, int vEnemigo) : id(id), v(vEnemigo) //Para cuando se crean en el inicio
{
	s_enemy.objectID = id;
	s_enemy.transform = {200,200,70,70};
	r = 35; //setRadius(id);
	dir[0] = ((rand() % 3) - 1)*3;
	dir[1] = ((rand() % 3) - 1)*3;
	Logger::Println(dir[0], dir[1]);
	score = 20;
};

Enemy::Enemy(ObjectID id, int vEnemigo, Transform e_trans, int size) : id(id), v(vEnemigo) //Constructor por destrucción de la roca
{
	s_enemy.objectID = id;
	s_enemy.transform = { e_trans.x, e_trans.y, size, size };
	r = size/2; //setRadius(id);
	dir[0] = ((rand() % 3) - 1) * 3;
	dir[1] = ((rand() % 3) - 1) * 3;
	switch (id)
	{
	case ObjectID::ROCK_M1:
		score = 50;
		break;
	case ObjectID::ROCK_M2:
		score = 50;
		break;
	case ObjectID::ROCK_S1:
		score = 100;
		break;
	case ObjectID::ROCK_S2:
		score = 100;
		break;
	case ObjectID::ROCK_XS1:
		score = 150;
		break;
	case ObjectID::ROCK_XS2:
		score = 150;
		break;
	case ObjectID::UFO_L:
		score = 200;
		break;
	case ObjectID::UFO_S:
		score = 1000;
		break;
	default:
		break;
	}
};

void Enemy::Movement() {
	float i;
	switch (id)
	{
	case ObjectID::ROCK_L:
		i = 1.0f;
		break;
	case ObjectID::ROCK_M1:
		i = 1.25f;
		break;
	case ObjectID::ROCK_M2:
		i = 1.25f;
		break;
	case ObjectID::ROCK_S1:
		i = 1.5f;
		break;
	case ObjectID::ROCK_S2:
		i = 1.5f;
		break;
	case ObjectID::ROCK_XS1:
		i = 1.75f;
		break;
	case ObjectID::ROCK_XS2:
		i = 1.75f;
		break;
	case ObjectID::UFO_L:
		i = 1.5f;
		break;
	case ObjectID::UFO_S:
		i = 2.0f;
		break;
	default:
		break;
	}
	s_enemy.transform.x += dir[0] * v * i;
	s_enemy.transform.y += dir[1] * v * i;

	checkLimits();
};

void Enemy::Attack() {
	if (id == ObjectID::UFO_L) {
		//Disparo random por el mapa
	}
	else if (id == ObjectID::UFO_S) {
		// Dispara al jugador con una precisión que ugmenta segun dificultad
	};
};

void Enemy::checkLimits() {
	if (s_enemy.transform.x <= 0) s_enemy.transform.x = int(W.GetWidth() - 1);
	if (s_enemy.transform.x >= int(W.GetWidth())) s_enemy.transform.x = 1;
	if (s_enemy.transform.y <= 0) s_enemy.transform.y = int(W.GetHeight() - 1);
	if (s_enemy.transform.y >= int(W.GetHeight())) s_enemy.transform.y = 1;
}

void Enemy::Collision(std::list<Bullet> bullet) {
	for each (Bullet b in bullet){
		int dx = s_enemy.transform.x - b.s_bullet.transform.x;
		int dy = s_enemy.transform.y - b.s_bullet.transform.y;
		int distance = sqrt(dx * dx + dy * dy);
		if (distance < r + b.r) { //Collision detected
			kill = true;
		}
	}
};

void Enemy::ChangeSpeed(int score) {
	if (score >= 10000) mod = 0.1f;
	else if (score >= 50000) mod = 0.2f;
	else if (score >= 100000) mod = 0.3f;
	else if (score >= 200000) mod = 0.4f; 
	else if (score >= 500000) mod = 0.5f; 
}

void Enemy::Update() {
	Movement();
	if (id == ObjectID::UFO_L || id == ObjectID::UFO_S) Attack();
}

void Enemy::Draw() { s_enemy.Draw(); }; 



Bullet::Bullet(Aim p_aim) : m_aim(p_aim){
	s_bullet.objectID = ObjectID::BULLET;
	s_bullet.transform = { m_aim.xPos, m_aim.yPos, 5, 5 };
	dirX = m_aim.aimX - s_bullet.transform.x;
	dirY = m_aim.aimY - s_bullet.transform.y;
}

void Bullet::Draw() { s_bullet.Draw(); };

void Bullet::checkLimits() {
	if (s_bullet.transform.x < 0 || s_bullet.transform.x > int(W.GetWidth()) || s_bullet.transform.y < 0 || s_bullet.transform.y > int(W.GetHeight())) {
		kill = true;
	}
}

void Bullet::Update() {
	s_bullet.transform.x += dirX / v;
	s_bullet.transform.y += dirY / v;
	checkLimits();
}
