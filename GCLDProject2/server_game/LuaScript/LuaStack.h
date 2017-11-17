#pragma once

#include <set>
#include <vector>
#include "def/TypeDef.h"

extern "C"
{ 
	#include "lua.h" 
	#include "lualib.h" 
	#include "lauxlib.h" 
} 

struct IScriptHost;

#define READARR_HEADER(name) \
        if (_inputIdx > ParamCount()) \
        {\
            _inputIdx ++;\
            return;\
        }\
        if (!CurrentParamIs(LUA_TTABLE, name))\
            return;\
        lua_pushnil(_L)


class LuaToCStack
{
public:
	LuaToCStack(lua_State*	L)
		:_L(L)
		,_inputIdx(1)
	{
	}

	// Stack -> C
	int ParamCount()
	{
		return lua_gettop(_L);
	}

	void ReadIntSet(std::set<int>& datas);

    void ReadFloatVec(std::vector<float>& datas);
    // unit64: coz 5.1 not support, use vector with 2 uint32 instead of custom uint64
	void ReadGuidsSet(std::set<ObjGUID>& guids) 
	{
        READARR_HEADER("ReadGuidsSet");
		std::vector<uint32> datas;
        uint32 guidArr[2];
		while(lua_next(_L, _inputIdx) != 0)
		{
			//printf("%s - %s\n", lua_typename(_L, lua_type(_L, -2)), lua_typename(_L, lua_type(_L, -1)));
            for(int ltIdx=1; ltIdx < 3; ltIdx++)
            {
                lua_pushnumber(_L, ltIdx);
                lua_gettable(_L, -2);
                guidArr[ltIdx-1] = static_cast<uint32>(lua_tonumber(_L, -1));
                lua_pop(_L, 1); 
            }
            guids.insert(Guid(guidArr[0], guidArr[1]));
            lua_pop(_L, 1); // pop value t
		}
        _inputIdx ++;
	}

	ObjGUID ReadGuidVec()
	{
		std::vector<uint32> datas;
		ReadIntegerVec(datas,true);
        if(datas.size() < 1)
            return static_cast<ObjGUID>(0);

		return Guid(datas[0], datas[1]);
	}
    template <typename T>
    void ReadIntegerVec(std::vector<T>& datas, bool IsUint=false) //datas is vector
    {
        READARR_HEADER("ReadIntegerVec"); //lua_pushnil(_L); // nil => first key

        while(lua_next(_L, _inputIdx) != 0) 
        {
            if(IsUint)
                datas.push_back(static_cast<uint32>(lua_tonumber(_L, -1))); // pick key //diff from set
            else
                datas.push_back(lua_tointeger(_L, -1));
            lua_pop(_L, 1); //pop value, key is at stack top now
        }

        _inputIdx ++;
    }
	IScriptHost* ReadHost();
	IScriptHost* ReadHostSafely();
	IScriptHost* ReadOptHost();
	const char* ReadStr();

	bool ReadBool(bool defaultVal)
	{
		if (_inputIdx > ParamCount())
		{
			_inputIdx ++;
			return defaultVal;
		}

		return (lua_toboolean(_L, _inputIdx ++) != 0);
	}

	int ReadInt(int defaultVal)
	{
		if (_inputIdx > ParamCount())
		{
			_inputIdx ++;
			return defaultVal;
		}

		return lua_tointeger(_L, _inputIdx ++);
	}

	float ReadFloat(float defaultVal)
	{
		if (_inputIdx > ParamCount())
		{
			_inputIdx ++;
			return defaultVal;
		}

		return (float)lua_tonumber(_L, _inputIdx ++);
	}

	ObjGUID ReadGuid(ObjGUID defaultVal)//abort it to use ReadGuidVec,lua5.1 dont support int64 and has no custom int64
	{
		if (_inputIdx > ParamCount())
		{
			_inputIdx ++;
			return defaultVal;
		}

		//return (ObjGUID)lua_tonumber(_L, _inputIdx ++);
		uint64 retVal =static_cast<uint64>( lua_tonumber(_L, _inputIdx ++) );
		return retVal ;
	}

private:
	bool CurrentParamIs( int type, const char* name );

private:
	lua_State*	_L;
	int			_inputIdx;
};

class CToLuaStack
{
public:
	CToLuaStack(lua_State*	L)
		:_L(L)
		,_outputIdx(0)
	{
	}

	// C -> Stack;
	int ParamCount()
	{
		return _outputIdx;
	}

	CToLuaStack& operator << (IScriptHost* pHost)
	{
		lua_pushlightuserdata(_L, pHost);
		_outputIdx ++;
		return *this;
	}

	CToLuaStack& operator << (int n)
	{
		lua_pushinteger(_L, n);
		_outputIdx ++;
		return *this;
	}

	CToLuaStack& operator << (float n)
	{
		lua_pushnumber(_L, n);
		_outputIdx ++;
		return *this;
	}


	CToLuaStack& operator << (bool b)
	{
		lua_pushboolean(_L, (int)b);
		_outputIdx ++;
		return *this;
	}

	CToLuaStack& operator << (const std::vector<IScriptHost*>& hosts);

	CToLuaStack& operator << (const std::vector<uint32>& arrs);
	CToLuaStack& operator << (const std::vector<int>& arrs);
    CToLuaStack& operator << (const std::vector<float>& arrs);

	CToLuaStack& operator << (const char* s)
	{
		lua_pushstring(_L, s);
		_outputIdx ++;
		return *this;
	}

private:
	lua_State*	_L;
	int			_outputIdx;		
};