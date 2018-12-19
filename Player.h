#pragma once
#include "Unit.h"
//#include "Projectile.h"


#define WORLD_SIZE 100

class Projectile;

class Player : public Unit
{
protected:
	static Player* player;

	static Mesh* blood_mesh;
	static Mesh* proj_mesh;

	float invulnerabilityTime;
	bool isDead;
	
	float healEffectTime;
	float levelUpEffectTime;

	float timeBeforeShoot;
	int level;

	float speed;
public:
	float getSpeed();
	void setSpeed(float sp);

	static Player* getPlayer();

	static void setBlood_mesh(Mesh* m);

	static void setProj_mesh(Mesh* m);

	float getInvulnerabilityTime();
	bool IsDead();
	
	float getHealEffectTime();
	float getLevelUpEffectTime();

	int getLevel();

	static void Print(string text,int x, int y, Vector3D color);

	int GetDamage();
	float GetShootSpeed();
	
	Player(void);
	virtual void Update();
	virtual void Player::Die();
	void Heal();
	void LevelUp();
	void TakeDamage(int dmg);
	void Shoot(Vector3D dir);
	void TooFar();
	~Player(void);
};

