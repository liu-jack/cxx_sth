#pragma once
#include <string>

class LuaScript;  

struct IScriptOwner
{
	virtual LuaScript* Create(const std::string& scriptName, bool showErr = false) = 0;
    
};