#include "Player.h"
#include "Projectile.h"

Player* Player::player = NULL;

Mesh* Player::blood_mesh;
Mesh* Player::proj_mesh;

#define PLAYER_MAX_HP 3



Player::Player(void)
{
	player = this;
	timeBeforeShoot = 0;
	transform = Vector3D(0,1,0);
	isDead = false;
	hp = PLAYER_MAX_HP;
	invulnerabilityTime=0;
	healEffectTime = 0;
	levelUpEffectTime = 0;
	layer = PLAYER;
	level = 1;
	speed = 0.05;

}

float Player::getSpeed(){
	return speed;
}

void Player::setSpeed(float sp){
	speed = sp;
}

Player* Player::getPlayer(){
	return player;
}

void Player::setBlood_mesh(Mesh* m){
	blood_mesh = m;
}

void Player::setProj_mesh(Mesh* m){
	proj_mesh = m;
}

float Player::getInvulnerabilityTime(){
	return invulnerabilityTime;
}

bool Player::IsDead(){
	return isDead;
}
	
float Player::getHealEffectTime(){
	return healEffectTime;
}
	
float Player::getLevelUpEffectTime(){
	return levelUpEffectTime;
}

int Player::getLevel(){
	return level;
}


void Player::Update(){
	if(!isDead){
		timeBeforeShoot-=deltaTime;
		if(timeBeforeShoot < 0){
			timeBeforeShoot = 0;
		}
		//Si je touche un ennemi, je perd une vie
		for(GameObject* go : gameObjects){
			if(go->getLayer() == ENEMY){
				Enemy *e = (Enemy*) go;
				if(CollideWith(e)){ 
						TakeDamage(1);
					break;
				}
			}
		}
		//Temps d'effet de clignotement de l'écran
		if(invulnerabilityTime > 0){
			invulnerabilityTime -= GameObject::deltaTime;
		}else if (invulnerabilityTime < 0){
			invulnerabilityTime = 0;
		}
		if(healEffectTime > 0){
			healEffectTime -= GameObject::deltaTime;
		}else if (healEffectTime < 0){
			healEffectTime = 0;
		}
		if(levelUpEffectTime > 0){
			levelUpEffectTime -= GameObject::deltaTime;
		}else if (levelUpEffectTime < 0){
			levelUpEffectTime = 0;
		}
	}else{
		//Animation de mort, on tombe au sol
		if(transform.getPosition().getY() > 0.1){
			transform.setPosition(transform.getPosition().getX(), transform.getPosition().getY() - 1.0*GameObject::deltaTime, transform.getPosition().getZ());
		}
	}
}

//Tire une boule de feu
void Player::Shoot(Vector3D dir){
	if(!isDead){
		if(timeBeforeShoot == 0){
			Projectile *fb = new Projectile();

			//Effet cosmétique d'apparition de boule de feu à des positions aléatoires
			float random1 = 2*(float)(rand()%100)/100-1;
			float random2 = 2*(float)(rand()%100)/100-1;
			float random3 = 2*(float)(rand()%100)/100-1;
			Vector3D disp = 0.2f*random1*Vector3D(0,1,0) + 0.2f*random2*Vector3D(1,0,0) + 0.2f*random3*Vector3D(0,0,1);

			fb->getTransform().setPosition(transform.getPosition() - 0.6f*Vector3D(0,1,0)+ 1.5*dir + disp);
			fb->setLayer( PLAYER_PROJECTILE);
			fb->setMesh(proj_mesh);
			fb->setSpeed(dir*0.2);
			int dmg = GetDamage();
			fb->setDamage(dmg);
			fb->getMesh()->setColor(Vector3D((dmg < 50) ? 1.0 : ((float)(100-dmg)/50),(dmg < 50) ? (float)(dmg)/50 : 1,0));
			timeBeforeShoot = 1.0/GetShootSpeed();
		}
	}
}

//Fonction appelée pour perdre de la vie
void Player::TakeDamage(int dmg){
	//Si on est plus en temps d'invulnérabilité
	if(invulnerabilityTime ==0){
		Unit::TakeDamage(dmg);
		invulnerabilityTime = 1;
		//On pop des particules de sang
		for(int i = 0; i < 6; i++){
			Particle* p = new Particle();
			p->getTransform().setPosition(transform.getPosition());
			p->setMesh(blood_mesh);
			p->setParticleMode(FALLING);
			p->setLifeTime(500);
		}

		healEffectTime = 0;
		levelUpEffectTime = 0;
	}

}


//Soigne toute la vie du joueur
void Player::Heal(){
	hp = PLAYER_MAX_HP;
	healEffectTime = 0.1f;
}

//Augmente le niveau du joueur
void Player::LevelUp(){
	level++;
	levelUpEffectTime = 0.1f;
	speed += 0.002;
}

//Dégâts des boules de feu du joueur
int Player::GetDamage(){
	return 10 + 2*level;
}
//Vitesse de tir du joueur
float Player::GetShootSpeed(){
	float factor = 1.0+((float)level)/10;
	return 2*factor;
}

//Mort du joueur
void Player::Die(){
	isDead = true;
	//Beaucoup de sang
	for(int i = 0; i < 30; i++){
		Particle* p = new Particle();
		p->getTransform().setPosition(transform.getPosition());
		p->setMesh(blood_mesh);
		p->setParticleMode(FALLING);
		p->setLifeTime(99999);
	}
}

//Appelé par les ennemis si le joueur est trop loin d'eux
void Player::TooFar(){
	if(!isDead){
		//Prend des dégâts
		if(invulnerabilityTime == 0){
			TakeDamage(1);
			invulnerabilityTime = 3;
		}
		Print("FOOL ! GO BACK TO FIGHT !",-4,0,Vector3D(1,1,1));
	}
}

//Affiche un texte sur l'écran aux positions x,y
void Player::Print(string text, int x, int y, Vector3D color){
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();

	glLoadIdentity();

	glColor3f(color.getX(),color.getY(),color.getZ());
	glTranslatef(0,1,-10);
	glRasterPos2f(x,y);
	for(int i=0; i < text.size(); i++){
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)text[i]);
	}
	

	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

}

Player::~Player(void)
{
}
