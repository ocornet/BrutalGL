#include "Scenery.h"


Scenery::Scenery(void)
{
	shiftDistance = WORLD_SIZE;
}

void Scenery::Update(){
	//Distance entre le joueur et le décor
	Vector3D u = transform.getPosition() - Player::getPlayer()->getTransform().getPosition();

	//Si cette distance dépasse le shiftDistance, on déplace le décor
	if(u.length() > shiftDistance) {
		float angle = Vector3D::angle(Vector3D(1,0,0),u); //angle par rapport au joueur au moment de la disparition
		float newAngle = angle+3.1415f;
		Vector3D v = Vector3D(cos(newAngle),0,sin(newAngle))*(shiftDistance-10); //on fait apparaître l'objet à l'angle opposé de sa disparition
		Vector3D newPos = Vector3D(Player::getPlayer()->getTransform().getPosition().getX() + v.getX(), 0,Player::getPlayer()->getTransform().getPosition().getZ() + v.getZ());
		transform.setPosition(newPos);

	}
}

Scenery::~Scenery(void)
{
}
