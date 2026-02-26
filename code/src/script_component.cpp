#include "script_component.h"
#include "entity.h"
#include "DebugLog/debug.h"


ScriptComponent::ScriptComponent(Entity& _owner, const std::string& _name)
	: IComponent(_owner, _name)
	, scriptFilePath("./Assets/Scripts/" + _name + ".py")
	, isScriptValid(true)
{
}

void ScriptComponent::ReloadScript()
{
	Start();
}

void ScriptComponent::Start()
{
	if (!isScriptValid)
		return;
}

void ScriptComponent::Update(float _deltaTime)
{
	if (!isScriptValid)
		return;
}

void ScriptComponent::LateUpdate(float _deltaTime)
{
	if (!isScriptValid)
		return;
}

void ScriptComponent::OnDestroy()
{
	if (!isScriptValid)
		return;
}
