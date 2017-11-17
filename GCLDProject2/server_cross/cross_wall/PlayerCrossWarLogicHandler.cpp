#include "object/Player.h"
#include "memory_buffer/NetPack.h"
#include "utility/MsgTool.h"
#include "Opcode.pb.h"
#include "Country.pb.h"
#include "World.pb.h"
#include "Map/WorldMap.h"
#include "Map/country/country.h"
#include "Map/city/city.h"
#include "CrossWarManager/CrossWarMgr.h"
#include "CrossWar.pb.h"
#include "ServerOpcode.pb.h"
#include "server_game/character/CharacterStorage.h"
#include "Technology/PlayerTechnology.h"
#include "Technology/Table/TechnologyTableMgr.h"
#include "CrossWorldMap.h"
#include "Map/Table/MapTableMgr.h"
#include "AutomaticUpdate/AutoUpdateCross.h"
#include "cross_wall/table/CrossTableMgr.h"
#include "reward/reward.h"
#include "CrossActivity/CrossDestroy/CrossDestroy.h"
#include "CrossActivity/CrossKill/CrossKill.h"
#include "CrossArsenal/CrossArsenalMgr.h"
#include "reward/reward.h"
#include "CrossActivity/CrossActMgr.h"
#include "InterServer.pb.h"

using namespace pb;

OpHandler(CMSG_QUIT_CROSS_WAR)
{
	if(sCrossWarMgr.GetCrossState() == State_Null) return;

	LLOG("[1002] Get CMSG_QUIT_CROSS_WAR");
	
	sCrossWorldMap.DelFromMap(*this);

	//NUll_Ret retmsg;
	//Send(SMSG_NULL_RET,retmsg);

	//玩家退出跨服世界地图
	pack.SetOpCode(SR2G_QUIT_CROSS_SERVER);
	SendToServer(pack,PACKET_TYPE_SERVER_CROSS);
}

OpHandler(CMSG_DONATION_GOODS_INFOS)
{
	GS2C_Cross_Donation_Datas msg;
	sCrossWarMgr.SaveTo(msg, GetCountryId());
	Send(SMSG_DONATION_GOODS_INFOS_RET,msg);
}

