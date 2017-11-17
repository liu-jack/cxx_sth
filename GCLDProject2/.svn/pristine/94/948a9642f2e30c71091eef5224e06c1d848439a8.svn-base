#include "FubenDataMgr.h"
#include "DbTool.h"
#include "datastruct/struct_fuben.h"
//#include "../AutomaticUpdate/AutomaticUpdate.h"
#include "../session/PlayerPool.h"
#include "FubenLog.h"

void FubenDataMgr::Init()
{
	//副本数据表
    FOREACH_DB_ITEM(ptr1, DB_FubenProto)	//stage_proto 副本大关
    {
		if (ptr1->name_string_id > 0)
		{
			m_fubenMap[ptr1->id] = new Fuben(*ptr1);
		}
    }

	//关卡数据表
    FOREACH_DB_ITEM(ptr2, DB_FubenStage)	//stage_level 小关卡
    {
		FubenMap::iterator it = --m_fubenMap.end();
		Fuben* testFuben = it->second;
		if (ptr2->name_id > 0 || ptr2->fuben_id == testFuben->Id())
		{
			m_stageMap[ptr2->id] = new FubenStage(*ptr2);
		}
    }

    //避免数据库里的关卡编号没按顺序排，读完DB后再扔进vector
    for (StageMap::const_iterator itStage = m_stageMap.begin();
        itStage != m_stageMap.end(); ++itStage)
    {
        // 普通关、资源关、军团关，组成副本  ？迷雾与远征没算在内
        const FubenStage* tmp = itStage->second;
        if (tmp->IsCommonStage() ||
            tmp->IsResourceStage() ||
            tmp->IsGroupStage() || tmp->IsHideStage() || tmp->IsBranchStage())
        {
            m_FubenStageList[tmp->FubenId()].push_back(tmp);
        }

		if(tmp->IsCrusadeStage() || tmp->IsCruGiftsStage())	//是远征关或者远征奖励关
		{
			m_CrusadeStageList[tmp->FubenId()].push_back(tmp);
		}
		if(tmp->IsBranchStage())
		{
			m_BranchStageList[tmp->FubenId()].push_back(tmp);
		}
		if(tmp->IsHideStage())
		{
			m_HideStageList[tmp->FubenId()].push_back(tmp);
		}
    }

	//sAutoUpdate.Add(boost::bind(&FubenDataMgr::initCrusadeTable,this),detail_enum::TIME_ZERO,detail_enum::PER_DAY);
}

#include "OS.h"
void FubenDataMgr::initCrusadeTable()
{
	//NLOG("FubenDataMgr::initCrusadeTable");
	//FOREACH_PLAYERPTR(PlayerId,playerPtr)
	//{
	//	if(!playerPtr->GetDataIsLost())
	//	{
	//		NLOG("WorldMap::updateArmyResourcePerHour one");
	//		playerPtr->m_fubenLog->InitCrusadeData(playerPtr.get());
	//	}
	//}
}

//获得一章副本信息
const Fuben* FubenDataMgr::GetFuben(uint32 fubenId) const
{
    return m_fubenMap.pfind(fubenId);
}

//获得一个关卡信息
const FubenStage* FubenDataMgr::GetStage(uint32 stageId) const
{
    return m_stageMap.pfind(stageId);
}

//获取所有副本中第一章副本信息
const Fuben& FubenDataMgr::GetFirstFuben() const
{
    return *m_fubenMap.begin()->second;
}

const FubenMap& FubenDataMgr::GetFubenMap() const
{	
	return m_fubenMap;
}

//获得所有关卡中第一个关卡的信息
const FubenStage& FubenDataMgr::GetFirstStage() const
{
    return *m_stageMap.begin()->second;
}

//通过关卡id获得一章副本信息
const Fuben* FubenDataMgr::GetFubenByStageID(uint32 stageId) const
{
    if (const FubenStage* pStage = m_stageMap.pfind(stageId))
    {
        return m_fubenMap.pfind( pStage->FubenId() );
    }
    return NULL;
}

//获得当前远征副本的关卡数
uint32 FubenDataMgr::GetCurrentFubenStageNum(uint32 stageID)
{
	if (const FubenStage* pStage = m_stageMap.pfind(stageID))
	{
		return m_CrusadeStageList[pStage->FubenId()].size();
	}
	return 0;
}

uint32 FubenDataMgr::GetCurrentCrusadeStageNum(uint32 stageId)
{
	if (const FubenStage* pStage = m_stageMap.pfind(stageId))
	{
		const std::vector<const FubenStage* > curAllStage = m_CrusadeStageList[pStage->FubenId()];
		uint32 count = 0;
		for(std::vector<const FubenStage*>::const_iterator iter = curAllStage.begin();iter != curAllStage.end();++iter)
		{
			if((*iter)->IsCrusadeStage())
				count++;
		}
		return count;
	}
	return 0;
}

uint32 FubenDataMgr::GetCurrentCrusadeRewardStageNum(uint32 stageId)
{
	if(GetCurrentFubenStageNum(stageId) >= GetCurrentCrusadeStageNum(stageId))
	{
		return GetCurrentFubenStageNum(stageId) - GetCurrentCrusadeStageNum(stageId);
	}
	return 0;
}

