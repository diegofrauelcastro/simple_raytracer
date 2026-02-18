#include "entity.h"

void Entity::Destroy()
{
	state = EntityState::PENDING_DESTROY;
}

Entity::~Entity()
{
	// Destroy all components.
	for (auto it = components.begin(); it != components.end(); it++)
	{
		IComponent* comp = it->second.get();
		if (comp)
			comp->OnDestroy();
	}
}

Entity& Entity::AddChild(const std::string& _childName)
{
	std::unique_ptr<Entity> newbornChild = std::make_unique<Entity>(*this, _childName);
	Entity* rawChildP = newbornChild.get();
	children.emplace_back(std::move(newbornChild));
	return *rawChildP; 
}

void Entity::StartComponents()
{
	// Ensure the object needs to call the Start() method.
	if (state != EntityState::UNINITIALIZED && state != EntityState::PENDING_DESTROY)
		return;

	// Call the Start() method of each component in this gameobject.
	for (auto& [type, comp] : components)
		comp->Start();
	// Make each child gameobject also start their own components.
	for (std::unique_ptr<Entity>& child : children)
		child->StartComponents();
	// Change state of the gameobject.
	state = EntityState::INITIALIZED;
}

void Entity::UpdateComponents(float _deltaTime)
{
	// Ensure the object has already called the StartComponents() method.
	if (state != EntityState::INITIALIZED && state != EntityState::PENDING_DESTROY)
		return;

	// Call the Update() method of each component in this gameobject.
	for (auto& [type, comp] : components)
		comp->Update(_deltaTime);
	// Make each child gameobject also update their own components.
	for (std::unique_ptr<Entity>& child : children)
		child->UpdateComponents(_deltaTime);
}

void Entity::LateUpdateComponents(float _deltaTime)
{
	// Ensure the object has already called the StartComponents() method.
	if (state != EntityState::INITIALIZED && state != EntityState::PENDING_DESTROY)
		return;

	// Call the LateUpdate() method of each component in this gameobject.
	for (auto& [type, comp] : components)
		comp->LateUpdate(_deltaTime);
	// Make each child gameobject also (late) update their own components.
	for (std::unique_ptr<Entity>& child : children)
		child->LateUpdateComponents(_deltaTime);
}
