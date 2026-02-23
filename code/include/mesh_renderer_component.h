#pragma once

#include "component.h"
#include "matrix4.h"
#include "material.h"

// Forward declarations.
class Mesh;

class MeshRendererComponent : public IComponent
{
private:
	// MESH RENDERER PROPERTIES

	Mesh* mesh = nullptr;
	Material* material = nullptr;

public:
	// CONSTRUCTOR & DESTRUCTOR

	MeshRendererComponent(Entity& _owner, const std::string& _name, Mesh* _mesh = nullptr, Material* _material = nullptr);
	virtual ~MeshRendererComponent() = default;


	// OVERRIDABLE METHODS

	virtual void Start() override;
	virtual void Update(float _deltaTime) override;
	virtual void LateUpdate(float _deltaTime) override;
	virtual void OnDestroy() override;


	// SETTERS METHODS

	void SetMesh(Mesh& _mesh) { mesh = &_mesh; }
	void SetMaterial(Material& _material) { material = &_material; }


	// GETTERS METHODS

	Mesh& GetMesh() const { return *mesh; }
	Material& GetMaterial() const { return *material; }
};