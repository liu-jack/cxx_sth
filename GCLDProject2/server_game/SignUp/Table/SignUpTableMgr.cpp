

#include "SignUpTableMgr.h"
#include "utility/Utility.h"
#include "DbTool.h"

SignUp::SignUp( const DB_SignUp& sign_up )
:sign_up_(sign_up)
{
	Utility::SplitStr(sign_up_.extra_reward,extra_reward_,',');
	Utility::SplitStr(sign_up_.extra_reward_num,extra_reward_num_,',');
}

uint32 SignUp::Id() const
{
	return sign_up_.id;
}

const std::string& SignUp::BaseReward()  
{
	return sign_up_.base_reward;
}

const std::string& SignUp::BaseRewardNum() 
{
	return sign_up_.base_reward_num;
}



uint32 SignUp::NeedVip() const
{	
	return sign_up_.need_vip;
}

uint32 SignUp::PowerNum() const
{
	return sign_up_.power_num;
}

int32 SignUp::SupSignCost() const
{
	return static_cast<int32>(sign_up_.sup_sign_cost);
}

std::vector<int>& SignUp::GetExtraReward() 
{
	return extra_reward_;
}

std::vector<int>& SignUp::GetExtraRewardNum() 
{
	return extra_reward_num_;
}
void SignUpTableMgr::Init()
{
	daily_sign_last_long_days_ = 0;
	FOREACH_DB_ITEM(sign_up,DB_SignUp)
	{
		uint32 Id = sign_up->id;
		SignUp * signUp = new SignUp(*sign_up);
		sign_up_map_[Id] = signUp;
		if(Id > daily_sign_last_long_days_)
		{
			daily_sign_last_long_days_ = Id;
		}
	}
}

uint32 SignUpTableMgr::SupSignCost( uint32 day )
{
	SignUp* sign_up = sign_up_map_.pfind(day);
	if(sign_up)
	{
		return sign_up->SupSignCost();
	}
	return 0;
}


const SignUpMap& SignUpTableMgr::GetSignUpMap() const
{
	return sign_up_map_;
}

SignUp* SignUpTableMgr::GetSignUp( uint32 day )
{
	SignUp* sign_up = sign_up_map_.pfind(day);
	if(sign_up)
	{
		return sign_up;
	}
	return NULL;
}

bool SignUpTableMgr::ParseItems( const std::string& rId, const std::string& rNum,IntV3Vec& retMap ) const
{
	std::vector<string> retVecId;
	std::vector<int> retVecNum;
	Utility::SplitStr(rId, retVecId, ',');
	Utility::SplitStr(rNum, retVecNum, ',');
	int idCount = (int)retVecId.size();
	int numCount = (int)retVecNum.size();
	if (idCount == 0 || numCount == 0 || idCount != numCount)
	{
		return false;
	}
	for (int i=0; i< idCount; i++)
	{
		IntV3 tmp;
		std::string curItem  = retVecId[i];
		std::vector<string> retItemAttr;
		Utility::SplitStr(curItem, retItemAttr, '|');
		int nAttr = (int)retItemAttr.size();
		if (nAttr == 1)
		{
			tmp.first = atoi(retItemAttr[0].c_str());
			tmp.third = "0";
		}
		else if (nAttr == 2)
		{
			tmp.first = atoi(retItemAttr[0].c_str());
			tmp.third = retItemAttr[1];
		}
		else
		{
			continue;
		}
		tmp.second = retVecNum[i];
		retMap.push_back(tmp);
	}
	return true;
}

uint32 SignUpTableMgr::GetDailySignLastLongDay()
{
	return daily_sign_last_long_days_;
}