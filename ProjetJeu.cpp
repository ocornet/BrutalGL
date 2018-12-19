// ProjetJeu.cpp : définit le point d'entrée pour l'application console.
//
#include "Projectile.h"
#include "Enemy.h"
#include "Snowman.h"
#include "Scenery.h"
#include <random>
#include <ctime>



#define NUMBER_OF_WAVES 14

#define NUMBER_OF_SNOW 30
#define NUMBER_OF_STONES 40
#define NUMBER_OF_GRASS 6

// angles de rotation (coordonnees spheriques) pour calculer le vecteur vision de la camera
float angleTheta = 0.0f;
float anglePhi = 0.0f;

// Camera viewing vectors
Vector3D forwardView;
Vector3D rightView;
// On garde le vecteur "up" du monde : 0 1 0
Vector3D upWorld;

// Difference "Fly" vs. "FPS" modes
Vector3D forwardMove;
Vector3D rightMove;

// Camera position
Vector3D targetPos;

// Sensibilite de deplacement
float mouseRotSensitivity = 0.005f;

int HEIGHT = 600;
int WIDTH = 600;

// Déplacement "Fly" ou "FPS" ?
bool flyMode = false;

// les increments des angles theta et phi
// egales a 0 quand on n'appuie sur aucune touche
float deltaTheta = 0.0f;
float deltaPhi = 0.0f;

// increments de deplacement
// remis a 0 quand on appuis sur rien
float deltaMove = 0.0f;
float deltaStrafe = 0.0f;

// coordonnes de la souris au moment du clic gauche
int xOrigin = -1;
int yOrigin = -1;

bool buttonClick = false;

bool gameStarted = false;


//Meshes
Mesh hand_idle("Assets/hand_idle.obj");
Mesh hand_shoot("Assets/hand_shoot.obj");
Mesh fireball_mesh("Assets/fireball.obj");
Mesh snowBall_mesh("Assets/fireball.obj");
Mesh skull("Assets/skull.obj");
Mesh snowParticle_mesh("Assets/snow_particle.obj");
Mesh snowShard_mesh("Assets/shard.obj");
Mesh fbShard_mesh("Assets/shard.obj");
Mesh blood_mesh("Assets/shard.obj");
Mesh health_mesh("Assets/health.obj");
Mesh damage_mesh("Assets/sword.obj");
Mesh healParticle_mesh("Assets/shard.obj");
Mesh snow_mesh("Assets/snow.obj");
Mesh stone_mesh("Assets/stone.obj");
Mesh grass_mesh("Assets/grass.obj");

//Transforms
Transform transform_hand(Vector3D(0,-0.5,-1),0,rightVector);
Transform transform_fireball(Vector3D(0,1,0));

//GameObjects
GameObject hand(transform_hand, &hand_idle);
Player player;

//Quantité d'ennemi à chaque vague
int waveNumbers[NUMBER_OF_WAVES] = {1,2,4,5,6,8,10,12,13,14,15,16,17,18};

//Temps
clock_t t;



// Fonction de redimensionnement de la fenetre
void redimensionner(int w, int h) {
   
   // On evite une division par 0
   // la fenetre ne peut avoir une largeur de 0
   if (h == 0)
      h = 1;
   
   // Calcul du ratio
   float ratio =  (w * 1.0) / h;
   
   // On passe en mode "matrice de projection"
   glMatrixMode(GL_PROJECTION);
   
   // on charge la matrice identite
   glLoadIdentity();
   
   // on definit le viewport pour prendre toute la fenetre
   glViewport(0, 0, w, h);
   
   // on definit la projection perspective
   gluPerspective(65.0f, ratio, 0.1f, 100.0f);

   WIDTH = w;
   HEIGHT = h;
   
   // on repasse en mode "matrice modelview"
   glMatrixMode(GL_MODELVIEW);
}

