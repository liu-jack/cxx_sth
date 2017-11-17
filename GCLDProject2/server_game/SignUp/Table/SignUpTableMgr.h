
#ifndef __SIGN_UP_TABLE_MGR__H
#define __SIGN_UP_TABLE_MGR__H

#include "SimpleSingleton.h"
#include "game_data/datastruct/struct_sign_up.h"
#include "common/def/TypeDef.h"
#include "ptr_container/PtrMap.h"


class SignUp
{
private:
	const DB_SignUp& sign_up_;
	std::vector<int> extra_reward_;
	std::vector<int> extra_reward_num_;
public:
	SignUp(const DB_SignUp& sign_up);
	uint32 Id() const;
	const std::string& BaseReward() ;
	const std::string& BaseRewardNum() ;
	std::vector<int>& GetExtraReward() ;
	std::vector<int>& GetExtraRewardNum() ;
	uint32 NeedVip() const;
	uint32 PowerNum() const;
	int32 SupSignCost() const;
};

typedef PtrMap<uint32, SignUp> SignUpMap;


class SignUpTableMgr : public SimpleSingleton<SignUpTableMgr>
{
private:
	SignUpMap sign_up_map_;
	uint32 daily_sign_last_long_days_;
public:
	void Init();
	static SignUpTableMgr& Instance(){ static SignUpTableMgr T; return T; }	//¾²Ì¬µ¥Àý
	const SignUpMap& GetSignUpMap() const;
	uint32 SupSignCost(uint32 day);
	bool ParseItems(const std::string& rId, const std::string& rNum,IntV3Vec& retMap ) const;
	SignUp* GetSignUp(uint32 day);
	uint32 GetDailySignLastLongDay();
};


#define sSignUpTableMgr SignUpTableMgr::Instance()

#endif ///__SIGN_UP_TABLE_MGR__H


