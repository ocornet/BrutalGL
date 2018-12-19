#pragma once
#include "GameObject.h"

#define MAX_PARTICLES 100

enum ParticleMode { FLOATING, FALLING};

class Particle : public GameObject
{
protected:
	static int nbOfParticles;

	ParticleMode particleMode;
	int lifeTime;
	int timer;
	Vector3D speed;

public:

	static int getNbOfParticles();

	void setParticleMode(ParticleMode pm);
	void setLifeTime(int t);

	Vector3D getSpeed();
	void setSpeed(Vector3D spd);

	virtual void Update();
	Particle();
	~Particle(void);
};

