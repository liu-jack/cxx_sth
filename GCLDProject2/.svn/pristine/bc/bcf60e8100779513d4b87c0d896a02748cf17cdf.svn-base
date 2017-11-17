#include "GlobalKeyValue.h"

#include "json/JsonTool.h"



GlobalKeyValue::GlobalKeyValue()
: m_json_tool( new JsonTool())
{

}

void GlobalKeyValue::LoadFromCURLRespon( const std::string& resp_str )
{
    m_json_tool->LoadFromStr( resp_str);
}

bool GlobalKeyValue::TryGetValueString( const std::string& key, std::string& strVal ) const
{
    m_json_tool->GetString( key, strVal );
    return true;
}

bool GlobalKeyValue::TryGetValueInt( const std::string& key, int& val ) const
{
    val = m_json_tool->GetInt(key);
    return true;
}

