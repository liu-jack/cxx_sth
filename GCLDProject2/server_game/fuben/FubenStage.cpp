#include "FubenStage.h"
#include "datastruct/struct_fuben.h"
#include "Fuben.pb.h"
#include "../reward/reward.h"
#include "System.h"
#include "utility/Utility.h"
#include "Base.h"

FubenStage::FubenStage(const DB_FubenStage& db) 
    : m_dbFubenStage(db)
{
    Utility::SplitStr2(m_dbFubenStage.stage_monsters, m_npcLst);	//����id��ȼ�д��ؿ�npc�б���
}

uint32 FubenStage::Id() const { return m_dbFubenStage.id; }		//�ؿ�id
uint32 FubenStage::ReqLv() const { return m_dbFubenStage.lv_req; }	//������ȼ�
uint32 FubenStage::FubenId() const { return m_dbFubenStage.fuben_id; }	//��������id
uint32 FubenStage::UnlockeHeroId() const { return m_dbFubenStage.unlocke_hero; }	//ͨ�ؿɽ������佫
void FubenStage::GetUnlockeHeroId( std::set< uint32>& heroIdSet ) const
{
	uint32   ulockId = m_dbFubenStage.unlocke_hero;
	if (ulockId > 0)
	{
		if (ulockId > 100)
		{
			uint32 san = 0;
			uint32 yushu = 0;
			do 
			{
				san = ulockId/100;
				yushu = ulockId%100;
				if (yushu > 0)
				{
					heroIdSet.insert(yushu);
				}
				ulockId = san;
			} while (san > 0);
		}
		else
		{
			heroIdSet.insert(ulockId);
		}
	}
}

uint32 FubenStage::PreStage() const { return m_dbFubenStage.pre_stage; }
int32  FubenStage::LandForm() const { return m_dbFubenStage.landform; }
const string& FubenStage::GetAwardList() const { return m_dbFubenStage.reward_list; }

//����ؿ�����Ҫ������
void FubenStage::GetCost(IntPair& cost, uint32 times) const
{
    int val = m_dbFubenStage.cost_value + times * m_dbFubenStage.cost_ratio;		//ʵ������ֵ = ��������ֵ + ����*����ϵ��
	if(IsResourceStage())
	{
		val = MIN((uint32)val,m_dbFubenStage.max_cost_diamond);
	}
	cost.first = m_dbFubenStage.cost_type;
	cost.second = -val;
}	

//��ȡ������Ϣ
void FubenStage::GetReward(IntPairVec& reward) const
{
    if (m_dbFubenStage.reward_exp > 0)		//���齱��
    {
		reward.push_back(std::make_pair(Reward::PlayerExp, m_dbFubenStage.reward_exp));
    }
    if (m_dbFubenStage.reward_curr_value > 0)		//��Ӧ��Դ����
    {
        reward.push_back(std::make_pair(m_dbFubenStage.reward_curr_type, m_dbFubenStage.reward_curr_value));
    }
    if (m_dbFubenStage.reward_item_id > 0)
    {
        if (System::Randf(0, 1) < m_dbFubenStage.reward_item_rate)	//��ý������ߵĸ���
        {
            reward.push_back(std::make_pair(m_dbFubenStage.reward_item_id, 1));
        }
    }
}

uint32 FubenStage::GetStageType() const
{
	return m_dbFubenStage.type;
}

//�Ƿ�����ͨ�ؿ�
bool FubenStage::IsCommonStage() const
{
    return m_dbFubenStage.type == COMMON_FUBEN;
}

//�Ƿ�����Դ�ؿ�
bool FubenStage::IsResourceStage() const
{
    return m_dbFubenStage.type == RESOURCE_FUBEN;
}

//�Ƿ��Ǿ��Źؿ�
bool FubenStage::IsGroupStage() const
{
    return m_dbFubenStage.type == GROUP_FUBEN;
}

//�Ƿ�������ؿ�
bool FubenStage::IsFogStage() const
{
    return m_dbFubenStage.type == FOG_FUBEN;
}

//�Ƿ���Զ���ؿ�
bool FubenStage::IsCrusadeStage() const
{
	return m_dbFubenStage.type == CRUSADE_FUBEN;
}

//�Ƿ���Զ�������ؿ�
bool FubenStage::IsCruGiftsStage() const
{
	return m_dbFubenStage.type == CRUSADE_GIFTS_FUBEN;
}

bool FubenStage::IsBranchStage() const
{
	return m_dbFubenStage.type == BRANCH_FUBEN;
}

bool FubenStage::IsHideStage() const
{
	return m_dbFubenStage.type == HIDE_FUBEN;
}

uint32 FubenStage::HideUnlockStar() const
{
	return m_dbFubenStage.hide_unlock_star;
}

uint32 FubenStage::BranchGroupId() const
{
	return m_dbFubenStage.branch_group_id;
}
