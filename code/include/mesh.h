#pragma once

#include "vector3.h"
#include "color.h"
#include <vector>
#include <map>

struct Vertex
{
	Vertex(const Maths::Vector3& _position = Maths::Vector3::zero, const Maths::Vector3& _normal = Maths::Vector3::zero, const Color& _color = Color::white);
	Vertex(const Vertex& _copy);
	~Vertex() = default;
	Maths::Vector3	position;
	Maths::Vector3	normal;
	Color			color;
};

struct AABB
{
	Maths::Vector3 min;
	Maths::Vector3 max;
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
	const AABB& GetBoundingBox() const { return boundingBox; }
private:
	/// MESH PROPERTIES ///

	std::vector<uint32_t> indices;
	std::vector<Vertex> vertices;
	std::string name;
	AABB boundingBox;
	uint32_t vertexCount;
	uint32_t indexCount;


	/// CLASS HELPERS /// 

	void UpdateBoundingBox();
	void CreateTriangleMesh();
	// Author: Merwan ASSEMAT
	bool Load(std::string _filePath);
	// Author: Merwan ASSEMAT
	void AddVertex(const std::string& _face, const std::vector<Maths::Vector3>& _positions, const std::vector<Maths::Vector3>& _normals, std::map<std::string, int>& _mapIds);
};
