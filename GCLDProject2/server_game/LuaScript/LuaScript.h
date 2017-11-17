#pragma once

#include <memory>
#include <vector>
#include "ptr_container/PtrMap.h"
#include "LuaStack.h"


typedef int (*LuaCFuncProto)(lua_State * L);

class LuaScript
{
public:
	LuaScript(std::string const &file);
	~LuaScript(void);

	bool Load(bool showErr);
	bool Reload(bool showErr);
	void RegisteFunc(const char* funcName, LuaCFuncProto func);	

	bool PushFunction(const char* funcName);
	bool CallCurrentStack(const char* fileName, const char* funcName, int args, int top);
	bool CallCurrentStack(const char* fileName, const char* funcName, int args, int top, bool& result);
	bool CallCurrentStack(const char* fileName, const char* funcName, int args, int top, int& result);
    bool CallCurrentStack(const char* fileName, const char* funcName, int args, int top, float& result);
	int GetLuaStackTop() const;
	CToLuaStack CreateParamStack() const;

private:
	lua_State *		m_LuaState;
	std::string		m_file;
};
