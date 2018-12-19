#pragma once

#include "GameObject.h"

class Unit : public GameObject
{
protected:
	int hp;
public:
	int getHp();
	void setHp(int hp);
	virtual void TakeDamage(int dmg);
	virtual void Die();
	Unit();
	~Unit(void);
};

