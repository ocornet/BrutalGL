#pragma once

#include "Mesh.h"
#include <algorithm>

enum CollisionLayer {
	DEFAULT, ENEMY, PLAYER, ENEMY_PROJECTILE, PLAYER_PROJECTILE
};

class GameObject
{	
protected:
	static float deltaTime;
	static std::vector<GameObject*> gameObjects;
	static std::vector<GameObject*> gameObjectsToAdd; //GameObjects qui sont créés dans les Update et qu'on ajoutera à la fin de la frame
	static bool inUpdate;
	Transform transform;
	float collisionRadius;
	CollisionLayer layer;
	Mesh* mesh;
	bool isDestroyed;

public:
	//Le temps écoulé en 1 frame
	static float getDeltaTime();
	static void setDeltaTime(float t);

	//Liste des GameObjects dans la scène
	static std::vector<GameObject*> getGameObjects();

	Transform& getTransform();
	void setTransform(Transform t);

	//Le rayon de la sphère de collision
	float getCollisionRadius();
	void setCollisionRadius(float r);

	CollisionLayer getLayer();
	void setLayer(CollisionLayer l);

	Mesh* getMesh();
	void setMesh(Mesh* m);

	bool IsDestroyed();

	void Init();
	GameObject();
	GameObject(Transform t);
	GameObject(Transform t, Mesh* m);
	virtual void Update();
	bool CollideWith(GameObject* other);
	virtual void Display() const;

	void Destroy();
	~GameObject(void);

	static void UpdateAllGameObjects();
	static void DisplayAllGameObjects();
	static void RemoveDestroyedGameObjects();
};

