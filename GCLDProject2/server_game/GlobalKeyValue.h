#ifndef GlobalKeyValue_h__
#define GlobalKeyValue_h__

#include "def/TypeDef.h"
#include "boost/scoped_ptr.hpp"
class JsonTool;

class GlobalKeyValue
{
public:
    GlobalKeyValue();
    void LoadFromCURLRespon( const std::string& resp_str); 

    bool TryGetValueString( const std::string& key, std::string& strVal) const;
    bool TryGetValueInt( const std::string& key, int& val) const;

private:
    boost::scoped_ptr<JsonTool> m_json_tool;
};

#endif // GlobalKeyValue_h__