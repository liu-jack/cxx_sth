#include "CityGraph.h"
#include "Base.h"
#include "datastruct/struct_city.h"
#include "../base/BaseMap.h"
#include "../city/city.h"
#include "MapTableMgr.h"
#include "MapCity.h"

#define TO_IDX(id)          (((id-1) % 164) + 1)
#define TO_ID(idx, mapId)   ((mapId) * 164 + (idx))

CityGraph::CityGraph()
{
    ZeroMemoryThis;
}
void CityGraph::Init(const std::vector<const MapCity*>& cityLst)
{
	//NLOG("CityGraph::Init");
    curCityCnt = cityLst.size();

    MMO_ASSERT(curCityCnt > 0);

	for (int i = 1; i <= curCityCnt; ++i)
	{
        if (const MapCity* pCity = cityLst[i-1])
		{
			for (std::vector<int>::const_iterator it = pCity->m_linkCity.begin();
				 it != pCity->m_linkCity.end(); ++it)
			{
                assert(TO_IDX(*it) < MAX_City_Cnt);
                arc[i][TO_IDX(*it)] = 1;
			}
		}
	}

	Floyd(Distance, DynamicPath, NULL);
	memcpy(DynamicDistance, Distance, sizeof(Distance));
}

//参数1：距离，参数2：路径
void CityGraph::Floyd(int arrDistance[MAX_City_Cnt][MAX_City_Cnt], int arrPath[MAX_City_Cnt][MAX_City_Cnt], CMap* pMap /* = NULL */, uint32 fromid/* = 0*/, uint32 toid/* = 0*/)
{
	LLOG("CityGraph::Floyd, curCityCnt=%d",curCityCnt);
    int i, j, k;
    for (i = 1; i <= curCityCnt; ++i) //初始化	所有城市
	{
        for (j = 1; j <= curCityCnt; ++j)
        {
            if (i == j) {
                arrDistance[i][j] = 0;
            }else{
                arrDistance[i][j] = arc[i][j]>0 ? arc[i][j] : INFINITE_LEN;	//相连就1，否则就99999,自己到自己为0
				
				if(i == TO_IDX(fromid) && j == TO_IDX(toid))
				{
					LLOG("arrDistance[i][j]=%d,arc[i][j]=%d",arrDistance[i][j],arc[i][j]);
				}

				//init跳过这部分
                if (City* pCity = pMap ? pMap->GetCity(TO_ID(j, pMap->Tid)) : NULL)		//City* pCity = pMap ? pMap->GetCity(TO_ID(i, pMap->Tid) ???i应该为起始城市，目的城市为j
				{
					City* fCity = pMap->GetCity(TO_ID(i, pMap->Tid));

					if(i == TO_IDX(fromid) && j == TO_IDX(toid))
					{
						LLOG("fCity:country=%d,pCity:countryid=%d",fCity->GetCountryId(),pCity->GetCountryId());
					}

					if(fCity->GetCountryId() != pCity->GetCountryId())	//敌对势力之间不可走
					{
						if(arrDistance[i][j] != 1 || toid != TO_ID(j, pMap->Tid))		//i,j相连并且目标城池是最后一个城池则还是1
							arrDistance[i][j] = INFINITE_LEN;
					}
					else
					{
						if (pCity->CanPass() == false) 
						{
							if (toid != TO_ID(j, pMap->Tid))
							{
								arrDistance[i][j] = INFINITE_LEN;
							}
						}
					}
                }
            }
            arrPath[i][j] = arrDistance[i][j] < INFINITE_LEN ? j : -1;	//无法走到为-1，如果可以走到就下一个点为j
			if(i == TO_IDX(fromid) && j == TO_IDX(toid))
			{
				LLOG("arrPath[%d][%d] = %d",i,j,arrPath[i][j]);
			}
        }
	}

    //将各个顶点顺次加入，并修改最短路径
    for (k = 1; k <= curCityCnt; ++k)	//各个中途点
	{
        for (i = 1; i <= curCityCnt; ++i) //在i，j之间加入k
		{
			for (j = 1; j <= curCityCnt; ++j) 
			{
                if (arrDistance[i][k] + arrDistance[k][j] < arrDistance[i][j])
				{
                    arrDistance[i][j] = arrDistance[i][k] + arrDistance[k][j];
                    arrPath[i][j] = arrPath[i][k]; //更改后继点
                }
            }
		}
	}
}

