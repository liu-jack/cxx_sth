#pragma once
#include "def/TypeDef.h"

class CMap;
class MapCity;
class CityGraph {
    enum {
        MAX_City_Cnt = 400,
        INFINITE_LEN = 99999, //当做无穷大
    };
    int curCityCnt;
    int arc[MAX_City_Cnt][MAX_City_Cnt];	//参数一：起点城池id，参数二：目的城市id，值为距离，初始根据相连城市表设为1或0

    int Distance[MAX_City_Cnt][MAX_City_Cnt];		//城池间距离
    int DynamicDistance[MAX_City_Cnt][MAX_City_Cnt];
    int DynamicPath[MAX_City_Cnt][MAX_City_Cnt];	//从参数一到参数二的路径中的下一步

public:
    CityGraph();
    void Init(const std::vector<const MapCity*>& cityLst);
	//常规弗洛伊德（无战斗）
    void Floyd(int arrDistance[MAX_City_Cnt][MAX_City_Cnt], int arrPath[MAX_City_Cnt][MAX_City_Cnt], CMap* pMap = NULL, uint32 fromid = 0, uint32 toid = 0);

	//非常规弗洛伊德（有战斗）
	void FloydFight(int arrDistance[MAX_City_Cnt][MAX_City_Cnt], int arrPath[MAX_City_Cnt][MAX_City_Cnt], CMap* pMap = NULL, uint32 fromid = 0, uint32 toid = 0);

    void GetCityPath(CMap& map, uint32 fromId, uint32 toId, std::queue<uint32>& path);	//获得常规路线
    int GetCityDistance(uint32 fromId, uint32 toId);
    void UpdateCityPath(CMap& map); //城市战斗状态改变时调用

	void GetCityFightPath(CMap& map, uint32 fromId, uint32 toId, std::queue<uint32>& path);	//获得非常规路线
};
