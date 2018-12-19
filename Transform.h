#pragma once

#include "vector3d.h"

#include <Windows.h>
#include <GL/GL.h>
#include <GL/glut.h>

#include <vector>

class Transform
{
private:
	void Init();
	Transform * parent;


	Vector3D position;
	float angleRotation;
	Vector3D axeRotation;

public:
	
	
	Vector3D& getPosition();
	void setPosition(Vector3D p);
	void setPosition(double x, double y, double z);

	float getAngleRotation();
	void setAngleRotation(float a);

	Vector3D& getAxeRotation();
	void setAxeRotation(Vector3D axeRotation);
	

	Transform();
	Transform(Vector3D position);
	Transform(Vector3D position, float angleRotation, Vector3D axeRotation);
	Transform(Transform * parent);
	Transform(Transform * parent, Vector3D position, float angleRotation, Vector3D axeRotation);



	void Apply() const; //Applique les transformations à la matrice actuelle
	void AddChild(Transform* child);

	void SetParent(Transform * parent);

};