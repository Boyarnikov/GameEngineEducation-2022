#pragma once

#include <sol/sol.hpp>
#include "IScriptProxy.h"


class SCRIPTSYSTEM_API CScriptProxy final : public IScriptProxy
{
private:
	sol::state lua_script;

public:
	CScriptProxy(const char* filename);

	float Update(bool L, bool R, float offset, float dt);
};