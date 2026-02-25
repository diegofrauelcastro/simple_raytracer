#pragma once

#include "entity.h"
#include "mesh.h"
#include "mesh_renderer_component.h"
#include "light_component.h"
#include <vector>
#include <string>

// Forward declarations

class Scene
{
private:
	// SCENE PROPERTIES

	std::string name;
	Entity root;
	EntityState state = EntityState::UNINITIALIZED;


	// CACHE

	std::vector<MeshRendererComponent*> meshRenderersFrameCache;
	std::vector<LightComponent*> lightComponentsFrameCache;


	// HELPERS

	void DestroyPendingEntities(Entity& _currentObject);
	void UpdateTransformsFromEntity(Entity& _currentObject, const Maths::Matrix4& _parentWorldMat);
	void ReloadSceneScriptsFromEntity(Entity& _currentObject);

public:
	// CONSTRUCTOR & DESTRUCTOR

	Scene(const std::string& _name) : name(_name) {}
	Scene(const Scene& _copy) = delete;
	~Scene();

	Scene& operator=(const Scene& _copy) = delete;


	// PUBLIC METHODS

	void StartScene();
	void UpdateScene(float _deltaTime);
	void LateUpdateScene(float _deltaTime);
	void Destroy();
	void GarbageCollect();
	void UpdateTransformsFromRoot();
	void UpdateFrameCachedComponents();
	void ReloadSceneScripts();


	// GETTERS

	Entity& GetRoot() { return root; }
	EntityState GetState() const { return state; }
	std::string GetName() const { return name; }
	const std::vector<MeshRendererComponent*>& GetMeshRenderersFrameCache() const { return meshRenderersFrameCache; }
	const std::vector<LightComponent*>& GetLightComponentsFrameCache() const { return lightComponentsFrameCache; }
};