//通过副本id获得当前远征第一个关卡
const FubenStage* FubenDataMgr::GetFirstCrusadeStageByFubenID(uint32 fubenID) const
{
	FubenStageMap::const_iterator it = m_CrusadeStageList.find(fubenID);	//<uint32, std::vector<const FubenStage*>>
	if (it != m_CrusadeStageList.end())
	{
		return (it->second)[0];
	}
	return NULL;
}

const FubenStage* FubenDataMgr::GetFirstCrusadeStage() const
{
	return  (m_CrusadeStageList.begin()->second)[0];
}

//获得关卡的前置关卡
const uint32 FubenDataMgr::GetPrestageID(uint32 stageid) const
{
	return m_stageMap.pfind(stageid)->PreStage();
}

const uint32 FubenDataMgr::GetBranchFubenGroupId(const uint32 stageId) const
{
	if (const FubenStage* p = GetStage(stageId))
	{
		if(p->IsBranchStage())
		{
			return p->BranchGroupId();
		}
	}
	return 0;
}

//获取一个章节副本中所有关卡的信息
const std::vector<const FubenStage*>* FubenDataMgr::GetFubenStageList(uint32 fubenId) const
{
    FubenStageMap::const_iterator it = m_FubenStageList.find(fubenId);
    if (it != m_FubenStageList.end())
    {
        return &(it->second);
    }
    return NULL;
}

const std::vector<const FubenStage*> * FubenDataMgr::GetCrusadeFubenStageList(uint32 fubenId) const
{
	FubenStageMap::const_iterator it = m_CrusadeStageList.find(fubenId);
	if (it != m_CrusadeStageList.end())
	{
		return &(it->second);
	}
	return NULL;
}

const std::vector<const FubenStage*> * FubenDataMgr::GetBranchFubenStageList(uint32 fubenId) const
{
	FubenStageMap::const_iterator it = m_BranchStageList.find(fubenId);
	if (it != m_BranchStageList.end())
	{
		return &(it->second);
	}
	return NULL;
}

const std::vector<const FubenStage*> * FubenDataMgr::GetHideFubenStageList(uint32 fubenId) const
{
	FubenStageMap::const_iterator it = m_HideStageList.find(fubenId);
	if (it != m_HideStageList.end())
	{
		return &(it->second);
	}
	return NULL;
}

//获得下一关id
bool FubenDataMgr::GetNextStageID(uint32& rFubenId, uint32& rStageId) const
{
    const std::vector<const FubenStage*>* vecStage = GetFubenStageList(rFubenId);

    if (vecStage == NULL) return false;

    std::vector<const FubenStage*>::const_iterator itStage = vecStage->begin();
    for (; itStage != vecStage->end(); ++itStage)
    {
        if ((*itStage)->Id() == rStageId) //找到当前关卡，查看其是否为最后一关
        {
            if (++itStage != vecStage->end()) //没到副本最后一关，切到下一关，副本ID不变
            {
                rStageId = (*itStage)->Id();
                return true;
            }
            else //过了最后一关，切副本
            {
                FubenMap::const_iterator itFuben = m_fubenMap.find(rFubenId);
				uint32 lastmode = itFuben->second->GetStageMode();
                if (itFuben != m_fubenMap.end())
                {
                    if (++itFuben != m_fubenMap.end())
                    {
						if(lastmode != itFuben->second->GetStageMode())
							return false;

                        rFubenId = itFuben->first;

                        if (const std::vector<const FubenStage*>* vec = GetFubenStageList(rFubenId))
                        {
                            if (vec->size() > 0)
                            {
                                if (const FubenStage* p = (*vec)[0]) //关卡换为副本的首个
                                {
                                    rStageId = p->Id();
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

uint32 FubenDataMgr::GetNextBranchStageId(uint32 FubenId,const uint32 stageId) const
{
	const std::vector<const FubenStage*>* vecStage = GetBranchFubenStageList(FubenId);
	if (vecStage == NULL) return false;

	std::vector<const FubenStage*>::const_iterator itStage = vecStage->begin();
	for (; itStage != vecStage->end(); ++itStage)
	{
		if ((*itStage)->Id() == stageId) //找到当前关卡，查看其是否为最后一关
		{
			if (++itStage != vecStage->end()) //没到副本最后一关，切到下一关，副本ID不变
			{
				return (*itStage)->Id();
			}
		}
	}
	return 0;
}

void FubenDataMgr::GetUnlockCharacter(const uint32 rFubenId,const uint32 rStageId,std::set<uint32>& unlockChar) const
{
	const std::vector<const FubenStage*>* vecStage = GetFubenStageList(rFubenId);

	if (vecStage == NULL) return;
	std::vector<const FubenStage*>::const_iterator itStage = vecStage->begin();
	for (; itStage != vecStage->end(); ++itStage)
	{
		if ((*itStage)->Id() == rStageId) //找到当前关卡，查看其是否为最后一关
		{
			(*itStage)->GetUnlockeHeroId(unlockChar);
			return;
		}
	}
}

// 是否是资源关卡
// bool FubenDataMgr::IsResourceStage(uint32 stageId)
// {
//     if (const FubenStage* p = GetStage(stageId))
//     {
//         return p->IsResourceStage();
//     }
//     return false;
// }

const StageMap& FubenDataMgr::GetStageMap() const
{
	return m_stageMap;
}
