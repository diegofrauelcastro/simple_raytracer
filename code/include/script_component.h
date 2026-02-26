#pragma once

#include "component.h"

class ScriptComponent : public IComponent
{
protected:
	// SCRIPT PROPERTIES

	std::string scriptFilePath;
	bool isScriptValid = false;

public:
	// CONSTRUCTOR & DESTRUCTOR

	ScriptComponent(Entity& _owner, const std::string& _name);
	virtual ~ScriptComponent() = default;

	// OVERRIDE METHODS

	virtual void ReloadScript();
	virtual void Start();
	virtual void Update(float _deltaTime);
	virtual void LateUpdate(float _deltaTime);
	virtual void OnDestroy();
};