OpHandler(CMSG_DONATION_GOODS)
{
	GC2S_Donate_Goods getMsg; pack >> getMsg;

	uint32 donate_type = getMsg.item_type();

	if(donate_type == Office_Troops)		//官员令
	{
		LLOG("[1002] Cross Get CMSG_DONATION_TROOPS_ORDER");
		GS2C_DONATE_RET msg;
		msg.set_item_type(Office_Troops);
		if(sCrossWarMgr.GetCrossState() == State_Prepare)
		{
			uint32 totalnum = sCrossWarMgr.GetTotalDonateNum(GetCountryId());
			LLOG("[1002] totalnum = %d",totalnum);
			if(totalnum < GET_BASE_DEF_UINT( pb::BD_PERISH_DRAFT_UPPER_LIMIT ) * 2)
			{
				LLOG("[1002] enough coin");
				if(EnoughCurrency(eSysGold,GET_BASE_DEF_INT( pb::BD_DRAFT_COST )))
				{
					TryDeductCurrency(pb::IR_TRAIN_CHAR_COST,eSysGold,GET_BASE_DEF_INT( pb::BD_DRAFT_COST ));
					sCrossWarMgr.AddTotalDonateNum(GetCountryId());
					LLOG("[1002] cost coin ,country donate num = %d",sCrossWarMgr.GetTotalDonateNum(GetCountryId()));
					msg.set_ret(Cro_Common_Suc);
				}
				else
					msg.set_ret(Cro_War_Cost_Lack);
			}
			else
				msg.set_ret(Cro_War_Donata_Troops_Max);
		}
		else
			msg.set_ret(Cro_War_State_Dismatch);
		msg.set_count(sCrossWarMgr.GetTotalDonateNum(GetCountryId()));
		Send(SMSG_DONATION_GOODS_RET,msg);
	}
	else if(donate_type == Shadow_Troops)	//幻影
	{
		LLOG("[1002] Cross Get CMSG_DONATION_SHADOW");

		GS2C_DONATE_RET msg;
		msg.set_item_type(Shadow_Troops);
		if(sCrossWarMgr.GetCrossState() == State_Prepare)
		{
			uint32 totalnum = sCrossWarMgr.GetTotalShadowNum(GetCountryId());
			LLOG("[1002] totalnum = %d",totalnum);
			if(totalnum < GET_BASE_DEF_UINT( pb::BD_PERISH_SHADOW_UPPER_LIMIT ))
			{
				bool enoughCosts = false;
				int tmpnum = GetCurrency(eTroopsNum);
				if(tmpnum > 0)
				{
					enoughCosts = true;
					sReward.Change(*this, Reward::AddSoldierTimes, -1);
					tmpnum--;
				}
				else if(EnoughCurrency(eSysGold,GET_BASE_DEF_INT( pb::BD_SHADOW_COST )))
				{
					enoughCosts = true;
					TryDeductCurrency(pb::IR_TRAIN_CHAR_COST,eSysGold,GET_BASE_DEF_INT( pb::BD_SHADOW_COST ));
				}

				LLOG("[1002] enough coin");
				if(enoughCosts)
				{
					//科技  金币增兵时玩家获得经验
					const TecTable* tec = NULL;
					//uint32 addExp = 0;
					if(HaveTechnology(ZENGBINGQIANGHUA8))
					{
						tec = sTechnologyTableMgr.GetTable(ZENGBINGQIANGHUA8);
						AddXP(tec->Value1());
						//addExp = tec->Value1();
					}
					else if(HaveTechnology(ZENGBINGQIANGHUA7))
					{
						tec = sTechnologyTableMgr.GetTable(ZENGBINGQIANGHUA7);
						AddXP(tec->Value1());
						//addExp = tec->Value1();
					}
					else if(HaveTechnology(ZENGBINGQIANGHUA6))
					{
						tec = sTechnologyTableMgr.GetTable(ZENGBINGQIANGHUA6);
						AddXP(tec->Value1());
						//addExp = tec->Value1();
					}
					else if(HaveTechnology(ZENGBINGQIANGHUA5))
					{
						tec = sTechnologyTableMgr.GetTable(ZENGBINGQIANGHUA5);
						AddXP(tec->Value1());
						//addExp = tec->Value1();
					}
					else if(HaveTechnology(ZENGBINGQIANGHUA4))
					{
						tec = sTechnologyTableMgr.GetTable(ZENGBINGQIANGHUA4);
						AddXP(tec->Value1());
						//addExp = tec->Value1();
					}
					else if(HaveTechnology(ZENGBINGQIANGHUA3))
					{
						tec = sTechnologyTableMgr.GetTable(ZENGBINGQIANGHUA3);
						AddXP(tec->Value1());
						//addExp = tec->Value1();
					}
					else if(HaveTechnology(ZENGBINGQIANGHUA2))
					{
						tec = sTechnologyTableMgr.GetTable(ZENGBINGQIANGHUA2);
						AddXP(tec->Value1());
						//addExp = tec->Value1();
					}
					else if(HaveTechnology(ZENGBINGQIANGHUA1))
					{
						tec = sTechnologyTableMgr.GetTable(ZENGBINGQIANGHUA1);
						AddXP(tec->Value1());
						//addExp = tec->Value1();
					}

					Character* ptr = m_characterStorage->GetMaxLvHero();
					if(ptr)
					{
						sCrossWarMgr.AddShadows(ptr->CreateShadow());
					}

					LLOG("[1002] cost coin ,country donate num = %d",sCrossWarMgr.GetTotalShadowNum(GetCountryId()));
					msg.set_ret(Cro_Common_Suc);
				}
				else
					msg.set_ret(Cro_War_Cost_Lack);
			}
			else
				msg.set_ret(Cro_War_Donata_Shadows_Max);
		}
		else
			msg.set_ret(Cro_War_State_Dismatch);
		msg.set_count(sCrossWarMgr.GetTotalShadowNum(GetCountryId()));
		Send(SMSG_DONATION_GOODS_RET,msg);
	}
}

