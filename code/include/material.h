#pragma once
#include "color.h"

enum class MaterialType
{
	DIFFUSE,
	TRANSPARENT,
	METALLIC,
};

// Forward declaration of Material class
class MetallicMaterial;
class DiffuseMaterial;
class TransparentMaterial;

class Material
{
public:
	// BASE PROPERTIES

	Color albedo = Color::white;


	// CONSTRUCTOR & DESTRUCTOR

	Material() = default;
	Material(const Material& _copy) = default;
	virtual ~Material() = default;

	Material& operator=(const Material& _copy)
	{
		type = _copy.type;
		return *this;
	}

	// GETTERS

	MaterialType GetType() const { return type; }

	virtual MetallicMaterial* AsMetallic() { return nullptr; }
	virtual DiffuseMaterial* AsDiffuse() { return nullptr; }
	virtual TransparentMaterial* AsTransparent() { return nullptr; }

protected:
	MaterialType type = MaterialType::DIFFUSE;
};

class MetallicMaterial : public Material
{
public:
	// METALLIC PROPERTIES

	float roughness = 0.5f; // [0,1] : 0 = perfect mirror, 1 = very rough.


	// CONSTRUCTOR & DESTRUCTOR

	MetallicMaterial() { type = MaterialType::METALLIC; }
	MetallicMaterial(const MetallicMaterial& _copy) : Material(_copy) {}
	~MetallicMaterial() override = default;


	// GETTERS

	MetallicMaterial* AsMetallic() override { return this; }
};

class DiffuseMaterial : public Material
{
public:
	// CONSTRUCTOR & DESTRUCTOR

	DiffuseMaterial() { type = MaterialType::DIFFUSE; }
	DiffuseMaterial(const DiffuseMaterial& _copy) : Material(_copy) {}
	~DiffuseMaterial() override = default;


	// GETTERS

	DiffuseMaterial* AsDiffuse() override { return this; }

};

class TransparentMaterial : public Material
{
public:
	// TRANSPARENT PROPERTIES

	float ior = 1.5f; // Index of refraction, typically between 1 and 2 for most materials. Air is ~1, glass is ~1.5, diamond is ~2.4.
	float roughness = 0.5f; // [0,1] : 0 = perfect clear, 1 = frosted glass.


	// CONSTRUCTOR & DESTRUCTOR

	TransparentMaterial() { type = MaterialType::TRANSPARENT; }
	TransparentMaterial(const TransparentMaterial& _copy) : Material(_copy) {}
	~TransparentMaterial() override = default;


	// GETTERS

	TransparentMaterial* AsTransparent() override { return this; }
};