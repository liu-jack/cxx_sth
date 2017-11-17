#include "PlayerAutoCountryWarLogic.h"
#include "OS.h"
#include "System.h"
#include "Opcode.pb.h"
#include "BaseDefineMgr.h"
#include "BaseDefine.pb.h"
#include "AutoCountryWar.pb.h"
#include "Map/city/city.h"
#include "Map/WorldMap.h"
#include "Technology/PlayerTechnology.h"
#include "Enum.pb.h"
#include "object/PlayerValueOwner.h"
#include "common/def/ObjectDefines.h"
#include "object/Player.h"
#include "character/Character.h"
#include "character/CharacterStorage.h"
#include "Dungeon.pb.h"
#include "Building/BuildingLogMgr.h"
#include "CrossLogic/PlayerCharacterLogic.h"
#include "reward/reward.h"
#include "Map/PlayerMapLogic.h"
#include "Map/country/country.h"
#include "BaseDefine.pb.h"
#include "BaseDefineMgr.h"

#ifdef _MMO_SERVER_
#include "AutomaticUpdate/AutoUpdateGame.h"
#endif

PlayerAutoCountryWarLogic::PlayerAutoCountryWarLogic()
	: m_isAutoCountryWar(false)
	, m_isUseCoinConscript(false)
	, m_isAutoCallUp(false)
	, m_desCityId(0)
	, m_callUpCity(0)
	, m_totalCityOccuty(0)
	, m_totalExpGet(0)
	, m_totalKillCount(0)
{
	m_herolst.clear();
}

bool PlayerAutoCountryWarLogic::Init()
{
	return true;
}

void PlayerAutoCountryWarLogic::SaveTo(pb::GS2C_Auto_Country_War_Info& msg,bool isShutDown/* = false */)
{
	msg.set_is_begin(GetAutoCountryWar());
	msg.set_isshutdown(isShutDown);
	pb::Struct_Auto_Country_War* structAuto = msg.mutable_detaildata();
	for(std::set<Character*>::iterator it = m_herolst.begin(); it != m_herolst.end(); ++it)
	{
		Character* charone = *it;
		structAuto->add_heroid(charone->GetID());
	}
	structAuto->set_end_time(m_autoWarEndTime);
	structAuto->set_killcount(GetKillCount());
	structAuto->set_expcount(GetExpGet());
	structAuto->set_occupycount(GetCityOccupy());
	structAuto->set_target_city(GetDesCityId());
}

void PlayerAutoCountryWarLogic::Reset()
{
	m_isAutoCountryWar = false;
	m_isUseCoinConscript = false;
	m_isAutoCallUp = false;
	m_desCityId = 0;
	m_callUpCity = 0;
	m_totalCityOccuty = 0;
	m_totalExpGet = 0;
	m_totalKillCount = 0;
	m_herolst.clear();
	myCountry = NULL;
}

void PlayerAutoCountryWarLogic::Update(Player* player, uint64 timeMS)
{
	if(GetAutoCountryWar())		//如果有自动国战
	{
		if(timeMS/1000 >= m_autoWarEndTime)		//自动国战超时
		{
			//Reset();
			Stop();
			return;
		}

		uint32 hpSpeed = player->m_buildingLogMgr->GetModuleGainValue(MODULE_BINGYING);

		//检查每一个自动国战武将
		for(std::set<Character*>::iterator it = m_herolst.begin(); it != m_herolst.end(); ++it)
		{
			Character* charone = *it;
			if(charone->GetIsInAutoCountryWar())	//在自动国战中
			{
				//如果武将不在战斗中且不在监狱（说明武将暂时是空闲状态）
				if(!charone->isInCombat && !charone->isInPrison())
				{
					//如果有征召城池，且该武将血量不为0，则传送到征召城池，关闭征召城池
					if(GetCallUpCity() != 0 && charone->CurHp() != 0)
					{
						//TODO: 是否需要预先判断可否征召

						//获得征召城池
						if(City* callCity = sWorldMap.GetCity(GetCallUpCity()))
						{
							//征召城池中的征召有该征召
							if(callCity->IsCallUpTokenInLst(GetCallUpId()))
							{
								//武将不在该城中，则征召成功
								if(charone->GetCurCity() != GetCallUpCity())
								{
									charone->_ExitCity(*sWorldMap.GetCity(charone->GetCurCity()));
									charone->_EnterCity(*callCity);
									charone->initPath();

									//TODO: 停止武将自动国战移动

									//发放征召奖励
									sReward.Change(*player, Reward::Food, GET_BASE_DEF_INT(pb::BD_ACCEPT_DRAFT_REWARD_FOOD));
									player->AddExploitValue(GET_BASE_DEF_INT(pb::BD_ACCEPT_DRAFT_REWARD_EXPLOIT));

									PlayerCallRec recone;
									recone.playerid = player->GetGuid();
									recone.uniqueid = GetCallUpId();
									myCountry->m_player_call_rec.push_back(recone);

									SetCallUpCity(0,0);
								}
							}
							else
								SetCallUpCity(0,0);
						}
						else
							SetCallUpCity(0,0);
					}

					//如果武将停止自动移动，则重新确定路线
					if(!charone->_GetHeroCanMove())
					{
						//如果武将在基地，则先自动回血，如果血满了则确定路线
						if(charone->GetCurCity() == myCountry->m_capitalCityId)
						{
							uint32 curNeedHp = charone->NeedHp();
							if(curNeedHp > 0 && hpSpeed > 0)	//不满血，恢复满兵力	满兵力后在update中重新确定路线
							{
								uint32 hpSpeed = player->m_buildingLogMgr->GetModuleGainValue(MODULE_BINGYING);
								PlayerCharacterLogic::TryToRecoverHp(player,charone,hpSpeed,GetUseCoinConscript());
							}

							curNeedHp = charone->NeedHp();
							if(curNeedHp == 0)	//血满了，重新确定路线
							{
								charone->AutoMoveTo(m_desCityId);
							}
						}
						else	//如果武将不在基地，则不回血，直接确立路线
						{
							if(!charone->IsDead())
							{
								charone->AutoMoveTo(m_desCityId);
							}

						}
					}
				}
			}
		}
	}
}

