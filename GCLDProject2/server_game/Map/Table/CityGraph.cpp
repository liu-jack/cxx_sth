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

//����1�����룬����2��·��
void CityGraph::Floyd(int arrDistance[MAX_City_Cnt][MAX_City_Cnt], int arrPath[MAX_City_Cnt][MAX_City_Cnt], CMap* pMap /* = NULL */, uint32 fromid/* = 0*/, uint32 toid/* = 0*/)
{
	LLOG("CityGraph::Floyd, curCityCnt=%d",curCityCnt);
    int i, j, k;
    for (i = 1; i <= curCityCnt; ++i) //��ʼ��	���г���
	{
        for (j = 1; j <= curCityCnt; ++j)
        {
            if (i == j) {
                arrDistance[i][j] = 0;
            }else{
                arrDistance[i][j] = arc[i][j]>0 ? arc[i][j] : INFINITE_LEN;	//������1�������99999,�Լ����Լ�Ϊ0
				
				if(i == TO_IDX(fromid) && j == TO_IDX(toid))
				{
					LLOG("arrDistance[i][j]=%d,arc[i][j]=%d",arrDistance[i][j],arc[i][j]);
				}

				//init�����ⲿ��
                if (City* pCity = pMap ? pMap->GetCity(TO_ID(j, pMap->Tid)) : NULL)		//City* pCity = pMap ? pMap->GetCity(TO_ID(i, pMap->Tid) ???iӦ��Ϊ��ʼ���У�Ŀ�ĳ���Ϊj
				{
					City* fCity = pMap->GetCity(TO_ID(i, pMap->Tid));

					if(i == TO_IDX(fromid) && j == TO_IDX(toid))
					{
						LLOG("fCity:country=%d,pCity:countryid=%d",fCity->GetCountryId(),pCity->GetCountryId());
					}

					if(fCity->GetCountryId() != pCity->GetCountryId())	//�ж�����֮�䲻����
					{
						if(arrDistance[i][j] != 1 || toid != TO_ID(j, pMap->Tid))		//i,j��������Ŀ��ǳ������һ���ǳ�����1
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
            arrPath[i][j] = arrDistance[i][j] < INFINITE_LEN ? j : -1;	//�޷��ߵ�Ϊ-1����������ߵ�����һ����Ϊj
			if(i == TO_IDX(fromid) && j == TO_IDX(toid))
			{
				LLOG("arrPath[%d][%d] = %d",i,j,arrPath[i][j]);
			}
        }
	}

    //����������˳�μ��룬���޸����·��
    for (k = 1; k <= curCityCnt; ++k)	//������;��
	{
        for (i = 1; i <= curCityCnt; ++i) //��i��j֮�����k
		{
			for (j = 1; j <= curCityCnt; ++j) 
			{
                if (arrDistance[i][k] + arrDistance[k][j] < arrDistance[i][j])
				{
                    arrDistance[i][j] = arrDistance[i][k] + arrDistance[k][j];
                    arrPath[i][j] = arrPath[i][k]; //���ĺ�̵�
                }
            }
		}
	}
}

//����1�����룬����2��·��
void CityGraph::FloydFight(int arrDistance[MAX_City_Cnt][MAX_City_Cnt], int arrPath[MAX_City_Cnt][MAX_City_Cnt], CMap* pMap /* = NULL */, uint32 fromid/* = 0*/, uint32 toid/* = 0*/)
{
	LLOG("CityGraph::FloydFight, curCityCnt=%d",curCityCnt);
	int i, j, k;
	for (i = 1; i <= curCityCnt; ++i) //��ʼ��	���г���
	{
		for (j = 1; j <= curCityCnt; ++j)
		{
			if (i == j) {
				arrDistance[i][j] = 0;
			}else{
				arrDistance[i][j] = arc[i][j]>0 ? arc[i][j] : INFINITE_LEN;	//������1�������99999,�Լ����Լ�Ϊ0

				if(i == TO_IDX(fromid) && j == TO_IDX(toid))
				{
					LLOG("arrDistance[i][j]=%d,arc[i][j]=%d",arrDistance[i][j],arc[i][j]);
				}

				//�����;�ǳ����׶�������ǣ������Ϊ�����
				if (City* pCity = pMap ? pMap->GetCity(TO_ID(j, pMap->Tid)) : NULL)		//City* pCity = pMap ? pMap->GetCity(TO_ID(i, pMap->Tid) ???iӦ��Ϊ��ʼ���У�Ŀ�ĳ���Ϊj
				{
					if(pCity->IsCapitalCity()/* || pCity->IsMonsterCity()*/)	//����ǳ����׶�������
					{
						arrDistance[i][j] = INFINITE_LEN;
					}
				}
			}
			arrPath[i][j] = arrDistance[i][j] < INFINITE_LEN ? j : -1;	//�޷��ߵ�Ϊ-1����������ߵ�����һ����Ϊj
			if(i == TO_IDX(fromid) && j == TO_IDX(toid))
			{
				LLOG("arrPath[%d][%d] = %d",i,j,arrPath[i][j]);
			}
		}
	}

	//����������˳�μ��룬���޸����·��
	for (k = 1; k <= curCityCnt; ++k)	//������;��
	{
		for (i = 1; i <= curCityCnt; ++i) //��i��j֮�����k
		{
			for (j = 1; j <= curCityCnt; ++j) 
			{
				if (arrDistance[i][k] + arrDistance[k][j] < arrDistance[i][j])
				{
					arrDistance[i][j] = arrDistance[i][k] + arrDistance[k][j];
					arrPath[i][j] = arrPath[i][k]; //���ĺ�̵�
				}
			}
		}
	}
}

////����1�����룬����2��·��
//void CityGraph::Floyd(int arrDistance[MAX_City_Cnt][MAX_City_Cnt], int arrPath[MAX_City_Cnt][MAX_City_Cnt], CMap* pMap /* = NULL */)
//{
//	NLOG("CityGraph::Floyd");
//	int i, j, k;
//	for (i = 1; i <= curCityCnt; ++i) //��ʼ��	���г���
//	{
//		for (j = 1; j <= curCityCnt; ++j)
//		{
//			if (i == j) {
//				arrDistance[i][j] = 0;
//			}else{
//				arrDistance[i][j] = arc[i][j]>0 ? arc[i][j] : INFINITE_LEN;	//������1�������99999
//
//				//init�����ⲿ��
//				if (City* pCity = pMap ? pMap->GetCity(TO_ID(i, pMap->Tid)) : NULL)		//City* pCity = pMap ? pMap->GetCity(TO_ID(i, pMap->Tid) ???iӦ��Ϊ��ʼ���У�Ŀ�ĳ���Ϊj
//				{
//
//					if (pCity->CanPass() == false) 
//					{
//						arrDistance[i][j] = INFINITE_LEN;
//					}
//				}
//			}
//			arrPath[i][j] = arrDistance[i][j] < INFINITE_LEN ? j : -1;	//�޷��ߵ�Ϊ-1����������ߵ�����һ����Ϊj
//		}
//	}
//
//	//����������˳�μ��룬���޸����·��
//	for (k = 1; k <= curCityCnt; ++k)	//������;��
//	{
//		for (i = 1; i <= curCityCnt; ++i) //��i��j֮�����k
//		{
//			for (j = 1; j <= curCityCnt; ++j) 
//			{
//				if (arrDistance[i][k] + arrDistance[k][j] < arrDistance[i][j])
//				{
//					arrDistance[i][j] = arrDistance[i][k] + arrDistance[k][j];
//					arrPath[i][j] = arrPath[i][k]; //���ĺ�̵�
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
        //path.push(TO_ID(toId, map.Tid));		//�����ϲ��ü����һ��
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
		//path.push(TO_ID(toId, map.Tid));		//�����ϲ��ü����һ��
	}
}

#undef TO_IDX
#undef TO_ID