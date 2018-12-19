#pragma once

#include "Enemy.h"
#include "Snowhead.h"
#include "PowerUp.h"



class Projectile;

class Snowman : public Enemy
{
protected:
	static int snowmenKilled;
	static int wave;

	static Mesh* particle_mesh;
	static Mesh* particle2_mesh;
	static Mesh* proj_mesh;

	float t;
	float shootSpeed;
	float timeBeforeShoot;
public:

	static int getSnowmenKilled();
	static int getWave();
	static void nextWave();

	static void setParticle_mesh(Mesh* m);
	static void setParticle2_mesh(Mesh* m);
	static void setProj_mesh(Mesh* m);



	Snowman(void);
	virtual void Update();
	virtual void Die();
	virtual void Display() const;
	~Snowman(void);
};

