#include "CrossArsenalMgr.h"
#include "object/Player.h"
#include "../CrossWarManager/CrossWarMgr.h"
#include "Palace/PalaceMgr.h"
#include "utility/Utility.h"
#include "CrossWar.pb.h"
#include "cross_wall/CrossWorldMap.h"
#include "Map/country/country.h"

CrossArsenalMgr::CrossArsenalMgr()
{

}

void CrossArsenalMgr::SaveTo(pb::GS2C_Cross_Ammo_Infos& msg, uint32 countryid)
{
	pb::GS2C_Cross_Ammo_Info* msgHammer = msg.add_task_list();
	msgHammer->set_itemtype(pb::Hammer);
	msgHammer->set_remain_num(GetRestBuyTimes(pb::Hammer,countryid));

	pb::GS2C_Cross_Ammo_Info* msgTroops = msg.add_task_list();
	msgTroops->set_itemtype(pb::KillTroops);
	msgTroops->set_remain_num(GetRestBuyTimes(pb::KillTroops,countryid));

	pb::GS2C_Cross_Ammo_Info* msgShield = msg.add_task_list();
	msgShield->set_itemtype(pb::Shield);
	msgShield->set_remain_num(GetRestBuyTimes(pb::Shield,countryid));
}

void CrossArsenalMgr::Init()
{
	m_arsenalRec.clear();
}

void CrossArsenalMgr::BuyArsenalItems(Player& player, pb::ArsenalType type, pb::GS2C_Buy_Ammo_Rsp& msg)
{
	msg.set_ret(pb::Cro_Common_Err);
	if(sCrossWarMgr.GetCrossState() == pb::State_Start)
	{
		int officeid = sPalaceMgr(player.GetCountryId())->GetOfficeId(player.GetGuid());
		Country* mycountry = sCrossWorldMap.GetCountry(player.GetCountryId());
		if(!mycountry) return;

		switch (type)
		{
		case pb::Hammer:
			if( 1 == officeid )	//皇帝才可以买
			{
				if(player.EnoughCurrency(eSysGold,GET_BASE_DEF_INT(pb::BD_CROSS_ATTACK_CITY_PRICE)))
				{
					ArsenalRecMap& recByCountry = m_arsenalRec[player.GetCountryId()];
					std::map<pb::ArsenalType, uint32>::iterator it = recByCountry.find(type);
					if(it == recByCountry.end())
					{
						player.TryDeductCurrency(pb::IR_CROSS_HAMMER_COST,eSysGold,GET_BASE_DEF_INT(pb::BD_CROSS_ATTACK_CITY_PRICE));
						recByCountry.insert(std::make_pair(pb::Hammer,1));
						player.AddCrossHammer();
						msg.set_hammernum(player.GetCrossHammerNum());
						msg.set_ret(pb::Cro_Common_Suc);
					}
					else
					{
						if(it->second < 3)	//3次及以上不能购买
						{
							player.TryDeductCurrency(pb::IR_CROSS_HAMMER_COST,eSysGold,GET_BASE_DEF_INT(pb::BD_CROSS_ATTACK_CITY_PRICE));
							it->second += 1;
							player.AddCrossHammer();
							msg.set_hammernum(player.GetCrossHammerNum());
							msg.set_ret(pb::Cro_Common_Suc);
						}
						else
							msg.set_ret(pb::Cro_War_UpLimit);
					}
				}
				else
					msg.set_ret(pb::Cro_War_Cost_Lack);
			}
			else
				msg.set_ret(pb::Cro_War_Offical_Err);
			break;
		case pb::KillTroops:
			if(player.EnoughCurrency(eSysGold,GET_BASE_DEF_INT(pb::BD_CROSS_KILL_COMMOND_PRICE)))
			{
				player.TryDeductCurrency(pb::IR_CROSS_KILL_TROOP_COST,eSysGold,GET_BASE_DEF_INT(pb::BD_CROSS_KILL_COMMOND_PRICE));
				player.AddCrossDoubleKill();
				msg.set_ret(pb::Cro_Common_Suc);
				msg.set_killtimeendtime(player.GetCrossKillEndTime());
			}
			else
				msg.set_ret(pb::Cro_War_Cost_Lack);
			break;
		case pb::Shield:
			if( officeid<=5 && officeid>=1 )	//一品官员可购买
			{
				if(player.EnoughCurrency(eSysGold,GET_BASE_DEF_INT(pb::BD_CROSS_DEFENSE_SHIELD_PRICE)))
				{
					ArsenalRecMap& recByCountry = m_arsenalRec[player.GetCountryId()];
					std::map<pb::ArsenalType, uint32>::iterator it = recByCountry.find(type);
					if(it == recByCountry.end())
					{
						player.TryDeductCurrency(pb::IR_CROSS_HAMMER_COST,eSysGold,GET_BASE_DEF_INT(pb::BD_CROSS_DEFENSE_SHIELD_PRICE));
						recByCountry.insert(std::make_pair(pb::Shield,1));
						mycountry->AddShieldTime();
						msg.set_shieldendtime(mycountry->GetShieldEndTime());
						msg.set_ret(pb::Cro_Common_Suc);
					}
					else
					{
						if(it->second < 3)	//3次及以上不能购买
						{
							player.TryDeductCurrency(pb::IR_CROSS_HAMMER_COST,eSysGold,GET_BASE_DEF_INT(pb::BD_CROSS_DEFENSE_SHIELD_PRICE));
							it->second += 1;
							mycountry->AddShieldTime();
							msg.set_shieldendtime(mycountry->GetShieldEndTime());
							msg.set_ret(pb::Cro_Common_Suc);
						}
						else
							msg.set_ret(pb::Cro_War_UpLimit);
					}
				}
				else
					msg.set_ret(pb::Cro_War_Cost_Lack);
			}
			else
				msg.set_ret(pb::Cro_War_Offical_Err);
			break;
		default:
			break;
		}
	}
	else
		msg.set_ret(pb::Cro_War_State_Dismatch);

	pb::GS2C_Cross_Ammo_Info* msgone = msg.mutable_ammo_info();
	msgone->set_itemtype(type);
	msgone->set_remain_num(GetRestBuyTimes(type, player.GetCountryId()));
}

uint32 CrossArsenalMgr::GetRestBuyTimes(pb::ArsenalType type, uint32 countryid)
{
	std::map<pb::ArsenalType, uint32>::iterator it;
	ArsenalRecMap& recByCountry = m_arsenalRec[countryid];
	switch (type)
	{
	case pb::Hammer:
		it = recByCountry.find(type);
		if(it == recByCountry.end())
		{
			recByCountry.insert(std::make_pair(pb::Hammer,0));
			return 3;
		}
		else
		{
			return (3-it->second);
		}
		break;
	case pb::KillTroops:
		return 0;
		break;
	case pb::Shield:
		it = recByCountry.find(type);
		if(it == recByCountry.end())
		{
			recByCountry.insert(std::make_pair(pb::Shield,0));
			return 3;
		}
		else
		{
			return (3-it->second);
		}
		break;
	default:
		return 0;
		break;
	}
}