void PlayerAutoCountryWarLogic::Stop(bool isShutDown /*= false*/)
{
	SetAutoCountryWar(false);

	for(std::set<Character*>::iterator it = m_herolst.begin(); it != m_herolst.end(); ++it)
	{
		Character* charone = *it;
		charone->SetIsInAutoCountryWar(false);
		charone->initPath();	//停止移动
	}
	m_autoWarEndTime = 0;

	if(m_player)
	{
		pb::GS2C_Auto_Country_War_Info sendmsg;
		SaveTo(sendmsg,isShutDown);
		m_player->Send(pb::SMSG_GET_AUTO_COUNTRY_WAR_DATA, sendmsg);
	}
}

void PlayerAutoCountryWarLogic::QuickTraining(Player* player, pb::C2GS_Quick_Training& msg, pb::GS2C_QuickTraining_Ret& sendmsg)
{
	uint32 trainid = msg.trainid();
	uint64 timeNow = sOS.TimeMS();
	sendmsg.set_ret(pb::AC_Success);
	//设置练兵令
	if(trainid > 0)
	{
		bool trainSuc = false;
		int costgold = GET_BASE_DEF_INT( pb::BD_ACCELERATE_TRAINING_COST) * msg.heroid_size();
		if(player->EnoughCurrency(eSysGold,costgold))	//有足够的金币
		{
			switch(trainid)
			{
			case 1:
				if(sReward.Change(*player, Reward::TrainToken_Normal, -msg.heroid_size()))
				{
					player->TryDeductCurrency(pb::IR_TRAIN_CHAR_COST,eSysGold,costgold);
					trainSuc = true;
				}
				break;
			case 2:
				if(sReward.Change(*player, Reward::TrainToken_Fight, -msg.heroid_size()))
				{
					player->TryDeductCurrency(pb::IR_TRAIN_CHAR_COST,eSysGold,costgold);
					trainSuc = true;
				}
				break;
			case 3:
				if(sReward.Change(*player, Reward::TrainToken_Double, -msg.heroid_size()))
				{
					player->TryDeductCurrency(pb::IR_TRAIN_CHAR_COST,eSysGold,costgold);
					trainSuc = true;
				}
				break;
			case 4:
				if(sReward.Change(*player, Reward::TrainToken_Reduce, -msg.heroid_size()))
				{
					player->TryDeductCurrency(pb::IR_TRAIN_CHAR_COST,eSysGold,costgold);
					trainSuc = true;
				}
				break;
			default:
				break;
			}
		}

		if(!trainSuc)	//使用练兵失败，函数不执行
		{
			sendmsg.set_ret(pb::AC_No_Resource);
			return;
		}

		player->m_worldMapLogic->m_trainingEndTime = timeNow;
		player->UpdateAllDailyQuestInfo(TUNTIAN_TRAINING,1,0);//每日任务
#ifdef _MMO_SERVER_
		int lasttime = GET_BASE_DEF_INT(pb::BD_TRAINING_BUFF_LAST_TIME);
		sAutoUpdateGame.AddEvent(boost::bind(&PlayerMapLogic::AdditionUpEnd, player->m_worldMapLogic.get(), player, timeNow),lasttime * 60);
#endif
		
		for(int i=0; i <msg.heroid_size(); i++)
		{
			uint32 heroid = msg.heroid(i);
			if(Character* charone = player->m_characterStorage->MutableCharacter(heroid))
			{
				////武将在空闲状态 （不在战斗中，不在地牢中，不在训练中）
				//if(!charone->isInCombat && !charone->isInPrison() && !charone->isInTraining )
				//{
					int expUpFactor = GET_BASE_DEF_INT(pb::BD_TRAINING_GET_EXP_FACTOR);		//经验提升系数
					//增加武将经验
					int expadd = (int)charone->GetLevel() * (int)(expUpFactor/100);
					charone->TryAddXP(expadd);
					charone->resetTrainAdditionUp();
					charone->expPercent = 50;
					switch(trainid)
					{
					case 1:
						break;
					case 2:
						charone->fightPercent = 50;
						break;
					case 3:
						charone->doubleKill = 100;
						break;
					case 4:
						charone->reduceCostHP = 50;
						break;
					}
				//}
			}
		}
	}
	else
		sendmsg.set_ret(pb::AC_Fail);
}

