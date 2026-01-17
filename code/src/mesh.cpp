#include "mesh.h"
#include "DebugLog/debug.h"
#include <fstream>

using namespace Maths;

Vertex::Vertex(Vector3 _position, Vector3 _normal, Vector3 _color)
	: position(_position)
	, normal(_normal)
	, color(_color)
{
}

Vertex::Vertex(const Vertex& _copy)
	: position(_copy.position)
	, normal(_copy.normal)
	, color(_copy.color)
{
}

void Mesh::CreateTriangleMesh()
{
	// Hardcoded vertices and indices.
	indices = { 0, 1, 2 };
	Vertex v1({ -0.5f, -0.5f, 0.f }, Vector3::forward);
	Vertex v2({ 0.5f, -0.5f, 0.f }, Vector3::forward);
	Vertex v3({ 0.f, 0.5f, 0.f }, Vector3::forward);
	vertices = { v1, v2, v3 };
	vertexCount = 3; indexCount = 3;
}

// Author: Merwan ASSEMAT
bool Mesh::Load(std::string _filePath)
{
	// Open obj file and read line by line.
	std::ifstream objFile(_filePath);
	std::string line;
	// Prepare to store all positions, normals and keep track of them in a map.
	std::vector<Vector3> positions;
	std::vector<Vector3> normals;
	std::map<std::string, int> mapIds;
	// Interpret every line depending on the prefix specifier.
	if (objFile.is_open())
	{
		std::string s;
		while (objFile >> s)
		{
			// Position.
			if (s == "v")
			{
				float x;
				float y;
				float z;
				objFile >> x >> y >> z;
				Vector3 pos = Vector3(x, y, z);
				positions.push_back(pos);
			}
			// Normal.
			if (s == "vn")
			{
				float x;
				float y;
				float z;
				objFile >> x >> y >> z;
				Vector3 normal = Vector3(x, y, z);
				normals.push_back(normal);
			}
			if (s == "f")
			{
				std::string v1;
				std::string v2;
				std::string v3;

				objFile >> v1;
				AddVertex(v1, positions, normals, mapIds);


				objFile >> v2;
				AddVertex(v2, positions, normals, mapIds);


				objFile >> v3;
				AddVertex(v3, positions, normals, mapIds);
			}
		}
		// Update counters.
		indexCount = indices.size();
		vertexCount = vertices.size();
		// Close file and clear vectors.
		objFile.close();
		positions.clear(); positions.shrink_to_fit();
		normals.clear(); normals.shrink_to_fit();
		mapIds.clear();
		// Return the object as successfully read.
		return true;
	}
	// Return to signal error.
	return false;
}

// Author: Merwan ASSEMAT
void Mesh::AddVertex(const std::string& _face, const std::vector<Vector3>& _positions, const std::vector<Vector3>& _normals, std::map<std::string, int>& _mapIds)
{
	if (_mapIds.find(_face) == _mapIds.end())
	{
		std::vector<std::string> subStrings;
		std::string currentSubString;


		for (char c : _face)
		{
			if (c == '/')
			{
				subStrings.push_back(currentSubString);
				currentSubString.clear();
			}
			else
				currentSubString += c;
		}
		subStrings.push_back(currentSubString);


		if (subStrings.size() != 3)
		{
			LOG_APP("Invalid face format: %s", _face.c_str());
			return;
		}


		int posId = std::stoi(subStrings[0]) - 1;
		int normalId = std::stoi(subStrings[2]) - 1;


		if (posId < 0 || posId >= _positions.size() || normalId < 0 || normalId >= _normals.size())
		{
			LOG_APP("Invalid index in face: %s", _face.c_str());
			return;
		}


		Vertex newVertex(_positions[posId], _normals[normalId]);
		vertices.push_back(newVertex);
		int id = vertices.size() - 1;
		indices.push_back(id);
		_mapIds[_face] = id;
	}
	// Just re-use old id
	else
		indices.push_back(_mapIds[_face]);
}

Mesh::Mesh(std::string _name, std::string _filePath)
	: name(_name)
{
	// Try to load the mesh file (.obj) at the specified path.
	bool loaded = Load(_filePath);
	if (!loaded)
		LOG_APP("Couldn't load mesh %s", name)
	else
		LOG_APP("Successfully loaded mesh %s", name);
}

Mesh::Mesh()
	: name("TriangleMesh")
{
	// Create a triangle by default.
	CreateTriangleMesh();
	LOG_APP("Successfully loaded a triangle.");
}

Mesh::Mesh(const Mesh& _copy)
	: name(_copy.name + " (Copy)")
	, vertexCount(_copy.vertexCount)
	, indexCount(_copy.indexCount)
	, vertices(_copy.vertices)
	, indices(_copy.indices)
{
}

Mesh::~Mesh()
{
	// Clear vertices and indices.
	indices.clear(); indices.shrink_to_fit();
	vertices.clear(); vertices.shrink_to_fit();
	LOG_APP("Successfully destroyed mesh %s", name);
}
