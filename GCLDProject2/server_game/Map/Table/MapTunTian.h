#pragma once
#include "def/TypeDef.h"
struct DB_TunTian_Up;

class MapTunTian {
	const DB_TunTian_Up& m_dbMapTunTian;	//屯田信息

public:

	explicit MapTunTian(const DB_TunTian_Up& db);

	//uint16 Id() const;	//迷雾标号

	//uint32 CountryId() const;	//所属国
	//uint32 BattleCityId() const;	//城池id
	//uint32 StageId() const;	//战斗关卡
	//uint32 ShowLevel() const;	//显示等级

	uint32 GetExp() const;
	uint32 GetAwardFactor() const;
};