OpHandler(CMSG_CROSS_ATTACK_CAPITAL)
{
	C2CR_Attack_Capital getMsg; pack >> getMsg;

	GS2C_Attack_Result sendmsg;
	sendmsg.set_ret(Cro_Common_Err);

	uint32 mycountryid = GetCountryId();
	Country* mycountry = sCrossWorldMap.GetCountry(mycountryid);
	if(mycountry)
	{
		if(sCrossWarMgr.GetCrossState() == pb::State_Start)	//是开战阶段
		{
			uint32 heroid = getMsg.hero_id();
			Character* charOne = m_characterStorage->MutableCharacter(heroid);
			if(charOne)
			{
				if(!charOne->isFighting)	//武将未上阵
				{
					if(charOne->GetCurCity() == getMsg.hero_in_city())		//武将所在城池信息正确
					{
						City* mycity = sCrossWorldMap.GetCity(getMsg.hero_in_city());	//武将所在城池
						if(mycity)
						{
							//判断两个城池是否相连
							if (const MapCity* table = sMapTableMgr.GetMapCity(getMsg.hero_in_city()))
							{
								std::vector<int>::const_iterator it = std::find(
									table->m_linkCity.begin(),
									table->m_linkCity.end(),
									getMsg.dest_city_id());
								if (it == table->m_linkCity.end())		//不相连，返回
								{
									sendmsg.set_ret(Cro_War_City_Not_Link);
									Send(SMSG_CROSS_ATTACK_RET,sendmsg);
									return;
								}
							}
							else
							{
								Send(SMSG_CROSS_ATTACK_RET,sendmsg);
								return;
							}

							//是相连的城池
							City* destcity = sCrossWorldMap.GetCity(getMsg.dest_city_id());
							if(destcity)
							{
								if(destcity->IsCapitalCity() && destcity->GetCountryId() != mycountryid)	//自己的国家与准备攻击的国家不同，则可以攻击
								{
									if(destcity->m_durability > 0)		//有耐久度
									{
										if(mycity->IsFighting())	//在战斗中，只能突进用
										{
											uint32 forcetimes = 0;
											if(charOne->m_player->HaveTechnology(TUJING_QIANGHUA))//科技   //玩家兵力满足2倍于对方的状态
											{
												const TecTable* tec = sTechnologyTableMgr.GetTable(TUJING_QIANGHUA);
												if(tec)
													forcetimes = tec->Value1();
												else
													forcetimes = GET_BASE_DEF_UINT(pb::BD_SUDDEN_NEED_TROOPS_MULTIPLE);
											}
											else
												forcetimes = GET_BASE_DEF_UINT(pb::BD_SUDDEN_NEED_TROOPS_MULTIPLE);

											if(sCrossWorldMap.GetCity(charOne->GetCurCity())->isRushLegal(true,forcetimes))		//玩家兵力满足
											{
												charOne->_ExitCity(*mycity);
												charOne->_EnterCity(*sCrossWorldMap.GetCity(mycountry->m_capitalCityId));  

												//对方首都扣除耐久度
												
												destcity->ReduceDurability(1,charOne->GetCountryId());

												if(Country* countryDest = sCrossWorldMap.GetCountry(destcity->GetCountryId()))
												{
													if(countryDest->IsCrossCountryOccypied())	//灭国
													{
														CrossStarRec recOne;
														recOne.desCountryId = countryDest->id;

														mycountry->m_crossStarRec.push_back(recOne);

														//广播星数
														Country_Stars_Update updatestarmsg;
														for(int i=1; i<4; i++)
														{
															sCrossWarMgr.SaveTo(*updatestarmsg.add_update_stars(), i);
														}
														sCrossWorldMap.Broadcast(SMSG_CROSS_UPDATE_STAR,updatestarmsg);
													}
												}
												sendmsg.set_hammernum(GetCrossHammerNum());
												sendmsg.set_ret(Cro_Common_Suc);
											}
											else
												sendmsg.set_ret(Cro_War_Cant_Rush);
										}
										else		//没有战斗，直接攻击
										{
											charOne->_ExitCity(*mycity);
											charOne->_EnterCity(*sCrossWorldMap.GetCity(mycountry->m_capitalCityId));  

											destcity->ReduceDurability(1,charOne->GetCountryId());
											if(Country* countryDest = sCrossWorldMap.GetCountry(destcity->GetCountryId()))
											{
												if(countryDest->IsCrossCountryOccypied())	//灭国
												{
													CrossStarRec recOne;
													recOne.desCountryId = countryDest->id;

													mycountry->m_crossStarRec.push_back(recOne);

													//广播星数
													Country_Stars_Update updatestarmsg;
													for(int i=1; i<4; i++)
													{
														sCrossWarMgr.SaveTo(*updatestarmsg.add_update_stars(), i);
													}
													sCrossWorldMap.Broadcast(SMSG_CROSS_UPDATE_STAR,updatestarmsg);
												}
											}
											sendmsg.set_hammernum(GetCrossHammerNum());
											sendmsg.set_ret(Cro_Common_Suc);
										}
									}
									else
										sendmsg.set_ret(Cro_War_Destroy);
								}
								else
									sendmsg.set_ret(Cro_War_Is_Same_Country);
							}
						}
					}
					else
						sendmsg.set_ret(Cro_War_Hero_Err);
				}
				else
					sendmsg.set_ret(Cro_War_Hero_Err);
			}
			else
				sendmsg.set_ret(Cro_War_Hero_Err);
		}
		else
			sendmsg.set_ret(Cro_War_State_Dismatch);
	}
	Send(SMSG_CROSS_ATTACK_RET,sendmsg);
}

