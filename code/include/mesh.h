#pragma once

#include "vector3.h"
#include <vector>
#include <map>

struct Vertex
{
	Vertex(Maths::Vector3 _position = Maths::Vector3::zero, Maths::Vector3 _normal = Maths::Vector3::zero, Maths::Vector3 _color = Maths::Vector3(1.f, 1.f, 1.f));
	Vertex(const Vertex& _copy);
	~Vertex() = default;
	Maths::Vector3 position;
	Maths::Vector3 normal;
	Maths::Vector3 color;;
};

class Mesh
{
public:
	Mesh(std::string _name, std::string _filePath);
	Mesh();
	Mesh(const Mesh& _copy);
	~Mesh();


	/// GETTERS ///

	const uint32_t GetVertexCount() const { return vertexCount; }
	const uint32_t GetIndexCount() const { return indexCount; }
	const std::string GetName() const { return name; }
	const std::vector<uint32_t>& GetIndices() const { return indices; };
	const Vertex* GetVertices() const { return vertices.data(); }
private:
	/// MESH PROPERTIES ///

	std::vector<uint32_t> indices;
	std::vector<Vertex> vertices;
	std::string name;
	uint32_t vertexCount;
	uint32_t indexCount;


	/// CLASS HELPERS /// 

	void CreateTriangleMesh();
	// Author: Merwan ASSEMAT
	bool Load(std::string _filePath);
	// Author: Merwan ASSEMAT
	void AddVertex(const std::string& _face, const std::vector<Maths::Vector3>& _positions, const std::vector<Maths::Vector3>& _normals, std::map<std::string, int>& _mapIds);
};
