#include "GameObject.h"

std::vector<GameObject*> GameObject::gameObjects;

std::vector<GameObject*> GameObject::gameObjectsToAdd;

bool GameObject::inUpdate;

float GameObject::deltaTime = 0.01;



GameObject::GameObject()
{
	Init();
}

GameObject::GameObject(Transform t)
{
	transform = t;
	Init();
}

GameObject::GameObject(Transform t, Mesh* m){
	transform = t;
	mesh = m;
	Init();
}

void GameObject::Init(){
	if(!inUpdate){ //On ajoute l'objet dans la liste des GameObjects
		gameObjects.push_back(this);
	}else{ //Pour éviter des soucis quand on ajoute des GameObjects dans la liste des GameObjects pendant qu'on la parcourt, on les ajoute à la fin de la frame
		gameObjectsToAdd.push_back(this);
	}
	isDestroyed = false;
	collisionRadius = 0.2f;
	layer = DEFAULT;
}

float GameObject::getDeltaTime(){
	return deltaTime;
}

void GameObject::setDeltaTime(float t){
	deltaTime = t;
}

std::vector<GameObject*> GameObject::getGameObjects(){
	return gameObjects;
}

Transform& GameObject::getTransform(){
	return transform;
}

void GameObject::setTransform(Transform t){
	transform = t;
}

float GameObject::getCollisionRadius(){
	return collisionRadius;
}

void GameObject::setCollisionRadius(float r){
	collisionRadius =r;
}

CollisionLayer GameObject::getLayer(){
	return layer;
}

void GameObject::setLayer(CollisionLayer l){
	layer = l;
}

Mesh* GameObject::getMesh(){
	return mesh;
}

void GameObject::setMesh(Mesh* m){
	mesh = m;
}

bool GameObject::IsDestroyed(){
	return isDestroyed;
}

void GameObject::Update(){

}

bool GameObject::CollideWith(GameObject* other){
	return ((transform.getPosition()-other->transform.getPosition()).length() < collisionRadius + other->collisionRadius);
}

GameObject::~GameObject(void)
{
}

void GameObject::Display() const{
	if(mesh != NULL){
		mesh->Display(&transform);
	}
}

void GameObject::UpdateAllGameObjects(){
	inUpdate = true;
	for(GameObject* go : gameObjects){
		go->Update();
	}

	//A la fin des update, on ajoute les objects qui y ont été créé
	gameObjects.insert( gameObjects.end(), gameObjectsToAdd.begin(), gameObjectsToAdd.end() );
	gameObjectsToAdd.clear();

	inUpdate = false;
}

void GameObject::DisplayAllGameObjects(){
	for(GameObject* go : gameObjects){
		go->Display();
	}
}

void GameObject::RemoveDestroyedGameObjects(){
	gameObjects.erase(std::remove_if(gameObjects.begin(), gameObjects.end(),[](GameObject* go) { return go->isDestroyed; }), gameObjects.end());
}



void GameObject::Destroy(){
	isDestroyed=true;
}