//参数1：距离，参数2：路径
void CityGraph::FloydFight(int arrDistance[MAX_City_Cnt][MAX_City_Cnt], int arrPath[MAX_City_Cnt][MAX_City_Cnt], CMap* pMap /* = NULL */, uint32 fromid/* = 0*/, uint32 toid/* = 0*/)
{
	LLOG("CityGraph::FloydFight, curCityCnt=%d",curCityCnt);
	int i, j, k;
	for (i = 1; i <= curCityCnt; ++i) //初始化	所有城市
	{
		for (j = 1; j <= curCityCnt; ++j)
		{
			if (i == j) {
				arrDistance[i][j] = 0;
			}else{
				arrDistance[i][j] = arc[i][j]>0 ? arc[i][j] : INFINITE_LEN;	//相连就1，否则就99999,自己到自己为0

				if(i == TO_IDX(fromid) && j == TO_IDX(toid))
				{
					LLOG("arrDistance[i][j]=%d,arc[i][j]=%d",arrDistance[i][j],arc[i][j]);
				}

				//如果中途城池是首都或蛮族城，则距离为无穷大
				if (City* pCity = pMap ? pMap->GetCity(TO_ID(j, pMap->Tid)) : NULL)		//City* pCity = pMap ? pMap->GetCity(TO_ID(i, pMap->Tid) ???i应该为起始城市，目的城市为j
				{
					if(pCity->IsCapitalCity()/* || pCity->IsMonsterCity()*/)	//蛮族城池与首都不可走
					{
						arrDistance[i][j] = INFINITE_LEN;
					}
				}
			}
			arrPath[i][j] = arrDistance[i][j] < INFINITE_LEN ? j : -1;	//无法走到为-1，如果可以走到就下一个点为j
			if(i == TO_IDX(fromid) && j == TO_IDX(toid))
			{
				LLOG("arrPath[%d][%d] = %d",i,j,arrPath[i][j]);
			}
		}
	}

	//将各个顶点顺次加入，并修改最短路径
	for (k = 1; k <= curCityCnt; ++k)	//各个中途点
	{
		for (i = 1; i <= curCityCnt; ++i) //在i，j之间加入k
		{
			for (j = 1; j <= curCityCnt; ++j) 
			{
				if (arrDistance[i][k] + arrDistance[k][j] < arrDistance[i][j])
				{
					arrDistance[i][j] = arrDistance[i][k] + arrDistance[k][j];
					arrPath[i][j] = arrPath[i][k]; //更改后继点
				}
			}
		}
	}
}

////参数1：距离，参数2：路径
//void CityGraph::Floyd(int arrDistance[MAX_City_Cnt][MAX_City_Cnt], int arrPath[MAX_City_Cnt][MAX_City_Cnt], CMap* pMap /* = NULL */)
//{
//	NLOG("CityGraph::Floyd");
//	int i, j, k;
//	for (i = 1; i <= curCityCnt; ++i) //初始化	所有城市
//	{
//		for (j = 1; j <= curCityCnt; ++j)
//		{
//			if (i == j) {
//				arrDistance[i][j] = 0;
//			}else{
//				arrDistance[i][j] = arc[i][j]>0 ? arc[i][j] : INFINITE_LEN;	//相连就1，否则就99999
//
//				//init跳过这部分
//				if (City* pCity = pMap ? pMap->GetCity(TO_ID(i, pMap->Tid)) : NULL)		//City* pCity = pMap ? pMap->GetCity(TO_ID(i, pMap->Tid) ???i应该为起始城市，目的城市为j
//				{
//
//					if (pCity->CanPass() == false) 
//					{
//						arrDistance[i][j] = INFINITE_LEN;
//					}
//				}
//			}
//			arrPath[i][j] = arrDistance[i][j] < INFINITE_LEN ? j : -1;	//无法走到为-1，如果可以走到就下一个点为j
//		}
//	}
//
//	//将各个顶点顺次加入，并修改最短路径
//	for (k = 1; k <= curCityCnt; ++k)	//各个中途点
//	{
//		for (i = 1; i <= curCityCnt; ++i) //在i，j之间加入k
//		{
//			for (j = 1; j <= curCityCnt; ++j) 
//			{
//				if (arrDistance[i][k] + arrDistance[k][j] < arrDistance[i][j])
//				{
//					arrDistance[i][j] = arrDistance[i][k] + arrDistance[k][j];
//					arrPath[i][j] = arrPath[i][k]; //更改后继点
//				}
//			}
//		}
//	}
//}

