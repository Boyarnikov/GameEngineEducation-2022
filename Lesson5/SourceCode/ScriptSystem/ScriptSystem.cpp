#include "ScriptProxy.h"
#include "ScriptSystem.h"


CScriptSystem::CScriptSystem()
{
    return;
}


void CScriptSystem::ProcessScript(IScriptProxy* scriptProxy)
{
    return;
}


void CScriptSystem::Update()
{
    return;
}


IScriptProxy* CScriptSystem::CreateProxy(const char* filename)
{
    return new CScriptProxy(filename);
}