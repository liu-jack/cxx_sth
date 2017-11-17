#include <boost/property_tree/ini_parser.hpp>
#include <boost/throw_exception.hpp>
#include <boost/tokenizer.hpp>
#include "ConfigMgr.h"
#include "utility/fileUtil.h"

void ConfigMgr::Load(const std::string& file)
{
	boost::property_tree::ini_parser::read_ini(file, m_ptree);
}

bool ConfigMgr::LoadOrCopyFormDefaultFile( const std::string &filePath, const std::string &strConfigFile, const std::string &strDefaultConfigFile )
{
	std::fstream configFile ;
	const std::string fullPath =filePath + strConfigFile ;

	if( !sFile.openFile( fullPath, configFile, std::fstream::in ) )
	{
		const std::string defaultFullPath = filePath + strDefaultConfigFile ;
		if( !sFile.saveAs( defaultFullPath, fullPath ) )
		{
			return false ;
		}
	}

	sFile.closeFile( configFile ) ;

	Load( fullPath.c_str() ) ;

	return true ;
}


int ConfigMgr::GetInt(const std::string& key, int defaultVal) const
{
	return m_ptree.get(key, defaultVal);
}

std::string ConfigMgr::GetString(const std::string& key, const std::string& defaultVal) const
{
	return m_ptree.get<std::string>(key, defaultVal);
}

float ConfigMgr::GetFloat(const std::string& key, float defaultVal) const
{
	return m_ptree.get(key, defaultVal);
}

bool ConfigMgr::GetStringArray( const std::string& key, const std::string& sepStr, std::vector<std::string>& strArray ) const
{
	std::string val = GetString(key, "");
	if (val.empty())
	{
		return false;
	}

	boost::char_separator<char> sep(sepStr.c_str());
	boost::tokenizer<boost::char_separator<char> > tok(val, sep);
	boost::tokenizer<boost::char_separator<char> >::iterator it = tok.begin();

	for (;it != tok.end(); ++it)
	{
		strArray.push_back(*it);
	}

	return !strArray.empty();
}

void ConfigMgr::Write(const std::string& file)
{
	boost::property_tree::ini_parser::write_ini(file, m_ptree);
}

void ConfigMgr::PutInt(const std::string& key, int defaultVal)
{
	m_ptree.put(key, defaultVal);
}
void ConfigMgr::PutFloat(const std::string& key, float defaultVal) 
{
	m_ptree.put(key, defaultVal);
}
void ConfigMgr::PutString(const std::string& key, const std::string& defaultVal) 
{
	m_ptree.put<std::string>(key, defaultVal);
}

void ConfigMgr::PutStringArray(const std::string& key, std::vector<std::string>& strArray,const std::string& sepStr)
{
	std::string value = "";
	for (size_t i = 0; i < strArray.size();i++)
	{
		value += strArray[i] + sepStr;
	}
	m_ptree.put<std::string>(key, value);
}
