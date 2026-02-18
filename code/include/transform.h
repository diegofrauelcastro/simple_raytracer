#pragma once

#include "vector3.h"
#include "matrix4.h"

class Transform
{
public:
	// CONSTRUCTOR & DESTRUCTOR

	Transform();
	Transform(const Maths::Vector3& _position);
	Transform(const Maths::Vector3& _position, const Maths::Vector3& _euleurAngles, const Maths::Vector3& _scale);
	Transform(const Transform& _copy);
	~Transform() = default;

	Transform& operator=(const Transform& copy);


	// PUBLIC METHODS

	void UpdateWorldTransform(const Maths::Matrix4& _parentWorldSpaceMat);
	Maths::Matrix4 GetLocalSpaceMatrix() const;


	// SETTERS

	void SetPosition(const Maths::Vector3& _newPos)				{ position = _newPos; shouldUpdateWorldMatrix = true; }
	void SetPosition(float _x, float _y, float _z)				{ position = Maths::Vector3(_x, _y, _z); shouldUpdateWorldMatrix = true; }
	void SetRotation(const Maths::Vector3& _newEulerAngles)		{ rotation = Maths::Vector3(_newEulerAngles); shouldUpdateWorldMatrix = true; shouldUpdateForwardUpRight = true; }
	void SetRotation(float _x, float _y, float _z)				{ rotation = Maths::Vector3(_x, _y, _z); shouldUpdateWorldMatrix = true; shouldUpdateForwardUpRight = true; }
	void SetScale(const Maths::Vector3& _newScale)				{ scale = _newScale; shouldUpdateWorldMatrix = true; }
	void SetScale(float _x, float _y, float _z)					{ scale = Maths::Vector3(_x, _y, _z); shouldUpdateWorldMatrix = true; }


	// GETTERS

	const Maths::Vector3& GetPosition()	const			{ return position; }
	const Maths::Vector3& GetRotation()		const		{ return rotation; }
	const Maths::Vector3& GetScale()	const			{ return scale; }
	const Maths::Matrix4& GetWorldSpaceMatrix() const	{ return worldSpaceMatrix; }
	const Maths::Vector3& GetForward()					{ if (shouldUpdateForwardUpRight) UpdateForwardUpRight(); return forward; }
	const Maths::Vector3& GetUp()						{ if (shouldUpdateForwardUpRight) UpdateForwardUpRight(); return up; }
	const Maths::Vector3& GetRight()					{ if (shouldUpdateForwardUpRight) UpdateForwardUpRight(); return right; }

private:
	// TRANSFORM PROPERTIES

	Maths::Vector3 position	= Maths::Vector3(0.f, 0.f, 0.f);
	Maths::Vector3 rotation	= Maths::Vector3(0.f, 0.f, 0.f);
	Maths::Vector3 scale	= Maths::Vector3(0.f, 0.f, 0.f);

	Maths::Vector3 forward	= Maths::Vector3(0.f, 0.f, 1.f);
	Maths::Vector3 up		= Maths::Vector3(0.f, 1.f, 0.f);
	Maths::Vector3 right	= Maths::Vector3(1.f, 0.f, 0.f);

	Maths::Matrix4 worldSpaceMatrix = Maths::Matrix4::Identity();
	bool shouldUpdateWorldMatrix	= true;
	bool shouldUpdateForwardUpRight = true;

	
	// HELPERS

	void UpdateForwardUpRight();
};