#ifndef _SCONFIG_H_
#define _SCONFIG_H_

#include <string>

#include "ConfigMgr.h"
#include "SimpleSingleton.h"


const std::string configPatch = "./GameClientsConfig/mmo_client.conf" ;

class SConfig:public SimpleSingleton<SConfig>, public ConfigMgr
{
private:
	friend class SimpleSingleton<SConfig>;

private:
	SConfig() {}

public:
	virtual ~SConfig() {}
};


#define sConfig (SConfig::Instance())

#endif
