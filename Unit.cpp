#include "Unit.h"


Unit::Unit()
{
}

int Unit::getHp(){
	return hp;
}

void Unit::setHp(int hp){
	this->hp = hp;
}

void Unit::TakeDamage(int dmg){
	hp-=dmg;
	if(hp<=0){
		Die();
	}
}

void Unit::Die(){
	Destroy();
}


Unit::~Unit(void)
{
}
