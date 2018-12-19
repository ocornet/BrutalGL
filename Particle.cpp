#include "Particle.h"

//Quantité de particules dans le jeu
int Particle::nbOfParticles = 0;

Particle::Particle(void)
{
	nbOfParticles ++;
	particleMode = FLOATING;
	lifeTime = 100;
	timer=0;
	//vitesse & rotation aléatoire
	speed = Vector3D(Vector3D::random(-1.0,1.0),Vector3D::random(-1.0,1.0),Vector3D::random(-1.0,1.0)).normalized();
	speed*=Vector3D::random(2,4);
	transform.setAxeRotation(Vector3D(Vector3D::random(-1.0,1.0),Vector3D::random(-1.0,1.0),Vector3D::random(-1.0,1.0)).normalized());


}

int Particle::getNbOfParticles(){
	return nbOfParticles;
}

void Particle::setParticleMode(ParticleMode pm){
	particleMode = pm;
}

void Particle::setLifeTime(int t){
	lifeTime = t;
}

Vector3D Particle::getSpeed(){
	return  speed;
}
void Particle::setSpeed(Vector3D spd){
	speed = spd;
}


void Particle::Update(){
	switch(particleMode){
	case FLOATING: //particule qui flotte
		transform.getPosition().translate(speed*GameObject::deltaTime);
		transform.setAngleRotation(transform.getAngleRotation() + 150*speed.length()*GameObject::deltaTime);
		speed*=0.9801;
		break;
	case FALLING: //particule qui tombe au sol
		if(transform.getPosition().getY() <= 0){ //Si on est au sol, on y reste
			transform.setPosition(transform.getPosition().getX(),0,transform.getPosition().getZ());
		}
		else{ //Sinon, on tombe
			speed += 0.5*Vector3D(0,-9.81,0)*GameObject::deltaTime;
			transform.getPosition().translate(speed*GameObject::deltaTime);
			transform.setAngleRotation(transform.getAngleRotation() + 150*0.03);
		}
		break;
	}
	//Pour une question de performance, on détruit la particule prématurément s'il y a trop de particles dans la scène
	if(nbOfParticles > MAX_PARTICLES && timer > 0){
		Destroy();
		nbOfParticles--;
	}
	timer++;

	if(timer >= lifeTime){
			Destroy();
			nbOfParticles--;
	}
}


Particle::~Particle(void)
{
}
