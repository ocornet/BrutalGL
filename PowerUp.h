#pragma once

#include "Player.h"
#include "Particle.h"

enum PowerUpType{ HEAL, DAMAGE};

class PowerUp :
	public GameObject
{
protected:
	static Mesh* health_mesh;
	static Mesh* particle_mesh;
	static Mesh* damage_mesh;

	PowerUpType type;
	float timer;
	float lifeTime;
	float flashSpeed;

public:
	
	static void setHealth_mesh(Mesh* m);
	static void setParticle_mesh(Mesh* m);
	static void setDamage_mesh(Mesh* m);


	void setLifeTime(int t);

	PowerUp(PowerUpType);
	virtual void Update();
	void Flash();
	~PowerUp(void);
};

