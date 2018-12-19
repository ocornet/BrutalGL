#pragma once
#include "particle.h"

class Snowhead :
	public Particle
{
public:
	Snowhead(void);
	virtual void Display() const;
	~Snowhead(void);
};

