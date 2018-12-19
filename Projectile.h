#pragma once

#include "Enemy.h"

class Projectile : public GameObject{
protected:
	Vector3D speed;
	int timer;
	int lifeTime;
	int damage;
	static Mesh* particle_mesh;
public:
	Vector3D getSpeed();
	void setSpeed(Vector3D spd);
	void setLifeTime(int t);
	void setDamage(int dmg);
	static void setParticle_mesh(Mesh* m);

	Projectile();
	virtual void Update();
	~Projectile();
};