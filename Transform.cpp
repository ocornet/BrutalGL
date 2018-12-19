#include "Transform.h"

Transform::Transform(){
	Init();
}

Transform::Transform(Vector3D position){
	Init();
	this->position= position;
}

Transform::Transform(Vector3D position, float angleRotation, Vector3D axeRotation){
	Init();
	this->position = position;
	this->angleRotation = angleRotation;
	this->axeRotation = axeRotation;
}

Transform::Transform(Transform * parent){
	Init();
	this->parent = parent;
}

Transform::Transform(Transform * parent, Vector3D position, float angleRotation, Vector3D axeRotation){
	this->position = position;
	this->angleRotation = angleRotation;
	this->axeRotation = axeRotation;
	this->parent = parent;
}

void Transform::Init(){
	position = Vector3D(0,0,0);
	angleRotation = 0;
	axeRotation = Vector3D(0,1,0);
	parent = NULL;
}

Vector3D& Transform::getPosition(){
	return position;
}

void Transform::setPosition(Vector3D p){
	position = p;
}

void Transform::setPosition(double x, double y, double z){
	position.setX(x);
	position.setY(y);
	position.setZ(z);
}

float Transform::getAngleRotation(){
	return angleRotation;
}

void Transform::setAngleRotation(float a){
	angleRotation = a;
}

Vector3D& Transform::getAxeRotation(){
	return axeRotation;
}

void Transform::setAxeRotation(Vector3D axeRotation){
	this->axeRotation = axeRotation;
}

//Applique la transformation à la matrice OpenGL
void Transform::Apply() const{
	if(parent!=NULL){
		parent->Apply();
	}
	glTranslatef(position.getX(),position.getY(),position.getZ());
	glRotatef(angleRotation,axeRotation.getX(),axeRotation.getY(),axeRotation.getZ());
}


void Transform::SetParent(Transform* parent){
	this->parent = parent;
}