// Fonction de dessin d'un bonhomme de neige
void drawSnowMan() {
   // On va dessiner en blanc
   glColor3f(1.0f, 1.0f, 1.0f);

   
   // Corps du bonhomme de neige : une sphere
   glTranslatef(0.0f ,0.75f, 0.0f);
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
}


// Fonction de gestion du deplacement de la camera
void cameraMovement(float dM, float dS) {
	//Si on se déplace en diagonale, on se déplace à la même vitesse que si on se déplaçait que vers l'avant
	if(dM != 0 && dS != 0) {
		dM/=1.414;
	}

   // est-on en "fly" mode ou non ?
   if(flyMode) {
		
		player.getTransform().getPosition().translate( forwardView*dM + rightView*dS);
		targetPos += forwardView*dM + rightView*dS;
   }
   else { // FPS mode

	   if(!player.IsDead()){
		    Vector3D projection = forwardView;
			projection.setY(0);
			player.getTransform().getPosition().translate( projection*dM + rightView*dS);
			targetPos += projection*dM + rightView*dS;
	   }
   }
   
   
}

//Clignotement de l'écran
float cligSpeed = 3; //fréquence de clignotement
float alpha = 2; //ratio cyclique de clignotement
float timeBeforeClig = 0;

float timeBeforeTextDisappear = 0;

