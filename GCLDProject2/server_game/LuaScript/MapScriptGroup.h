#pragma once

#include <string>
#include <vector>
#include "SimpleSingleton.h"
#include "ptr_container/PtrMap.h"
#include "LuaScript.h"
#include "IScriptOwner.h"

class MapScriptGroup : public IScriptOwner 
{
	typedef PtrMap<std::string, LuaScript>			NameScriptMap;
	typedef std::map<std::string, LuaCFuncProto>	FunctionMap;

public:
	static bool AddFunction(const std::string& name, LuaCFuncProto func);

	MapScriptGroup(void);
	~MapScriptGroup(void);

	virtual LuaScript* Create(const std::string& scriptName, bool showErr);
	void ReloadAll();

private:
	static FunctionMap& GetFunctionList();
	void RegisteFunctions(LuaScript* pScript);

private:
	NameScriptMap		m_scripts;
};