OpHandler(CMSG_CROSS_USE_TROOPS)
{
	C2GS_Use_Cross_Troops msg; pack >> msg;

	uint32 destCityId = msg.destcityid();
	LLOG("CMSG_CROSS_USE_TROOPS destCityId=%d",destCityId);

	GS2C_Combat_Call_Up sendMsg;
	if(GetPlaceOfficeId() > 0 && GetPlaceOfficeId() <= 9)	//子爵及以上
	{
		if(sCrossWarMgr.GetTotalDonateNum(GetCountryId()) > 0)
		{
			bool isHeroHere = m_characterStorage->HasCharacterInCity(destCityId);
			if(isHeroHere)
			{	
				if (City* pCity = sWorldMap.GetCity(destCityId))
				{
					if(!pCity->HaveStatus(City::CallUp))
					{
						uint64 lasttime = sOS.TimeSeconds();

						uint64 callendtime = lasttime + GET_BASE_DEF_INT(pb::BD_DRAFT_MAX_LAST_TIME) * 60;
						uint64 uniquecallid = lasttime << 16 | destCityId;
						pCity->AddCallUpCountry(GetCountryId(),callendtime,uniquecallid);
						pCity->SetStatus(City::CallUp, true);
						sCrossWarMgr.UseTroops(GetCountryId());
						LLOG("call up success !!!!");
						// 5min
						sAutoUpdateCross.AddEvent(
							boost::bind(&City::CallUpEndCallBack, pCity, uniquecallid),
							GET_BASE_DEF_INT(pb::BD_DRAFT_MAX_LAST_TIME) * 60/* 5*60*/
							);
						sendMsg.set_result(pb::SUCCESS);
					}
					else
						sendMsg.set_result(pb::CITY_STATE_ERR);	
				}
				else
					sendMsg.set_result(pb::CITY_NOT_FOUND1);
			}
			else
				sendMsg.set_result(pb::NO_HERO_HERE);
		}
		else
			sendMsg.set_result(pb::NO_ENOUGH_TROOPS);
	}
	else
		sendMsg.set_result(pb::CROSS_CALL_OFFICE_ERR);
	
	Send(SMSG_CROSS_USE_TROOPS_RET,sendMsg);
}

OpHandler(CMSG_CROSS_TAKE_WAR_REWARD)
{
	GS2C_TakeStarAwardRet sendmsg;
	sendmsg.set_ret(Cro_Common_Err);

	uint32 mycountryid = GetCountryId();
	Country* mycountry = sCrossWorldMap.GetCountry(mycountryid);
	if(mycountry)
	{
		if(sCrossWarMgr.GetCrossState() == pb::State_Award)	//是领奖阶段
		{
			CrossPlayerMap::iterator it = mycountry->m_CrossPlayerLst.find(GetGuid());
			if(it != mycountry->m_CrossPlayerLst.end())		//有击杀记录
			{
				CrossPlayer& crossPlayer = it->second;
				if(!crossPlayer.isTakeReward)	//没有领取
				{
					int mylevel = GetLevel();
					if(const Table_Cross_One* tableone = sCrossTableMgr.GetPQuestByLevel(sCrossActMgr.GetPlayerTaskLevel(*this)))
					{
						SR2G_AwardTakePlayer msg; 
						msg.set_playerid(GetGuid());
						if(!mycountry->IsCrossCountryOccypied())	//防守成功
						{
							for(IntPairVec::const_iterator it = tableone->m_DefenceRewards.begin(); it != tableone->m_DefenceRewards.end(); it++)
							{
								pb::PairValue* pairvalue = msg.add_rewards();
								pairvalue->set_first(it->first);
								pairvalue->set_second(it->second);
							}
						}

						if(uint32 starNum = mycountry->m_crossStarRec.size())	//星数奖励
						{
							for(IntPairVec::const_iterator ite = tableone->m_StarRewards[starNum-1].begin(); ite != tableone->m_StarRewards[starNum-1].end(); ite++)
							{
								pb::PairValue* pairvalue = msg.add_rewards();
								pairvalue->set_first(ite->first);
								pairvalue->set_second(ite->second);
							}
						}
						SendToServer(pb::SR2G_Award_Take_Player,msg,PACKET_TYPE_SERVER_CROSS);
						crossPlayer.isTakeReward = true;
						sendmsg.set_ret(Cro_Common_Suc);
					}
				}
				else
					sendmsg.set_ret(Cro_War_Reward_IsTake);
			}
			else
				sendmsg.set_ret(Cro_war_Player_Not_In_Lst);
		}
		else
			sendmsg.set_ret(Cro_War_State_Dismatch);
	}
	Send(SMSG_CROSS_TAKE_WAR_REWARD_RET,sendmsg);
}

