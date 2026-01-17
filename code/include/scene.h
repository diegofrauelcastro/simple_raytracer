#pragma once

#include "entity.h"
#include <vector>

class Scene
{
private:
	/// SCENE PROPERTIES ///

	std::vector<Entity*> entities;

public:
	/// CONSTRUCTOR & DESTRUCTOR ///

	Scene() = default;
	Scene(std::vector<Entity*> _entities);
	Scene(const Scene& _copy);
	~Scene();

	
	/// METHODS ///

	void AddEntity(Entity* _entity);


	/// GETTERS ///

	const std::vector<Entity*>& GetEntities() const { return entities; }
};