// fonction d'affichage de notre scene 3D
void affichageScene() {
   t = clock();
	
   // On gere le deplacement de la camera
   cameraMovement(deltaMove, deltaStrafe);

   //Si le joueur n'a plus qu'un point de vie, on clignote moins vite
   if(player.getHp() <= 1 && player.getInvulnerabilityTime() == 0){
	   cligSpeed = 1;
	   alpha = 4;
   }else{
	   cligSpeed = 3;
	   alpha = 2;
   }
   //Si le joueur est low life ou en train d'être blessé, on clignote à une fréquence cligSpeed avec un ratio cyclique alpha
   if((player.getHp() <= 1 || player.getInvulnerabilityTime()>0 ) && timeBeforeClig  <= 1.0/(alpha*cligSpeed) )
   {
		glClearColor(1,0,0,0); //rouge
		timeBeforeClig = 1.0/cligSpeed;
   }//Sinon, si il se soigne, on flash en vert
   else if(player.getHealEffectTime() >0)
   {
		glClearColor(0,0.7f,0,0);
   }//Sinon, si il monte de niveau, on flash en jaune
   else if(player.getLevelUpEffectTime() > 0)
   {
	    glClearColor(0.8f,0.7f,0,0);
   }
   else //Si on est dans aucun de ces cas, on clear avec la couleur normale
   {
	    
	    glClearColor(0.10,0.25,0.5,0);
   }
   timeBeforeClig -=GameObject::getDeltaTime();

   // On efface les tampons de couleur et de profondeur
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   // On passe en matice modelview
   glMatrixMode(GL_MODELVIEW);
   
   // on charge la matrice identite
   glLoadIdentity();
   

   // on definit la position de la camera et ou elle regarde
   gluLookAt(player.getTransform().getPosition().getX(),player.getTransform().getPosition().getY(),player.getTransform().getPosition().getZ(),targetPos.getX(),targetPos.getY(),targetPos.getZ(),0,1,0);
   
   
   // On dessine le "sol" : un plan gris
   glDisable(GL_LIGHTING);
   glColor3f(0.9f, 0.9f, 0.9f);
   glBegin(GL_QUADS);
   glNormal3f(0,1,0);
   //On dessine toujours le sol centré sur le joueur
   float offsetX = player.getTransform().getPosition().getX();
   float offsetZ = player.getTransform().getPosition().getZ();
   glVertex3f(offsetX-WORLD_SIZE, 0.0f, offsetZ-WORLD_SIZE);
   glVertex3f(offsetX-WORLD_SIZE, 0.0f, offsetZ+WORLD_SIZE);
   glVertex3f(offsetX+WORLD_SIZE, 0.0f, offsetZ+WORLD_SIZE);
   glVertex3f(offsetX+WORLD_SIZE, 0.0f, offsetZ-WORLD_SIZE);
   glEnd();
   glEnable(GL_LIGHTING);


   //Fonction qui affiche tous les GameObjects
	GameObject::DisplayAllGameObjects();

	//Si le joueur a cliqué et a commencé le jeu
   if(gameStarted){

	   bool waveDefeated = true;
	   //Si tous les ennemis sont morts (i.e. n'existent plus), la vague est vaincue
	   for(GameObject* go : GameObject::getGameObjects()){
		   if(go->getLayer() == ENEMY){
			   waveDefeated = false;
		   }
	   }

	   //Si la vague est vaincue, on passe à la prochaine vague
	   if(waveDefeated){
		   Snowman::nextWave();
		   //Si on dépasse le nombre max de vagues, on répète indéfiniment la dernière vague
		   for(int i=0; i< waveNumbers[(Snowman::getWave() > NUMBER_OF_WAVES-1) ? NUMBER_OF_WAVES-1 : Snowman::getWave()];i++){
			   Snowman* s = new Snowman();
			   if(Snowman::getWave() >1){
				   //A partir de la vague 2, 50% de chance qu'un ennemi attaque à distance
				   if(rand()%100 <50){
					   s->setAI(STRAFE);
				   }
			   }
		   }
		   timeBeforeTextDisappear = 3;
	   }

		  //On affiche le numéro de la vague au début de celle-ci
		if(timeBeforeTextDisappear >0){
		   Player::Print("WAVE " + to_string(Snowman::getWave()+1),-1,0,Vector3D(1,1,1) );
		   timeBeforeTextDisappear -= GameObject::getDeltaTime();
	   }

   }
   

	// Si le joueur meurt, on affiche son score
   if(player.IsDead()){
	   hand.setMesh(NULL);
	   Player::Print("YOU DIED AT WAVE " + to_string(Snowman::getWave()+1), -4,0,Vector3D(1,0,0));
	   Player::Print("YOU KILLED " + to_string(Snowman::getSnowmenKilled()) + " SNOWMEN", -4,-1,Vector3D(1,0,0));
   }
   else
   {
	//Si on clique, on change le mesh de la main
	if(buttonClick){
		hand.setMesh(&hand_shoot);
		Player::getPlayer()->Shoot(forwardView);
	}else{
		hand.setMesh(&hand_idle);
	}
   }

      //Génération du premier ennemi au clic du joueur
   if(!gameStarted ){
	   if(buttonClick){	      
			Snowman* snowman = new Snowman();
			gameStarted = true;
			timeBeforeTextDisappear = 3;
	   }else{ //En attendant, on affiche l'écran titre
		   player.Print("BRUTAL GL", -1,0,Vector3D(1,0,0));
		   player.Print("Click once on the screen to start", -3,-1,Vector3D(1,1,1));
	   }
   }

	//On appelle la fonction Update de chaque GameObject
	GameObject::UpdateAllGameObjects();
	
	//On enlève tous les GameObject qui ont été détruit
	GameObject::RemoveDestroyedGameObjects();

   
   // on echange les tampons d'affichage
   glutSwapBuffers();

   //On met à jour le deltaTime
   t= clock()-t;
   GameObject::setDeltaTime (((float)t)/CLOCKS_PER_SEC);


   
}


// Fonction de gestion du clavier (touche enfoncee)
void clavier(unsigned char key, int xx, int yy) {
   
   // Quelle touchea ete appuyee ?
   switch(key) {
         // Q et S on strafe
      case 'q' :
      case 'Q' :
         deltaStrafe = -player.getSpeed();
		 
         break;
      case 'd' :
      case 'D' :
         deltaStrafe = player.getSpeed();
         break;
         
         // Z et S avance/recule
      case 'z' :
      case 'Z' :
         deltaMove = player.getSpeed();

         break;
      case 's' :
      case 'S' :
         deltaMove = -player.getSpeed();

         break;
         
         // F pour activer/desactiver le mode "fly" // Enlevé pour le jeu
      case 'f' :
      case 'F' :
         //flyMode = !flyMode;
         // On force la camera a revenir sur le sol :)
		break;

         // ESCAPE on termine l'application

      case 27 :
         exit(0);
         break;
   }
   
}

