#pragma once

#include <string>
#include <iostream>

// Forward declarations.
class Entity;
class Transform;

class IComponent
{
protected:
	// COMPONENT PROPERTIES

	std::string name;
	Entity* owner;

public:
	// CONSTRUCTOR & DESTRUCTOR

	IComponent() = delete;
	IComponent(Entity& _owner, const std::string& _name) : name(_name), owner(&_owner) {};
	IComponent(const IComponent& _copy) = delete;
	virtual ~IComponent() = default;

	IComponent& operator=(const IComponent& _copy) = delete;


	// OVERRIDABLE METHODS

	virtual void Start()						= 0;
	virtual void Update(float _deltaTime)		= 0;
	virtual void LateUpdate(float _deltaTime)	= 0;
	virtual void OnDestroy()					= 0;


	// GETTERS

	const std::string GetName() const { return name; }
	Entity& GetOwner() const;
	Transform& GetTransform() const;
};