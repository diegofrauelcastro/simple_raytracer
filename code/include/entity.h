#pragma once

#include "vector3.h"
#include "mesh.h"

class Mesh;

struct Transform
{
	Maths::Vector3 position;
	Transform(const Maths::Vector3& _position = Maths::Vector3::zero);
	Transform(const Transform& _copy);
	~Transform() = default;
};

class Entity
{
private:
	/// ENTITY PROPERTIES ///

	std::string name;
	Mesh* mesh = nullptr;

public:
	/// CONSTRUCTOR & DESTRUCTOR ///

	Entity(std::string _name, Mesh* _mesh, const Maths::Vector3& _position = Maths::Vector3::zero);
	Entity(const Entity& _copy);
	~Entity() = default;


	/// TRANSFORM ///

	Transform transform;


	/// GETTERS ///

	const Mesh* GetMesh() const { return mesh; }
};