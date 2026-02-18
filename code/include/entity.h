#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <typeindex>

#include "component.h"
#include "transform.h"
#include "DebugLog/debug.h"

// GameObject state enum.
enum class EntityState
{
	UNINITIALIZED,		// Before "Start()" call.
	INITIALIZED,		// After "Start()" call.
	PENDING_DESTROY		// After "Destroy()" call.
};

class Entity
{
private:
	// ENTITY PROPERTIES

	std::string name;
	Transform transform;
	EntityState state = EntityState::UNINITIALIZED;
	Entity* parent = nullptr;

	std::vector<std::unique_ptr<Entity>> children;
	std::unordered_map<std::type_index, std::unique_ptr<IComponent>> components;


	// HELPERS

	friend class Scene;
	void StartComponents();							// Call "Start()" on all components and also childrens' components.
	void UpdateComponents(float _deltaTime);		//	...	"Update()"		...			...			...			...
	void LateUpdateComponents(float _deltaTime);	//  ...	"LateUpdate()"	...			...			...			...

public:
	// CONSTRUCTOR & DESTRUCTOR

	Entity(Entity& _parentRef, std::string _name = "Entity") : name(_name), parent(&_parentRef) {};
	Entity(std::string _name = "Entity") : name(_name), parent(nullptr) {};
	Entity(const Entity& _copy) = delete;
	~Entity();

	Entity& operator=(const Entity& _copy) = delete;


	// PUBLIC METHODS

	Entity& AddChild(const std::string& _childName);
	void Destroy();

	template<typename T>
	T* AddComponent(const std::string& _componentName)
	{
		// Get the type index for the current type.
		std::type_index type = typeid(T);

		// If the entity already has the component, abort.
		if (components.find(type) != components.end())
		{
			LOG_APP("Warning: Entity %s already has a %s component. Aborting...", name.c_str(), typeid(T).name());
			return nullptr;
		}

		// Create the new component and emplace it into the unordered map.
		std::unique_ptr<T> component = std::make_unique<T>(*this, _componentName);
		T* rawPtr = component.get();
		components.emplace(type, std::move(component));

		return rawPtr;
	}

	template<typename T>
	T* GetComponent()
	{
		// Get the type index for the current type.
		std::type_index type = typeid(T);

		// If the component exists, find it.
		if (components.find(type) != components.end())
			return static_cast<T*>(components[type].get());
		return nullptr;
	}


	// GETTERS

	Transform& GetTransform() { return transform; }
	EntityState GetState() const  { return state; }
	std::string GetName() const { return name; }
};
