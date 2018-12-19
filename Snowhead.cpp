#include "Snowhead.h"


Snowhead::Snowhead(void)
{

	lifeTime = 500;
}

//Affiche juste une tête de bonhomme de neige
void Snowhead::Display() const{
	glPushMatrix();

	transform.Apply();
	// Tete de bonhomme : une sphere
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


Snowhead::~Snowhead(void)
{
}