OpHandler(CMSG_CROSS_COUNTRY_INFO)
{
	GS2L_Devide_Country_Infos sendmsg;
	if(sCrossWarMgr.GetCrossState() == pb::State_Preview || sCrossWarMgr.GetCrossState() == pb::State_Start)	//战斗阶段或者预览阶段
	{
		for(int i=1; i<=NEURRAL_COUNTRY_ID; i++)
		{
			if(i == MONSTER_COUNTRY_ID) continue;
			if(Country* country = sCrossWorldMap.GetCountry(i))
			{
				Devide_Country_Info* countryone = sendmsg.add_countrys();
				countryone->set_orig_country_id(country->OriGameCountryId);
				countryone->set_devide_country_id(i);
				countryone->set_unique_index_id(country->cross_uniqueID);
				countryone->set_pros_degree(country->m_Prosperity_Degree);
				countryone->set_country_durable(country->GetCrossCountryDurability());
				countryone->set_server_id(country->OriGameServerId);
			}
		}
	}
	Send(SMSG_CROSS_COUNTRY_INFO_RET,sendmsg);
}

OpHandler(CMSG_CROSS_WAR_STATE)
{
	GS2C_Notice_CrossWar_State sendmsg;
	sendmsg.set_curstate(sCrossWarMgr.m_cState);
	sendmsg.set_stateendtime(sCrossWarMgr.m_state_end_time);
	Send(SMSG_NOTICE_CROSS_START,sendmsg);
}

//请求灭国任务
OpHandler(CMSG_CROSS_ALL_DESTROY_TASK_INFO)
{
	GS2C_Cross_Destroy_Task_Infos msg;
	if(sCrossWarMgr.GetCrossState() == pb::State_Start)
	{
		sCrossDestroy.SaveTo(*this,msg);
	}
	Send(SMSG_CROSS_ALL_DESTROY_TASK_INFO_RET,msg);
}

OpHandler(CMSG_CROSS_TAKE_DESTROY_TASK_AWARD)
{
	GC2S_Take_Destroy_Task_Award msg; pack >> msg;
	GS2C_Take_Destroy_Task_Award_Ret sendmsg;
	sCrossDestroy.TakeAwards(*this,msg.task_id(),sendmsg);
	Send(SMSG_CROSS_TAKE_DESTROY_TASK_AWARD_RET,sendmsg);
}

OpHandler(CMSG_CROSS_KILL_TASK_INFO)
{
	GS2C_Cross_Kill_Task_Info msg;
	if(sCrossWarMgr.GetCrossState() == pb::State_Start)
	{
		sCrossKill.SaveTo(*this,msg);
	}
	Send(SMSG_CROSS_KILL_TASK_INFO_RET,msg);
}

OpHandler(CMSG_CROSS_TAKE_KILL_TASK_AWARD)
{
	GS2C_Cross_Kill_Task_Info msg; pack >> msg;
	GS2C_Take_Kill_Task_Award_Ret sendmsg;
	sCrossKill.TakeAwards(*this,msg.task_id(),sendmsg);
	Send(SMSG_CROSS_TAKE_KILL_TASK_AWARD_RET,sendmsg);
}

OpHandler(CMSG_CROSS_ALL_AMMO_INFO)
{
	GS2C_Cross_Ammo_Infos msg;
	sCrossArsenalMgr.SaveTo(msg, GetCountryId());
	Send(SMSG_CROSS_ALL_AMMO_INFO_RET, msg);
}