// Fonction de gestion du clavier (touche relachee)
void clavierUp(unsigned char key, int x, int y) {
   
   // Que faire quand une touche est relachee
   switch (key) {
         
         // On arrete de strafer
      case 'q' :
      case 'd' :
      case 'Q' :
      case 'D' :
         deltaStrafe = 0;
         break;
         
         // On arrete d'avance/reculer
      case 'z' :
      case 's' :
      case 'Z' :
      case 'S' :
         deltaMove = 0;
         break;
   }
}


// Fonction de gestion du clavier (touche speciale appuyee)
void specialKeyDown(int key, int xx, int yy) {
   
   switch (key) {
         // Deplacement avance/recul
      case GLUT_KEY_UP :
         deltaMove = player.getSpeed();
         break;
      case GLUT_KEY_DOWN :
		deltaMove = -player.getSpeed();
         break;
         
         // Strafe a gauche/droite
      case GLUT_KEY_RIGHT :
         deltaStrafe = player.getSpeed();
         break;
      case GLUT_KEY_LEFT :
         deltaStrafe = -player.getSpeed();
         break;
      default :
         break;
   }
   
}

// Fonction de gestion du clavier (touche speciale relachee)
void releaseSpecialKey(int key, int x, int y) {
   
   switch (key) {
         // On remet le delate deplacement a 0
      case GLUT_KEY_UP :
      case GLUT_KEY_DOWN :
         deltaMove = 0;
         break;
         
         // On remet le delta strafe a 0
      case GLUT_KEY_RIGHT :
      case GLUT_KEY_LEFT :
         deltaStrafe = 0;
         break;
   }
}

//Inutilisé :
// Fonction de gestion du deplacement de la souris
void deplacementSouris(int x, int y) {
   
   // On ne fait quelque chose que si l'utilisateur
   // a deja clique quelque part avec le bouton gauche
   if (xOrigin >= 0 || yOrigin >= 0) {
      
      // mise a jour des deltas des angles theta et phi
      deltaTheta = -mouseRotSensitivity*(float)(x-xOrigin);
	  deltaPhi = -mouseRotSensitivity*(float)(y-yOrigin);
      
      // Calcul du nouveau vecteur vision :
      
      // En se basant sur les coordonnes spheriques
      forwardView.setX(-sin(angleTheta+deltaTheta)*cos(anglePhi+deltaPhi));
	  forwardView.setY(sin(anglePhi+deltaPhi));
	  forwardView.setZ(-cos(angleTheta+deltaTheta)*cos(anglePhi+deltaPhi));

      // normalisation du vecteur forward
      forwardView = forwardView.normalized();
      
      // Up ne change pas
      // et right est le cross product entre up et forward
      rightView = forwardView.crossProduct(upWorld);
      
      
      // Mettre a jour la cible = point "vise" par la camera
     targetPos = player.getTransform().getPosition() + forwardView;
      // Pour le mode "FPS" on force les deplacements a avoir lieu sur le plan de la camera !
      // Mise a jour des vecteurs forwardMove et rightMove

	 glutSetCursor(GLUT_CURSOR_NONE);
     if(x > WIDTH){
		glutWarpPointer(x-(WIDTH),y);
		xOrigin -= (WIDTH);
	}else if(x < 0){
		glutWarpPointer(x+(WIDTH),y);
		xOrigin += (WIDTH);
	 }
   }
}

