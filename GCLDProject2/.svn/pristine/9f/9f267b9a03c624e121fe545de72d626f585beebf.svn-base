#pragma once

#include <vector>
#include <boost/property_tree/ptree.hpp>

class ConfigMgr
{
public:
	void Load(const std::string& file);
	bool LoadOrCopyFormDefaultFile( const std::string &filePath, const std::string &strConfigFile, const std::string &strDefaultConfigFile ) ;

	int GetInt(const std::string& key, int defaultVal) const;
	float GetFloat(const std::string& key, float defaultVal) const;
	std::string GetString(const std::string& key, const std::string& defaultVal) const;
	bool GetStringArray(const std::string& key, const std::string& sepStr, std::vector<std::string>& strArray) const;

	void Write(const std::string& file);
	void PutInt(const std::string& key, int defaultVal) ;
	void PutFloat(const std::string& key, float defaultVal) ;
	void PutString(const std::string& key, const std::string& defaultVal) ;
	void PutStringArray(const std::string& key, std::vector<std::string>& strArray,const std::string& sepStr);

private:
	boost::property_tree::ptree		m_ptree;
};