OpHandler(CMSG_CROSS_BUY_AMMO)
{
	GC2S_Buy_Ammo msg; pack >> msg;
	GS2C_Buy_Ammo_Rsp sendmsg;
	sendmsg.set_playerid(GetGuid());
	sCrossArsenalMgr.BuyArsenalItems(*this,msg.itemtype(),sendmsg);
	//if(msg.itemtype() == pb::Shield)
	//	sCrossWorldMap.Broadcast(SMSG_CROSS_BUY_AMMO_RET,sendmsg);
	//else
	//	Send(SMSG_CROSS_BUY_AMMO_RET, sendmsg);

	if(sendmsg.ret() == pb::Cro_Common_Suc)
		sCrossWorldMap.BroadcastInCountry(SMSG_CROSS_BUY_AMMO_RET,sendmsg,GetCountryId());
	else
		Send(SMSG_CROSS_BUY_AMMO_RET, sendmsg);
}

OpHandler(CMSG_CROSS_KILL_ENEMY_RANK)
{
	GS2C_Cross_Rank_Info msg;
	msg.set_rank_num(sCrossWarMgr.GetCrossKillRank(GetGuid(),GetCountryId()));
	Send(SMSG_CROSS_KILL_ENEMY_RANK_RET,msg);
}

OpHandler(CMSG_CROSS_USE_ATTACK_HUMMER)
{
	GS2C_Use_Attack_Hummer msg; pack>>msg;
	GS2C_Attack_Result sendmsg;
	sendmsg.set_ret(Cro_Common_Err);
	if(sCrossWarMgr.GetCrossState() == pb::State_Start)	//战斗阶段或者预览阶段
	{
		if(Country* mycountry = sCrossWorldMap.GetCountry(GetCountryId()))
		{
			if(!mycountry->IsCrossCountryOccypied())		//自己国家没有被占领
			{
				if(GetCrossHammerNum()>0)	//有攻城锤
				{
					uint32 desCountryid = msg.country_id();
					uint32 heroid = msg.heroid();
					Character* charOne = m_characterStorage->MutableCharacter(heroid);
					if(charOne)
					{
						City* mycity = sCrossWorldMap.GetCity(charOne->GetCurCity());	//武将所在城池
						if(mycity)
						{
							if(desCountryid != GetCountryId())
							{
								if(Country* desCountry = sCrossWorldMap.GetCountry(desCountryid))
								{
									if(!desCountry->IsCrossCountryOccypied())	//目标国家没有被占领
									{
										uint32 capCityid = sMapTableMgr.GetCapitalCity(desCountryid,1);
										if(City* capCity = sCrossWorldMap.GetCity(capCityid))
										{
											//判断两个城池是否相连
											if (const MapCity* table = sMapTableMgr.GetMapCity(charOne->GetCurCity()))
											{
												std::vector<int>::const_iterator it = std::find(
													table->m_linkCity.begin(),
													table->m_linkCity.end(),
													capCityid);
												if (it == table->m_linkCity.end())		//不相连，返回
												{
													return;
												}
											}
											else
												return;

											capCity->ReduceDurability(20,GetCountryId());
											UseCrossHammer();
											sendmsg.set_hammernum(GetCrossHammerNum());
											sendmsg.set_ret(Cro_Common_Suc);
										

											if(desCountry->IsCrossCountryOccypied())	//灭国
											{
												CrossStarRec recOne;
												recOne.desCountryId = desCountryid;

												mycountry->m_crossStarRec.push_back(recOne);

												//广播星数
												Country_Stars_Update updatestarmsg;
												for(int i=1; i<4; i++)
												{
													sCrossWarMgr.SaveTo(*updatestarmsg.add_update_stars(), i);
												}
												sCrossWorldMap.Broadcast(SMSG_CROSS_UPDATE_STAR,updatestarmsg);
											}
										}
									}
									else
										sendmsg.set_ret(Cro_War_Destroy);
								}
							}
							else
								sendmsg.set_ret(Cro_War_Is_Same_Country);
						}
					}
				}
				else
					sendmsg.set_ret(Cro_War_No_Hammer);
			}
			else
				sendmsg.set_ret(Cro_War_Destroy);
		}
	}
	else
		sendmsg.set_ret(Cro_War_State_Dismatch);
	Send(SMSG_CROSS_ATTACK_RET,sendmsg);
}