//Inutilisé :
// Fonction de gestion des clics souris
void clicSouris(int button, int state, int x, int y) {
   
   // On ne fait quelque chose que sur le bouton gauche de la souris
   if (button == GLUT_LEFT_BUTTON || button == GLUT_RIGHT_BUTTON) {   
      // si on relache le bouton on met a jour les angles theta et phi
      // et on dit que l'on a pas clique
      if (state == GLUT_UP) {
		  angleTheta += deltaTheta;
			anglePhi += deltaPhi;
			if(button == GLUT_LEFT_BUTTON){
				buttonClick = false;
			}
      }
      else  {// state = GLUT_DOWN
         // si l'on a clique sur le bouton gauche
         // on garde les positions de la souris au moment du clic gauche

			deltaTheta=0;
			deltaPhi=0;
			xOrigin = x;
			yOrigin = y;
			if(button == GLUT_LEFT_BUTTON){
				buttonClick = true;
			}
      }
   }
}


// Fonction de gestion du deplacement de la souris
void deplacementSouris_passive(int x, int y) {
   // On ne fait quelque chose que si l'utilisateur
   // a deja clique quelque part avec le bouton gauche
   if (buttonClick) {
      
      // mise a jour des deltas des angles theta et phi
      deltaTheta = -mouseRotSensitivity*(float)(x-xOrigin);
	  deltaPhi = -mouseRotSensitivity*(float)(y-yOrigin);
      
	  // Calcul du nouveau vecteur vision :
      
      // En se basant sur les coordonnes spheriques
      forwardView.setX(-sin(angleTheta+deltaTheta)*cos(anglePhi+deltaPhi));
	  forwardView.setY(sin(anglePhi+deltaPhi));
	  forwardView.setZ(-cos(angleTheta+deltaTheta)*cos(anglePhi+deltaPhi));

      // normalisation du vecteur forward
      forwardView = forwardView.normalized();
      
      // Up ne change pas
      // et right est le cross product entre up et forward
      rightView = forwardView.crossProduct(upWorld);
      
      
      // Mettre a jour la cible = point "vise" par la camera
     targetPos = player.getTransform().getPosition() + forwardView;
      // Pour le mode "FPS" on force les deplacements a avoir lieu sur le plan de la camera !
      // Mise a jour des vecteurs forwardMove et rightMove

	 //Curseur invisible
	 glutSetCursor(GLUT_CURSOR_NONE);

	 //On calcule la distance entre le curseur et le centre de la fenêtre
	xOrigin -= x - WIDTH/2;
	yOrigin -= y - HEIGHT/2;

	//Si le curseur bouge, on le recentre au centre de la fenêtre
	 if(x != WIDTH/2 || y != HEIGHT/2) 
	 {
		 glutWarpPointer(WIDTH/2,HEIGHT/2);
	 }

   }
}

// Fonction de gestion des clics souris
void clicSouris_passive(int button, int state, int x, int y) {
   // On ne fait quelque chose que sur le bouton gauche de la souris

   if (button == GLUT_LEFT_BUTTON) { 
	   //Si on clique 1 fois, le clic est activé (ne plus cliquer ensuite)
	   if (!buttonClick){
		   xOrigin = x;
		   yOrigin = y;
	   }

		buttonClick = true;
   }
}


