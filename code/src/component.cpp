#include "component.h"
#include "entity.h"

Entity& IComponent::GetOwner() const
{
	return *owner;
}

Transform& IComponent::GetTransform() const
{
	return owner->GetTransform();
}
