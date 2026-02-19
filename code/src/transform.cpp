#include "transform.h"

using namespace Maths;

Transform::Transform()
	: position(Vector3::zero)
	, rotation(Vector3::zero)
	, scale(Vector3::one)
	, forward(Vector3::forward)
	, up(Vector3::up)
	, right(Vector3::right)
{
}

Transform::Transform(const Vector3& _position)
	: position(_position)
	, rotation(Vector3::zero)
	, scale(Vector3::one)
	, forward(Vector3::forward)
	, up(Vector3::up)
	, right(Vector3::right)
{
}

Transform::Transform(const Vector3& _position, const Vector3& _euleurAngles, const Vector3& _scale)
	: position(_position)
	, rotation(_euleurAngles)
	, scale(_scale)
{
	UpdateForwardUpRight();
}

Transform::Transform(const Transform& _copy)
	: position(_copy.position)
	, rotation(_copy.rotation)
	, scale(_copy.scale)
	, forward(_copy.forward)
	, up(_copy.up)
	, right(_copy.right)
	, objectToWorldSpaceMatrix(_copy.objectToWorldSpaceMatrix)
	, shouldUpdateWorldMatrix(_copy.shouldUpdateWorldMatrix)
	, shouldUpdateForwardUpRight(_copy.shouldUpdateForwardUpRight)
{
}

Transform& Transform::operator=(const Transform& _copy)
{
	position	= _copy.position;
	rotation	= _copy.rotation;
	scale		= _copy.scale;
	forward		= _copy.forward;
	up			= _copy.up;
	right		= _copy.right;
	objectToWorldSpaceMatrix			= _copy.objectToWorldSpaceMatrix;
	shouldUpdateWorldMatrix		= _copy.shouldUpdateWorldMatrix;
	shouldUpdateForwardUpRight	= _copy.shouldUpdateForwardUpRight;
	return *this;
}

Matrix4 Transform::GetLocalSpaceMatrix() const
{
	return Matrix4::CreateTransformMatrix(rotation, position, scale);
}

void Transform::UpdateWorldTransform(const Matrix4& _parentWorldSpaceMat)
{
	if (shouldUpdateWorldMatrix)
	{
		objectToWorldSpaceMatrix = GetLocalSpaceMatrix() * _parentWorldSpaceMat;
		worldSpaceToObjectMatrix = objectToWorldSpaceMatrix.GetInverse();
		shouldUpdateWorldMatrix	 = false;
	}
}

void Transform::UpdateForwardUpRight()
{
	// Update forward, up and right vectors from the current rotation.
	Matrix4 rotMat = Matrix4::CreateTransformMatrix(Vector3::zero, rotation, Vector3::one);
	forward = rotMat * Vector3::forward;
	up		= rotMat * Vector3::up;
	right	= rotMat * Vector3::right;
	shouldUpdateForwardUpRight = false;
}
