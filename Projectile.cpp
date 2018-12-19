#include "Projectile.h"

Mesh* Projectile::particle_mesh;

Projectile::Projectile(){
	lifeTime = 100;
	timer=0;
	damage = 10;
	layer = PLAYER_PROJECTILE;
}

Vector3D Projectile::getSpeed(){
	return speed;
}

void Projectile::setSpeed(Vector3D spd){
	speed = spd;
}

void Projectile::setLifeTime(int t){
	lifeTime = t;
}
void Projectile::setDamage(int dmg){
	damage = dmg;
}

void Projectile::setParticle_mesh(Mesh* m){
	particle_mesh = m;
}

void Projectile::Update(){
	//Avance à vitesse constante
	transform.getPosition().translate(speed);

	timer++;
	if(timer >= lifeTime){
		Destroy();
	}

	for(GameObject* go : gameObjects){
		//Si le proj rentre dans une unité qu'il est censé blesser (ie le joueur si c'est un projectile ennemi ou un ennemi si c'est un projectile du joueur)
		if((layer == PLAYER_PROJECTILE && go->getLayer() == ENEMY) || (layer == ENEMY_PROJECTILE && go->getLayer() == PLAYER)){
			Unit *u = (Unit*) go; //On prend la classe parent Unit
			if(CollideWith(u) && ! isDestroyed){ 
				u->TakeDamage(damage); //Et on lui inflige des dégâts
				//Petite explosion de particules !
				for(int i = 0; i < 2; i++){
					Particle* p = new Particle();
					p->getTransform().setPosition(transform.getPosition());
					p->setMesh(particle_mesh);
					p->getMesh()->setColor(mesh->getColor());
				}

				Destroy();
				break;
			}
		}
	}
}

Projectile::~Projectile(){

}

