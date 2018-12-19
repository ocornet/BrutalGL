#include "PowerUp.h"

Mesh* PowerUp::health_mesh;
Mesh* PowerUp::damage_mesh;
Mesh* PowerUp::particle_mesh;


PowerUp::PowerUp(PowerUpType type)
{
	
	timer = 0;
	lifeTime = 15;
	flashSpeed = 4;
	transform.setAxeRotation(Vector3D(0,1,0));
	collisionRadius = 1;
	//Seulement 2 types de powerup : Soin ou Dégâts
	this->type = type;
	switch(type){
	case HEAL:
		mesh = health_mesh;
		break;
	case DAMAGE:
		mesh = damage_mesh;
		break;
	}

}

void PowerUp::setHealth_mesh(Mesh* m){
	health_mesh = m;
}
void PowerUp::setParticle_mesh(Mesh* m){
	particle_mesh = m;
}
void PowerUp::setDamage_mesh(Mesh* m){
	damage_mesh = m;
}


void PowerUp::setLifeTime(int t){
	lifeTime = t;
}

void PowerUp::Update(){
	timer += GameObject::deltaTime;

	//animation de rotation et de lévitation
	transform.setPosition(transform.getPosition().getX(), 0.5f+0.2f*cosf(2*timer), transform.getPosition().getZ());
	transform.setAngleRotation(30*timer);

	//Si on touche le joueur, on lui accorde le bonus concerné
	if(CollideWith(Player::getPlayer())){
		switch(type){
		case HEAL:
			Player::getPlayer()->Heal();
			break;
		case DAMAGE:
			Player::getPlayer()->LevelUp();
			break;

		}
		//Explosion de particules !
		for(int i = 0; i < 10; i++){
			Particle* p = new Particle();
			Vector3D dir = Vector3D(Vector3D::random(-1,1),Vector3D::random(-1,1),Vector3D::random(-1,1)).normalized()/2;
			p->getTransform().setPosition(transform.getPosition());
			p->setSpeed(Vector3D::random(4,6)*dir);
			p->setMesh( particle_mesh);
			p->setParticleMode( FLOATING);
			p->setLifeTime (100);
		}
		Destroy();
	}
	//Si il ne reste que un quart du temps de vie, on clignote
	if(timer >= 0.75f * lifeTime){
		Flash();
		if(timer >= lifeTime){
			Destroy();
		}
	}
}

//Animation de clignotement
void PowerUp::Flash(){
	if((int)(timer*flashSpeed) % 2 == 0 ){
		mesh = NULL;
	}else{
		switch(type){
		case HEAL:
			mesh = health_mesh;
			break;
		case DAMAGE:
			mesh = damage_mesh;
			break;
		}
	}
}


PowerUp::~PowerUp(void)
{
}
