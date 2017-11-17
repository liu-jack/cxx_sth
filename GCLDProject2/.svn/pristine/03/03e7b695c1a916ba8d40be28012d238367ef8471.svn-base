#pragma once
#include "def/TypeDef.h"
struct DB_MapFog;

class MapFog {
    const DB_MapFog& m_dbMapFog;	//当前迷雾关的所有信息

public:
    std::vector<int>  m_openCity;	//攻打过这一关卡之后可以显示哪些城市

   explicit MapFog(const DB_MapFog& db);

    uint16 Id() const;	//迷雾标号

    uint32 CountryId() const;	//所属国
    uint32 BattleCityId() const;	//城池id
    uint32 StageId() const;	//战斗关卡
	uint32 ShowLevel() const;	//显示等级
};