#include "scene.h"

Scene::Scene(std::vector<Entity*> _entities)
	: entities(_entities)
{
}

Scene::Scene(const Scene& _copy)
	: entities(_copy.entities)
{
}

Scene::~Scene()
{
	entities.clear(); entities.shrink_to_fit();
}

void Scene::AddEntity(Entity* _entity)
{
	entities.push_back(_entity);
}
