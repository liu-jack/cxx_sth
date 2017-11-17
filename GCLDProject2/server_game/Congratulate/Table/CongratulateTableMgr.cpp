/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/


#include "Congratulate/Table/CongratulateTableMgr.h"
#include "utility/Utility.h"
#include "DbTool.h"



CongratulateConfig::CongratulateConfig(const DB_CongratulateConfig& db)
:db_(db)
{
	Utility::SplitStr2(db_.congratulate_reward_id,congratulate_rewrad_);
	Utility::SplitStr2(db_.be_congratulate_reward_id,be_congratulate_reward_);
}

uint32 CongratulateConfig::Id() const
{
	return db_.id;
}

uint32 CongratulateConfig::CongratulateType() const
{
	return db_.congratulate_type;
}

uint32 CongratulateConfig::Cat1() const
{
	return db_.cat1;
}

uint32 CongratulateConfig::BeforeRank() const
{
	return db_.before_rank;
}

const IntPairVec& CongratulateConfig::GetCongratulateReward() const
{
	return congratulate_rewrad_;
}

const IntPairVec& CongratulateConfig::GetBeCongratulateReward() const
{
	return be_congratulate_reward_;
}


CongratulateMgr::CongratulateMgr()
{
	memset(suit_pri,0,sizeof(suit_pri));
	memset(suit_mid,0,sizeof(suit_mid));
	memset(suit_adv,0,sizeof(suit_adv));
	memset(congratulate_shen_qi_,0,sizeof(congratulate_shen_qi_));

	FOREACH_DB_ITEM(config,DB_CongratulateConfig)
	{
		CongratulateConfig * congratulate_ = new CongratulateConfig(*config);
		congratulate_config_map_[congratulate_->Id()] = congratulate_;
	}
}

uint32 CongratulateMgr::GetTableId(const uint32 congratualte_type,const uint32 cat1,uint32 &before_rank)
{
	for(CongratulateConfigMap::iterator iter = congratulate_config_map_.begin();iter != congratulate_config_map_.end();++iter)
	{
		if(iter->second->CongratulateType() == congratualte_type && iter->second->Cat1() == cat1)
		{
			before_rank = iter->second->BeforeRank();
			return iter->first;
		}
	}
	return 0;
}

const IntPairVec* CongratulateMgr::GetCongratulateReward(const uint32 table_id)
{
	CongratulateConfig *iter = congratulate_config_map_.pfind(table_id);
	if(iter)
	{
		return &iter->GetCongratulateReward();
	}
	return NULL;
}

const CongratulateConfigMap& CongratulateMgr::getCongratulateConfigMap() const
{
	return congratulate_config_map_;
}
