#pragma once

#include "Player.h"
#include "Particle.h"

#define NB_PARTICLES 10

enum AI { 
	RUSH, STRAFE
};

class Enemy : public Unit
{
protected:

	float dropChance;
	Player* target;
	float speed;
	AI ai;

public:


	float getSpeed();
	void setSpeed(float spd);
	void setAI(AI ai);
	static void setParticle_mesh(Mesh* m);

	virtual void Update();
	virtual void Die();
	Enemy(void);
	~Enemy(void);
};

