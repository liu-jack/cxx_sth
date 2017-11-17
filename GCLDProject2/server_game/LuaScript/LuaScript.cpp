#include "LuaScript.h"
#include "Logger.h"
#include "def/MmoAssert.h"

//=================================================================

LuaScript::LuaScript(std::string const &file): m_LuaState(NULL), m_file(file)
{
	m_LuaState = lua_open();
	MMO_ASSERT(m_LuaState);
	luaL_openlibs(m_LuaState); 
}

LuaScript::~LuaScript(void)
{
	lua_close(m_LuaState);
	m_LuaState = NULL;
}

bool LuaScript::PushFunction(const char* funcName)
{
	lua_getglobal(m_LuaState, funcName);

	if(lua_isnil(m_LuaState,-1))
	{
		lua_pop(m_LuaState, 1); 
		return false;
	}

	return true;
}

bool LuaScript::CallCurrentStack(const char* fileName, const char* funcName, int args, int top)
{
	bool success = (lua_pcall(m_LuaState, args, 0, 0) == 0);

	/*if (!success)
	{
		ELOG("%s => %s(), error: %s", fileName, funcName, lua_tostring(m_LuaState,-1));
	}

	lua_settop(m_LuaState, top);*/
	return success;
}

bool LuaScript::CallCurrentStack(const char* fileName, const char* funcName, int args, int top, bool& result)
{
	bool success = (lua_pcall(m_LuaState, args, 1, 0) == 0);

	//if (success)
	//{
	//	result = (lua_toboolean(m_LuaState, -1) != 0);
	//}
	//else
	//{
	//	ELOG("%s => %s(), error: %s", fileName, funcName, lua_tostring(m_LuaState,-1));
	//}

	//lua_settop(m_LuaState, top);
	return success;
}

bool LuaScript::CallCurrentStack(const char* fileName, const char* funcName, int args, int top, int& result)
{
	bool success = (lua_pcall(m_LuaState, args, 1, 0) == 0);

	/*if (success)
	{
		result = lua_tointeger(m_LuaState, -1);
	}
	else
	{
		ELOG("%s => %s(), error: %s", fileName, funcName, lua_tostring(m_LuaState,-1));
	}

	lua_settop(m_LuaState, top);*/
	return success;
}

bool LuaScript::CallCurrentStack( const char* fileName, const char* funcName, int args, int top, float& result )
{
    bool success = (lua_pcall(m_LuaState, args, 1, 0) == 0);

    //if (success)
    //{
    //    result = static_cast<float>( lua_tonumber(m_LuaState, -1));
    //}
    //else
    //{
    //    ELOG("%s => %s(), error: %s", fileName, funcName, lua_tostring(m_LuaState,-1));
    //}

    //lua_settop(m_LuaState, top);
    return success;
}



//void PrintTable(lua_State *L)
//{
//    lua_pushnil(L);
//
//    while(lua_next(L, -2) != 0)
//    {
//		if (lua_type(L, -1) == LUA_TFUNCTION && (!lua_iscfunction(L, -1)))
//		{
//			printf("%s = %p\n", lua_tostring(L, -2), lua_topointer(L, -1));
//		}
//        lua_pop(L, 1);
//    }
//
//	return;
//}

bool LuaScript::Load(bool showErr)
{
	//if(luaL_dofile(m_LuaState, m_file.c_str()) == 0)
	//{
	//	//lua_getglobal( m_LuaState, "_G" );
	//	//PrintTable(m_LuaState);

	//	return true;
	//}

	//if (showErr)
	//{
	//	ELOG("[LuaScript::Load]: %s, error: %s", m_file.c_str(), lua_tostring (m_LuaState,-1));
	//}
	//
	//lua_pop(m_LuaState, 1);

	return false;
}

bool LuaScript::Reload(bool showErr)
{
	/*if( m_LuaState )
		lua_close(m_LuaState);

	m_LuaState = lua_open();    
	MMO_ASSERT(m_LuaState);
	luaL_openlibs(m_LuaState); 

	if(luaL_dofile(m_LuaState, m_file.c_str()) == 0)
		return true;

	if (showErr)
	{
		ELOG("[LuaScript::Load]: %s, error: %s", m_file.c_str(), lua_tostring (m_LuaState,-1));
	}
	
	lua_pop(m_LuaState, 1);*/
	return false;
}

void LuaScript::RegisteFunc (const char* funcName, LuaCFuncProto func)
{
	//lua_register(m_LuaState, funcName, func); 
}

int LuaScript::GetLuaStackTop() const
{
	return lua_gettop(m_LuaState);	
}

CToLuaStack LuaScript::CreateParamStack() const
{
	return CToLuaStack(m_LuaState);
}