#pragma once
#include "def/TypeDef.h"

struct IScriptHost;

struct ILuaScriptHolder
{
    enum LoadOptions
    {
        IgnoreError,
        ShowError,
    };

    virtual ~ILuaScriptHolder() {}

    virtual void Load(const std::string& scriptName, LoadOptions option = ShowError ) = 0;

    virtual bool Call(const char* funcName, const std::vector<float>& params , float& result) { return false; } 
    virtual bool Call(const char* funcName, IScriptHost* pHost) { return false; }
    virtual bool Call(const char* funcName, IScriptHost* pHost, int param) { return false; }
    virtual bool Call(const char* funcName, IScriptHost* pHost, int param, int param2) { return false; }
    virtual bool Call(const char* funcName, IScriptHost* pHost, bool& result) { return false; }
    virtual bool Call(const char* funcName, IScriptHost* pHost, IScriptHost* pHost2) { return false; }
    virtual bool Call(const char* funcName, IScriptHost* pHost, IScriptHost* pHost2, int param) { return false; }

    virtual bool Call(const char* funcName, IScriptHost* pHost, IScriptHost* pHost2,const std::vector<IScriptHost*>& hosts,int spellId, bool& result) { return false; }
    virtual bool Call(const char* funcName, IScriptHost* pHost, IScriptHost* pHost2, int spellId, int buffId, int curStack, ObjGUID casterGuid,float specialParam) { return false; }
    //virtual bool Call(const char* funcName, IScriptHost* pHost, IScriptHost* pHost2, const std::vector<IScriptHost*>& hosts, int spellId, float specialParam) =0;
    virtual bool Call(const char* funcName, IScriptHost* pHost, IScriptHost* pHost2, const std::vector<IScriptHost*>& hosts, int runeIndex, float x, float y,float  z, float specialParam,const std::vector<int>& params ) { return false; }
    
};