void CityGraph::GetCityPath(CMap& map, uint32 fromId, uint32 toId, std::queue<uint32>& path)
{
	//NLOG("CityGraph::GetCityPath");
	std::queue<uint32> tempque;
	std::swap(path, tempque);

	Floyd(DynamicDistance, DynamicPath, &map, fromId, toId);

    toId = TO_IDX(toId); fromId = TO_IDX(fromId);
	
    if (fromId >= CityGraph::MAX_City_Cnt || toId >= CityGraph::MAX_City_Cnt)
    {
        return;
    }

	LLOG("fromId=%d,toId=%d,dis=%ld",fromId,toId,DynamicDistance[fromId][toId]);
	//for(int i=0; i<MAX_City_Cnt; i++)
	//{
	//	for(int j=0; j<MAX_City_Cnt; j++)
	//	{
	//		LLOG("DynamicDistance[%d][%d] = %d",i,j,DynamicDistance[i][j]);
	//	}
	//}
	
    if (DynamicDistance[fromId][toId] < INFINITE_LEN && DynamicDistance[fromId][toId] > 0)
    {
        while (fromId != toId)
        {
            fromId = DynamicPath[fromId][toId];
			if(fromId == 0)
			{
				PLOG("[IMPORTANT BUG] cant get path");
				break;
			}
            path.push(TO_ID(fromId, map.Tid));
			LLOG("push fromId=%d",fromId);
		}
        //path.push(TO_ID(toId, map.Tid));		//理论上不用加最后一次
    }
}
int CityGraph::GetCityDistance(uint32 fromId, uint32 toId)
{
	//NLOG("CityGraph::GetCityDistance");
    toId = TO_IDX(toId); fromId = TO_IDX(fromId);

    return Distance[fromId][toId];
}
void CityGraph::UpdateCityPath(CMap& map)
{
	//NLOG("CityGraph::UpdateCityPath");
    Floyd(DynamicDistance, DynamicPath, &map);
}

void CityGraph::GetCityFightPath(CMap& map, uint32 fromId, uint32 toId, std::queue<uint32>& path)
{
	//NLOG("CityGraph::GetCityPath");
	std::queue<uint32> tempque;
	std::swap(path, tempque);

	FloydFight(DynamicDistance, DynamicPath, &map, fromId, toId);

	toId = TO_IDX(toId); fromId = TO_IDX(fromId);

	if (fromId >= CityGraph::MAX_City_Cnt || toId >= CityGraph::MAX_City_Cnt)
	{
		return;
	}

	LLOG("fromId=%d,toId=%d,dis=%ld",fromId,toId,DynamicDistance[fromId][toId]);


	if (DynamicDistance[fromId][toId] < INFINITE_LEN && DynamicDistance[fromId][toId] > 0)
	{
		while (fromId != toId)
		{
			fromId = DynamicPath[fromId][toId];
			if(fromId == 0)
			{
				PLOG("[IMPORTANT BUG] cant get path");
				break;
			}
			path.push(TO_ID(fromId, map.Tid));
			LLOG("push fromId=%d",fromId);
		}
		//path.push(TO_ID(toId, map.Tid));		//理论上不用加最后一次
	}
}

#undef TO_IDX
#undef TO_ID