void PlayerAutoCountryWarLogic::StartAutoCountryWar(Player* player, pb::C2GS_Start_Auto_Country_War& msg, pb::GS2C_Start_AC_Ret& sendmsg)
{
	m_player = player;
	sendmsg.set_ret(pb::AC_Success);
	if(GetAutoCountryWar())	//在自动战斗
	{
		sendmsg.set_ret(pb::AC_IS_IN_AC);
		return;
	}

	uint32 descityid = msg.cityid();	//目标城池
	//uint32 trainid = msg.trainid();		//练兵令id
	bool useCoinConscript = msg.isusegoldaddsoldier();	//是否使用钻石征兵
	bool autoCallUp = msg.isautocallup();				//是否自动征召
	//uint64 timeNow = sOS.TimeSeconds();
	City* descity = sWorldMap.GetCity(descityid);
	if(!descity)
	{
		sendmsg.set_ret(pb::AC_DES_CITY_ERR);
		return;
	}

	if((descity->GetCountryId() == player->GetCountryId()) && !descity->IsFighting())	//是自己的城池，且城池没有战斗，则不能自动打
	{
		sendmsg.set_ret(pb::AC_DES_CITY_ERR);
		return;
	}

	if(descity->IsTonden() || descity->IsCapitalCity())		//屯田区与首都不允许作为目标城池
	{
		sendmsg.set_ret(pb::AC_DES_CITY_ERR);
		return;
	}

	if(msg.heroid_size() == 0)	//传来的参数不合法
	{
		sendmsg.set_ret(pb::AC_HERO_ERR);
		return;
	}

	uint32 costGas = 0;
	uint64 lastTime = 0;
	if(!player->HaveTechnology(ZHANGETONGKUAI))
		costGas = GET_BASE_DEF_UINT(pb::BD_WORLD_AUTOFIGHT_CONSUME);

	if(player->TestFlag(pb::PLAYER_UP_COUNTRY_WAR_TIME))
		lastTime = GET_BASE_DEF_UINT(pb::BD_WORLD_AUTOFIGHT_EXTIME);
	else
		lastTime = GET_BASE_DEF_UINT(pb::BD_WORLD_AUTOFIGHT_TIME);//1800 seconds

	if(player->EnoughCurrency(eFood,costGas))	//有足够的气矿
	{
		Reset();	//确认可以自动国战，则先清除数据

		player->TryDeductCurrency(pb::IR_AUTO_COUNTRY_WAR_COST,eFood,costGas);
		SetAutoCountryWar(true);
		m_autoWarEndTime = sOS.TimeSeconds() + lastTime;
		SetUseCoinConscript(useCoinConscript);
		SetAutoCallUp(autoCallUp);
		SetDesCityId(descityid);		
		myCountry = sWorldMap.GetCountry(player->GetCountryId());

		if(descity->GetCountryId() != player->GetCountryId())	//不是自己国家，则进攻
			SetIsAttackGoal(true);
		else
			SetIsAttackGoal(false);

		//兵营恢复速度
		uint32 hpSpeed = player->m_buildingLogMgr->GetModuleGainValue(MODULE_BINGYING);

		for(int i=0; i <msg.heroid_size(); i++)
		{
			uint32 heroid = msg.heroid(i);
			if(Character* charone = player->m_characterStorage->MutableCharacter(heroid))
			{
				//武将在空闲状态 （不在战斗中，不在地牢中，不在训练中）
				if(!charone->isInCombat && !charone->isInPrison() && !charone->isInTraining )
				{
					m_herolst.insert(charone);
					charone->SetIsInAutoCountryWar(true);
					charone->initPath();	//停止移动

					uint32 curNeedHp = charone->NeedHp();
					if(curNeedHp > 0 && hpSpeed > 0)	//不满血，恢复满兵力	满兵力后在update中重新确定路线
					{
						uint32 hpSpeed = player->m_buildingLogMgr->GetModuleGainValue(MODULE_BINGYING);
						PlayerCharacterLogic::TryToRecoverHp(player,charone,hpSpeed,GetUseCoinConscript());
					}
				}
			}
		}
	}
	else
		sendmsg.set_ret(pb::AC_No_Resource);
}


