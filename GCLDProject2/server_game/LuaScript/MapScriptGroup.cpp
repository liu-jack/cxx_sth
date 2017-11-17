#include "MapScriptGroup.h"
#include "def/MmoAssert.h"
#include "Logger.h"

//==========================================================================
// static members
//==========================================================================

MapScriptGroup::FunctionMap& MapScriptGroup::GetFunctionList()
{
	static FunctionMap	s_functions;
	return s_functions;
}

bool MapScriptGroup::AddFunction( const std::string& name, LuaCFuncProto func )
{
	FunctionMap& functions = GetFunctionList();
	functions[name] = func;

	return true;
}

//==========================================================================
MapScriptGroup::MapScriptGroup(void)
{
}

MapScriptGroup::~MapScriptGroup(void)
{
}

LuaScript* MapScriptGroup::Create(const std::string& scriptName, bool showErr)
{
	NameScriptMap::iterator it = m_scripts.find(scriptName);
	if (it != m_scripts.end())
	{
		return it->second;
	}

	std::string file = std::string("script/") +scriptName +".lua";

	std::auto_ptr<LuaScript> pScript(new LuaScript(file));
	if (pScript->Load(showErr))
	{
		RegisteFunctions(pScript.get());
		m_scripts[scriptName] = pScript.get();

		return pScript.release();
	}

	return NULL;
}

void MapScriptGroup::RegisteFunctions(LuaScript* pScript)
{
	FunctionMap& functions = GetFunctionList();
	for(FunctionMap::const_iterator it = functions.begin(); it != functions.end(); ++it)
	{
		pScript->RegisteFunc(it->first.c_str(), it->second);
	}
}

void MapScriptGroup::ReloadAll()
{
	for(NameScriptMap::iterator it = m_scripts.begin(); it != m_scripts.end(); ++it)
	{
		(*it).second->Reload(true);
		RegisteFunctions((*it).second);
	}
	DLOG("Reload all script of current map OK");
}
