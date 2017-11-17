#pragma once

#include "ILuaScriptHolder.h"
#include "boost/scoped_ptr.hpp"

class LuaScript;
struct IScriptOwner;

class FormulaScriptHolder : public ILuaScriptHolder
{
public:
    FormulaScriptHolder( const std::string& name, IScriptOwner* scriptOwner );
    ~FormulaScriptHolder();  

    void Init();

    virtual bool Call(const char* funcName, const std::vector<float>& params , float& result);  
private:
    virtual void Load(const std::string& scriptName, LoadOptions option );


private:
    std::string			                m_name;
    IScriptOwner*                       m_scriptOwner;
    LuaScript*                  		m_script;
};