void PlayerAutoCountryWarLogic::ChechAutoWarEnd(uint32 belongCountry)
{
	if(!GetAutoCountryWar()) return;

	if(GetIsAttackGoal())	//是攻击城池
	{
		if(belongCountry == myCountry->id)	//已经是自己国家的城池了，则任务结束
			Stop();
	}
	else		//是防守城池，战斗结束则结束自动国战
	{
		Stop();
	}
}

void PlayerAutoCountryWarLogic::ReautoHero(Player* player, pb::C2GS_Reauto_Country_War& msg, pb::GS2C_REAUTO_HERO_Ret& sendmsg)
{
	sendmsg.set_ret(pb::AC_Success);
	if(!GetAutoCountryWar()) 
	{
		sendmsg.set_ret(pb::AC_IS_NOT_IN_AC);
		return;
	}

	uint32 heroid = msg.heroid();
	bool isJoin = msg.isjoinauto();

	if(isJoin)
	{
		if(Character* charone = player->m_characterStorage->MutableCharacter(heroid))
		{
			//武将在空闲状态 （不在战斗中，不在地牢中，不在训练中）
			if(!charone->isInCombat && !charone->isInPrison() && !charone->isInTraining )
			{
				m_herolst.insert(charone);
				charone->SetIsInAutoCountryWar(true);
				charone->initPath();	//停止移动

				uint32 curNeedHp = charone->NeedHp();
				uint32 hpSpeed = player->m_buildingLogMgr->GetModuleGainValue(MODULE_BINGYING);
				if(curNeedHp > 0 && hpSpeed > 0)	//不满血，恢复满兵力	满兵力后在update中重新确定路线
				{
					uint32 hpSpeed = player->m_buildingLogMgr->GetModuleGainValue(MODULE_BINGYING);
					PlayerCharacterLogic::TryToRecoverHp(player,charone,hpSpeed,GetUseCoinConscript());
				}
			}
			else
				sendmsg.set_ret(pb::AC_HERO_ERR);
		}
		else
			sendmsg.set_ret(pb::AC_Fail);
	}
	else
	{
		if(Character* charone = player->m_characterStorage->MutableCharacter(heroid))
		{
			m_herolst.erase(charone);
		}
		else
			sendmsg.set_ret(pb::AC_Fail);
	}
}

void PlayerAutoCountryWarLogic::SetDesCity(Player* player,pb::C2GS_Change_Target_City& msg, pb::GS2C_Change_Des_Ret& sendmsg)
{
	sendmsg.set_ret(pb::AC_Success);
	if(!GetAutoCountryWar()) 
	{
		sendmsg.set_ret(pb::AC_IS_NOT_IN_AC);
		return;
	}

	uint32 desid = msg.cityid();
	City* descity = sWorldMap.GetCity(desid);
	if(!descity)
	{
		sendmsg.set_ret(pb::AC_DES_CITY_ERR);
		return;
	}

	if((descity->GetCountryId() == player->GetCountryId()) && !descity->IsFighting())	//是自己的城池，且城池没有战斗，则不能自动打
	{
		sendmsg.set_ret(pb::AC_DES_CITY_ERR);
		return;
	}

	if(descity->IsTonden() || descity->IsCapitalCity())		//屯田区与首都不允许作为目标城池
	{
		sendmsg.set_ret(pb::AC_DES_CITY_ERR);
		return;
	}

	SetDesCityId(desid);	
	if(descity->GetCountryId() != player->GetCountryId())	//不是自己国家，则进攻
		SetIsAttackGoal(true);
	else
		SetIsAttackGoal(false);
}