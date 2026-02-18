#include "scene.h"
#include "mesh_renderer_component.h"
#include "script_component.h"
#include "matrix4.h"

using namespace Maths;

Scene::~Scene()
{
	// Destroy all entity in the scene. The root is never destroyed, but all its children can be.
	for (std::unique_ptr<Entity>& child : root.children)
		child->Destroy();
}

void Scene::StartScene()
{
	// Ensure the object needs to call the Start() method.
	if (state != EntityState::UNINITIALIZED)
		return;

	// Make each child entity start their own components.
	root.StartComponents();

	// Change state of the entity.
	state = EntityState::INITIALIZED;
}

void Scene::UpdateScene(float _deltaTime)
{
	// Ensure the object has already called the StartComponents() method.
	if (state != EntityState::INITIALIZED && state != EntityState::PENDING_DESTROY)
		return;

	// Make each child entity update their own components.
	root.UpdateComponents(_deltaTime);
}

void Scene::LateUpdateScene(float _deltaTime)
{
	// Ensure the object has already called the StartComponents() method.
	if (state != EntityState::INITIALIZED && state != EntityState::PENDING_DESTROY)
		return;

	// Make each child entity (late) update their own components.
	root.LateUpdateComponents(_deltaTime);
}

void Scene::Destroy()
{
	state = EntityState::PENDING_DESTROY;
}

void Scene::GarbageCollect()
{
	// Destroy all entities that are pending to be destroyed. The root is never destroyed, but all its children can be.
	DestroyPendingEntities(root);
}

void Scene::UpdateTransformsFromRoot()
{
	// Update the transforms of the scene starting from the root entity.
	UpdateTransformsFromEntity(root, Matrix4::Identity());
}

void Scene::UpdateFrameCachedComponents()
{
	// Clear the cache and fill all necessary caches for the current frame.
	meshRenderersFrameCache.clear();
	std::vector<Entity*> entitiesToVisit;
	entitiesToVisit.push_back(&root);
	while (!entitiesToVisit.empty())
	{
		// Take current entity to visit.
		Entity* currentEntity = entitiesToVisit.back();
		entitiesToVisit.pop_back();

		// Mesh Renderer component cache.
		if (MeshRendererComponent* meshRenderer = currentEntity->GetComponent<MeshRendererComponent>())
			meshRenderersFrameCache.push_back(meshRenderer);

		// ... More caches will probably be added here in the future.

		// Add children to visit.
		for (std::unique_ptr<Entity>& child : currentEntity->children)
			entitiesToVisit.push_back(child.get());
	}
}

void Scene::ReloadSceneScripts()
{
	// Hot reload the scripts of the scene starting from the root entity.
	ReloadSceneScriptsFromEntity(root);
}

void Scene::DestroyPendingEntities(Entity& _currentObject)
{
	// For the current entity, check if any of its children are pending to be destroyed. If so, destroy them and remove them from the children vector.
	for (int i = 0; i < _currentObject.children.size(); i++)
	{
		if (_currentObject.children[i]->state == EntityState::PENDING_DESTROY)
		{
			_currentObject.children.erase(_currentObject.children.begin() + i);
			i--;
		}
		else
			DestroyPendingEntities(*_currentObject.children[i].get());
	}
}

void Scene::UpdateTransformsFromEntity(Entity& _currentObject, const Matrix4& _parentWorldMat)
{
	// Update current entity world transform.
	_currentObject.transform.UpdateWorldTransform(_parentWorldMat);

	// Update each child entity.
	for (std::unique_ptr<Entity>& child : _currentObject.children)
		UpdateTransformsFromEntity(*child.get(), _currentObject.transform.GetWorldSpaceMatrix());
}

void Scene::ReloadSceneScriptsFromEntity(Entity& _currentObject)
{
	// Hot reload current entity script component if it has one.
	if (ScriptComponent* scriptComp = _currentObject.GetComponent<ScriptComponent>())
		scriptComp->ReloadScript();

	// Hot reload each child entity.
	for (std::unique_ptr<Entity>& child : _currentObject.children)
		ReloadSceneScriptsFromEntity(*child.get());
}
	