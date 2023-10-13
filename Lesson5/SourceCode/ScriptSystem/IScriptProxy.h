#pragma once
#include "Common.h"

class SCRIPTSYSTEM_API IScriptProxy
{
public:
	virtual float Update(bool L, bool R, float offset, float dt) = 0;
};