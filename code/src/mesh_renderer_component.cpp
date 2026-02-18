#include "mesh_renderer_component.h"
#include "mesh.h"
#include "entity.h"

using namespace Maths;

MeshRendererComponent::MeshRendererComponent(Entity& _owner, const std::string& _name, Mesh* _mesh, Material* _material)
	: IComponent(_owner, _name)
	, mesh(_mesh)
	, material(_material)
{
}

void MeshRendererComponent::Start() {}
void MeshRendererComponent::Update(float _deltaTime) {}
void MeshRendererComponent::LateUpdate(float _deltaTime) {}
void MeshRendererComponent::OnDestroy()
{
	mesh = nullptr;
	material = nullptr;
}