// Le main
int main(int argc, char **argv) {
	srand(time(NULL));
	
   
   // init GLUT and create window
   glutInit(&argc, argv);
   
   glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
   glutInitWindowPosition(100,100);
   glutInitWindowSize(WIDTH,HEIGHT);
   glutCreateWindow("BrutalGL");
   glClearColor(0.10,0.25,0.5,0);
   
   // enregistrement des callbacks d'affichage
   // de redimensionnement et d'idle
   glutDisplayFunc(affichageScene);
   glutReshapeFunc(redimensionner);
   glutIdleFunc(affichageScene);
   
   // pour que l'on puisse rester appuye sur les touches
   glutIgnoreKeyRepeat(1);
   
   // Declaration des callbacks clavier
   glutKeyboardFunc(clavier);
   glutKeyboardUpFunc(clavierUp);
   glutSpecialFunc(specialKeyDown);
   glutSpecialUpFunc(releaseSpecialKey);
   
   // declaration des callbacks souris
   /*
   glutMouseFunc(clicSouris);
   glutMotionFunc(deplacementSouris);
   */
   glutMouseFunc(clicSouris_passive);
   glutPassiveMotionFunc(deplacementSouris_passive);


   // on active le tampon de profondeur
   glEnable(GL_DEPTH_TEST);
   
   //on active la lumière
   glEnable(GL_LIGHTING);
   glEnable(GL_COLOR_MATERIAL);
   glEnable(GL_LIGHT0);

   
   // on initialise les vecteurs 'view'
   forwardView = Vector3D(0,0,-1);
   upWorld = Vector3D(0,1,0);
   rightView = Vector3D(1,0,0);
   
   // Pour le FPS mode
   forwardMove = Vector3D(0,0,-1);
   rightMove = Vector3D(1,0,0);
   
   // on initialise la cible a partir de la camera et du vecteur vision
   targetPos = player.getTransform().getPosition() + forwardView;
   

   ///Configuration des meshes
   //Main
   hand_idle.setColor(Vector3D(0.9,0.7,0.5));
   hand_shoot.setColor( Vector3D(0.9,0.7,0.5));
   hand_idle.setIsOnHUD(true);
   hand_shoot.setIsOnHUD (true);
   //Boules de feu
   fireball_mesh.setColor (Vector3D(1,0.3,0));
   Player::setProj_mesh (&fireball_mesh);
   //Sang
   blood_mesh.setColor (Vector3D(1,0,0));
   Player::setBlood_mesh( &blood_mesh);
   //Crâne (inutilisé)
   skull.setColor(Vector3D(0.6,0.2,0.2));

   //Bonhommes de neige
   snowParticle_mesh.setColor ( Vector3D(1,1,1));
   snowBall_mesh.setColor (Vector3D(1,1,1));
   snowShard_mesh.setColor ( Vector3D(1,1,1));
   Snowman::setParticle_mesh (&snowParticle_mesh);
   Snowman::setParticle2_mesh (&snowShard_mesh);
   Snowman::setProj_mesh(&snowBall_mesh);

   //Particle de boule de feu
   fbShard_mesh.setColor (fireball_mesh.getColor());
   Projectile::setParticle_mesh( &fbShard_mesh);

   //PowerUps
   health_mesh.setColor(Vector3D(1,0,0));
   healParticle_mesh.setColor (Vector3D(0.8,0.8,0));
   damage_mesh.setColor (Vector3D(0.8,0.6,0));
   PowerUp::setHealth_mesh (&health_mesh);
   PowerUp::setParticle_mesh( &healParticle_mesh);
   PowerUp::setDamage_mesh (&damage_mesh);

   //Décor
   snow_mesh.setColor(Vector3D(0.85,0.85,0.85));
   snow_mesh.setReactToLight(false);
   stone_mesh.setColor(Vector3D(0.1,0.1,0.1));
   grass_mesh.setColor(Vector3D(0,0.5,0));

   
   
  //On setup le décor avec le nombre de tas de neige, de pierres et d'épi d'herbe renseigné
   for(int i=0; i<NUMBER_OF_SNOW+NUMBER_OF_STONES+NUMBER_OF_GRASS;i++){
	   Scenery* s = new Scenery();
	   if(i<NUMBER_OF_SNOW){
		   s->setMesh(&snow_mesh);
	   }
	   else if(i<NUMBER_OF_SNOW+NUMBER_OF_STONES) s->setMesh(&stone_mesh);
	   else s->setMesh(&grass_mesh);

	   s->getTransform().setAngleRotation(rand()%360);
	   float random1 = Vector3D::random(-WORLD_SIZE,WORLD_SIZE);
	   float random2 = Vector3D::random(-WORLD_SIZE,WORLD_SIZE);
	   
	   s->getTransform().setPosition(random1,0,random2);

   }




   // enter GLUT event processing cycle
   glutMainLoop();
   
   return 1;
}
