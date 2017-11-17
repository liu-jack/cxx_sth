#include "FormulaScriptHolder.h"
#include "IScriptOwner.h"
#include "LuaScript.h"
#include "def/MmoAssert.h"

FormulaScriptHolder::FormulaScriptHolder( const std::string& name , IScriptOwner* scriptOwner )
: m_name ( name)
, m_scriptOwner( scriptOwner)
{
}
void FormulaScriptHolder::Init()
{
    Load( m_name , ILuaScriptHolder::ShowError);
}

FormulaScriptHolder::~FormulaScriptHolder()
{

}



bool FormulaScriptHolder::Call(const char* funcName, const std::vector<float>& params , float& result )
{
    if ( m_script == NULL)
    {
        return false;
    }
    int top = m_script->GetLuaStackTop();
    if (!m_script->PushFunction(funcName))
    {
        return false;
    }
    CToLuaStack out = m_script->CreateParamStack();

    for ( std::vector<float>::const_iterator it = params.begin(); it != params.end(); ++it)
    {
        out << *it;
    }
    bool success = m_script->CallCurrentStack(m_name.c_str(), funcName, out.ParamCount(), top, result);
    MASSERT( success , "Call formula error" );
    return success;
}

void FormulaScriptHolder::Load( const std::string& scriptName ,LoadOptions option)
{
    if (scriptName.empty() || scriptName == "NONE" || m_scriptOwner == NULL)
    {
        return;
    }
    m_name = scriptName;
    m_script = m_scriptOwner->Create(m_name, option == ILuaScriptHolder::ShowError);

}
