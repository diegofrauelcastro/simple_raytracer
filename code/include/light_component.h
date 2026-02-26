#pragma once

#include "color.h"
#include "component.h"

class LightComponent : public IComponent
{
private:
	// LIGHT PROPERTIES

	Color color = Color::white;
	float intensity = 1.f;

public:
	// CONSTRUCTOR & DESTRUCTOR
	LightComponent() = delete;
	LightComponent(Entity& _owner, const std::string& _name) : IComponent(_owner, _name) {}
	LightComponent(const LightComponent& _copy) : IComponent(*_copy.owner, _copy.name), color(_copy.color), intensity(_copy.intensity) {}
	~LightComponent() override = default;


	// OVERRIDABLE METHODS

	virtual void Start() override {};
	virtual void Update(float _deltaTime) override {};
	virtual void LateUpdate(float _deltaTime) override {};
	virtual void OnDestroy() override {};


	// GETTERS

	Color GetColor() const { return color; }
	float GetIntensity() const { return intensity; }


	// SETTERS

	void SetColor(const Color& _color) { color = _color; }
	void SetIntensity(float _intensity) { intensity = _intensity; }
};