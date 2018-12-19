#pragma once
#include "Player.h"
class Scenery :
	public GameObject
{
private:
	float shiftDistance;
public:
	Scenery(void);
	virtual void Update();
	~Scenery(void);
};

