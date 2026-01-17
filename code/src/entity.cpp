#include "entity.h"

using namespace Maths;

Entity::Entity(std::string _name, Mesh* _mesh, const Vector3& _position)
	: mesh(_mesh)
	, name(_name)
{
	transform.position = _position;
}

Entity::Entity(const Entity& _copy)
	: mesh(_copy.mesh)
	, transform(_copy.transform)
	, name(_copy.name)
{
}

Transform::Transform(const Maths::Vector3& _position)
	: position(_position)
{
}

Transform::Transform(const Transform& _copy)
	: position(_copy.position)
{
}
