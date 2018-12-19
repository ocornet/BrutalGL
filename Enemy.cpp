#include "Enemy.h"





Enemy::Enemy(void)
{
	target = Player::getPlayer();
	hp=100;
	ai = RUSH;
	speed = 0.03;
	layer = ENEMY;
	transform.setAxeRotation(upVector);
}


float Enemy::getSpeed(){
	return speed;
}
void Enemy::setSpeed(float spd){
	speed = spd;
}
void Enemy::setAI(AI ai){
	this->ai = ai;
}

void Enemy::Update(){
	switch(ai){
	case RUSH: //Fonce vers le joueur
		transform.getPosition().translate(speed*((target->getTransform().getPosition()-transform.getPosition()).normalized()));
		transform.setAngleRotation(transform.getAngleRotation() + 150*speed);
		break;
	}
}

void Enemy::Die(){
	Destroy();

}

Enemy::~Enemy(void)
{
}
