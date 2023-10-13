#pragma once

#include "flecs.h"
#include "RenderEngine.h"
#include "../InputHandler.h"
#include "../../ScriptSystem/ScriptProxy.h"

class EntitySystem
{
public:
	EntitySystem() = delete;
	EntitySystem(RenderEngine* renderEngine, InputHandler* inputHandler, IScriptProxy* scriptsProxy);

	void Update();
private:
	flecs::world ecs;
};

