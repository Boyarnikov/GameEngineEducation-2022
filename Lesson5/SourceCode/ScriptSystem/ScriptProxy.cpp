#include "ScriptProxy.h"

#include <fstream>

CScriptProxy::CScriptProxy(const char* filename)
{
    std::ifstream file(filename);
    std::stringstream program;
    program << file.rdbuf();
    lua_script.script(program.str());
}

float CScriptProxy::Update(bool L, bool R, float offset, float dt)
{
    std::optional<float> new_offset = lua_script["Update"](L, R, offset, dt);

    if (new_offset.has_value())
        return new_offset.value();
    return offset;
}