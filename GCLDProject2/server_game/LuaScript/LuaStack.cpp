#include <string>
#include <boost/thread.hpp>
#include "LuaStack.h"
#include "DummyScriptHost.h"
#include "Logger.h"

extern boost::thread_specific_ptr<std::string>	g_CurScriptFileName;
extern boost::thread_specific_ptr<char>			g_CurScriptLuaFuncName;
extern boost::thread_specific_ptr<char>			g_CurScriptCFuncName;

bool LuaToCStack::CurrentParamIs( int type, const char* name )
{
	if (lua_type(_L, _inputIdx) != type)
	{
		ELOG ("%s.lua => %s() : invalid param in C Func %s() => %s()", 
			g_CurScriptFileName->c_str(), 
			g_CurScriptLuaFuncName.get(), 
			g_CurScriptCFuncName.get(),
			name
		);
		return false;
	}
	return true;
}

IScriptHost* LuaToCStack::ReadHost()
{
	if (!CurrentParamIs(LUA_TLIGHTUSERDATA, "ReadHost"))
	{
		return &DummyScriptHost::Instance();
	}

	IScriptHost* pHost = (IScriptHost*)lua_touserdata(_L, _inputIdx ++);
	if (pHost != NULL && pHost->IsValid())
	{
		return pHost;
	}

	return NULL;
}

IScriptHost* LuaToCStack::ReadHostSafely()
{
	IScriptHost* pHost = ReadHost ();

	if (pHost == NULL)
	{
		ELOG ("%s.lua => %s() : C Func %s() => ReadHost() returns NULL", 
			g_CurScriptFileName->c_str(), 
			g_CurScriptLuaFuncName.get(),
			g_CurScriptCFuncName.get()
			);
		return &DummyScriptHost::Instance();
	}

	return pHost;
}

IScriptHost* LuaToCStack::ReadOptHost()
{
	if (_inputIdx > ParamCount())
	{
		_inputIdx ++;
		return NULL;
	}

	return ReadHost();
}

void LuaToCStack::ReadIntSet(std::set<int>& datas)
{
    READARR_HEADER("ReadIntSet"); //lua_pushnil(_L); // nil => first key

    while(lua_next(_L, _inputIdx) != 0) 
    {
		//datas.insert(lua_tointeger(_L, -2)); // pick key
		datas.insert(lua_tointeger(_L, -1)); // pick value
        lua_pop(_L, 1); //pop value, key is at stack top now
    }

	_inputIdx ++;
}

void LuaToCStack::ReadFloatVec(std::vector<float>& datas)
{
    READARR_HEADER("ReadFloatVec");
    while(lua_next(_L, _inputIdx) != 0) 
    {
		//datas.insert(lua_tointeger(_L, -2)); // pick key
        datas.push_back(static_cast<float>(lua_tonumber(_L, -1)));    // pick value
        lua_pop(_L, 1); //pop value, key is at stack top now
    }
	_inputIdx ++;
}

const char* LuaToCStack::ReadStr()
{
	if (!CurrentParamIs(LUA_TSTRING, "ReadStr"))
	{
		return "";
	}

	const char* str = lua_tostring(_L, _inputIdx ++);
	return str;
}

//==================================================================
// CToLuaStack
//==================================================================

CToLuaStack& CToLuaStack::operator << (const std::vector<IScriptHost*>& hosts)
{
	lua_newtable(_L);

	for (size_t i=0; i<hosts.size(); ++i)
	{
		lua_pushinteger(_L, i+1);
		lua_pushlightuserdata(_L, hosts[i]);
		lua_settable(_L, -3);
	}

	_outputIdx ++;
	return *this;
}

#define WRITE_VEC_NUM lua_newtable(_L);\
	for (size_t i=0; i<vecs.size(); ++i)\
	{\
		lua_pushnumber(_L, i+1);\
		lua_pushnumber(_L, vecs[i]);\
		lua_settable(_L, -3);\
	}\
	_outputIdx ++;\
	return *this

CToLuaStack& CToLuaStack::operator << (const std::vector<uint32>& vecs)
{
    WRITE_VEC_NUM;
}

CToLuaStack& CToLuaStack::operator << (const std::vector<int>& vecs)
{
    WRITE_VEC_NUM;
}

CToLuaStack& CToLuaStack::operator<<( const std::vector<float>& vecs )
{
    WRITE_VEC_NUM;
}
