#include "Snowman.h"
#include "Projectile.h"

Mesh* Snowman::particle_mesh;
Mesh* Snowman::particle2_mesh;
Mesh* Snowman::proj_mesh;

int Snowman::snowmenKilled = 0;
int Snowman::wave = 0;

Snowman::Snowman(void)
{
	target = Player::getPlayer();
	collisionRadius=1;
	transform.setAxeRotation(Vector3D(0,1,0));

	//Chance de drop, réduit à chaque vague jusqu'à atteindre un minimum de 5% à la vague 15
	dropChance = (wave < 15) ? 50 - 3*wave : 5;//sur 100

	hp = 100 + 5*wave;
	speed = 0.03 + 0.002 * wave;
	//timer (utilisé comme seed pour le rythme de tir des snowmen pour qu'ils ne tirent pas tous en même temps)
	t=rand()%360; 
	shootSpeed = 0.5f;
	//durée aléatoire entre 0 et 1/shootSpeed (pour que les snowmen ne tirent pas tous en même temps)
	timeBeforeShoot = (1000*rand()%(int)((1.0/shootSpeed)*1000))/1000; 

	//Positionne le snowman aléatoirement sur un cercle de rayon 15 autour du joueur
	float r = rand()%360;
	Vector3D v = Vector3D(cos(r),0,sin(r)); 
	transform.setPosition( target->getTransform().getPosition() + 15*v); 
}

void Snowman::Update(){
	if(!target->IsDead()){
		//En fonction du comportement du bonhomme de neige (déterminé dans le constructeur)
		switch(ai){
			case RUSH: //Fonce vers le joueur en ligne droite
				transform.getPosition().translate(speed*((target->getTransform().getPosition()-transform.getPosition()).normalized()));
			break;
			case STRAFE: //Tourne autour du joueur
				transform.getPosition().translate(speed*((target->getTransform().getPosition()+10*Vector3D(cosf(t),0,sinf(t))-transform.getPosition()).normalized()));
				if(timeBeforeShoot == 0){ //Et lance des projectiles vers lui
					Projectile *fb = new Projectile();
					fb->getTransform().setPosition(transform.getPosition() + 0.25f*Vector3D(0,1,0)) ;
					fb->setDamage(1);
					fb->setLayer( ENEMY_PROJECTILE);
					fb->setMesh(proj_mesh);
					fb->setSpeed ((target->getTransform().getPosition()-fb->getTransform().getPosition()).normalized()*0.08);
					fb->setLifeTime(300);
					timeBeforeShoot = 1.0/shootSpeed;
				}
			break;
		}
		//On garde le snowman au sol, quoi qu'il arrive
		transform.setPosition(transform.getPosition().getX(), 0.75f, transform.getPosition().getZ());
		//On le fait regarder le joueur
		float angle = Vector3D::angle(transform.getPosition(), target->getTransform().getPosition());
		transform.setAngleRotation(90-(angle*180.0f/3.14159));

		//Si le joueur est trop loin, on appelle une fonction qui lui fait des dégâts
		if((target->getTransform().getPosition()-transform.getPosition()).length() > 0.5*WORLD_SIZE){
			target->TooFar();
		}

		//Rechargement
		if(timeBeforeShoot >0){
			timeBeforeShoot -=GameObject::deltaTime;
		}else{
			timeBeforeShoot = 0;
		}
	}
	//Timer
	t+=GameObject::deltaTime;
}

//Nombre de bonhommes de neige tués
int Snowman::getSnowmenKilled(){
	return snowmenKilled;
}
//Numéro de la vague
int Snowman::getWave(){
	return wave;
}
//Augmente le numéro de la vague
void Snowman::nextWave(){
	wave++;
}

//Particle de mort 1
void Snowman::setParticle_mesh(Mesh* m){
	particle_mesh = m;
}

//Particle de mort 2
void Snowman::setParticle2_mesh(Mesh* m){
	particle2_mesh = m;
}

void Snowman::setProj_mesh(Mesh* m){
	proj_mesh = m;
}

void Snowman::Die(){
	if(!isDestroyed){
		//Explosion de particules !
		for(int i = 0; i < 15; i++){
			Particle* p = new Particle();
			Vector3D dir = Vector3D(Vector3D::random(-1,1),Vector3D::random(-1,1),Vector3D::random(-1,1)).normalized()/2;
			p->getTransform().setPosition(transform.getPosition()+dir);
			p->setSpeed(Vector3D::random(3,6)*dir);
			if(i<10){
				p->setMesh(particle_mesh);
			}else{
				p->setMesh(particle2_mesh);
			}
			p->setParticleMode(FALLING);
			p->setLifeTime(500);
		}
		//Et la tête
		Particle* h = new Snowhead();
		Vector3D dir = Vector3D(0,1,0)/2;
		h->getTransform().setPosition(transform.getPosition()+dir);
		h->setSpeed(Vector3D::random(3,6)*dir);
		h->setParticleMode(FALLING);
		h->setLifeTime(500);

		//Le drop
		if(rand()%100 < dropChance){ //Si on drop quelque chose
			PowerUpType type;
			if(rand()%100 < 25-wave){ // A la vague 25, 0% de chance d'obtenir un health pack
				type = HEAL;
			}else{
				type = DAMAGE;
			}
			PowerUp* p = new PowerUp(type);
			p->getTransform().setPosition(transform.getPosition());
		}
		
		snowmenKilled++;
		Destroy();
		
	}
}
//Code pour afficher un snowman, transformé par le transform
void Snowman::Display() const{
	glPushMatrix();

	transform.Apply();
	// On va dessiner en blanc
   glColor3f(1.0f, 1.0f, 1.0f);
   
   // Corps du bonhomme de neige : une sphere
   glutSolidSphere(0.75f,20,20);
   
   // Tete de bonhomme : une sphere
   glTranslatef(0.0f, 1.0f, 0.0f);
   glutSolidSphere(0.25f,20,20);
   
   // Yeux du bonhomme de neige : deux spheres noires
   glPushMatrix();
   glColor3f(0.0f,0.0f,0.0f);
   glTranslatef(0.05f, 0.10f, 0.18f);
   glutSolidSphere(0.05f,10,10);
   glTranslatef(-0.1f, 0.0f, 0.0f);
   glutSolidSphere(0.05f,10,10);
   glPopMatrix();
   
   // Nez du bonhomme de neige : un cone orange
   glColor3f(1.0f, 0.5f , 0.5f);
   glRotatef(0.0f,1.0f, 0.0f, 0.0f);
   glutSolidCone(0.08f,0.5f,10,2);

   glPopMatrix();
}

Snowman::~Snowman(void)
{
}
