#include "CountryInfoTable.h"
#include "InterServer.pb.h"
//#include "Country.pb.h"
//#include "server_game\GameServer.h"
#include "City.pb.h"

//void CountryBaseTable::SaveTo(pb::CountyInfo& msg)
//{
//    msg.set_id(country_id);
//    msg.set_exp(exp);
//    msg.set_level(level);
//    msg.set_donate_times(donate_times);
//    msg.set_country_npc_level(npc_lv);
//    msg.set_country_npc_exp(npc_exp);
//
//    msg.set_open_level_up_task_time(m_OpenLevelUpTaskTime);
//    msg.set_cur_level_do_task_cnt(m_CurLevelDoTaskCnt);
//    msg.set_kill_monster_num(m_KillMonsterNum);
//    msg.set_is_monster_task_open(m_isMonsterTaskOpen > 0);
//    msg.set_is_monster_task_succeed(m_isMonsterTaskSucceed > 0);
//	SaveCrossWarInfo(*msg.mutable_cross_war_info());
//
//    pb::MonsterCityLst* lst = msg.mutable_monster_city_lst();
//    for (int i = 0; i < ARRAY_SIZE(donationMoney); ++i) {
//        pb::MonsterCity* pInfo = lst->add_list();
//        pInfo->set_donation_money(donationMoney[i]);
//        pInfo->set_friend_val(friendVal[i]);
//        pInfo->set_can_start_invade(canStartInvade[i] > 0);
//        pInfo->set_invade_cnt(invadeCnt[i]);
//        pInfo->set_invade_time(invadeTime[i]);
//    }
//}
//void CountryBaseTable::LoadFrom(const pb::CountyInfo& msg)
//{
//    country_id = msg.id();
//    exp = msg.exp();
//    level = msg.level();
//    donate_times = msg.donate_times();
//    npc_lv = msg.country_npc_level();
//    npc_exp = msg.country_npc_exp();
//
//    m_OpenLevelUpTaskTime = msg.open_level_up_task_time();
//    m_CurLevelDoTaskCnt = msg.cur_level_do_task_cnt();
//    m_KillMonsterNum = msg.kill_monster_num();
//    m_isMonsterTaskOpen = msg.is_monster_task_open();
//    m_isMonsterTaskSucceed = msg.is_monster_task_succeed();
//	UpdateCrossWarInfo(msg.cross_war_info());
//    const pb::MonsterCityLst& lst = msg.monster_city_lst();
//    for (int i = 0; i < lst.list_size(); ++i) {
//        const pb::MonsterCity& info = lst.list(i);
//        donationMoney[i] = info.donation_money();
//        friendVal[i] = info.friend_val();
//        canStartInvade[i] = info.can_start_invade();
//        invadeCnt[i] = info.invade_cnt();
//        invadeTime[i] = info.invade_time();
//    }
//}
//
//void CountryBaseTable::SaveCrossWarInfo(pb::CrossWarInfo &info )
//{
//	info.set_is_open_cross_war(isOpenedCrossWar > 0);
//	info.set_cross_war_start_time(cross_war_start_time);
//	info.set_cross_server_ip(crossServerIp);
//	info.set_cross_server_port(crossServerPort);
//	info.set_is_open_anabasis(isOpenedAnabasis > 0);
//	info.set_is_anabasis(isAnabasisSide > 0);
//	info.set_anabasis_start_time(anabasis_start_time);
//	info.set_anabasis_server_port(anabasisServerPort);
//	info.set_anabasis_server_ip(anabasisServerIp);
//	info.set_cross_state(cross_state);
//	info.set_anabasis_state(anabasis_state);
//}
//void CountryBaseTable::UpdateCrossWarInfo( const pb::CrossWarInfo&info )
//{
//	isAnabasisSide = info.is_anabasis();
//	isOpenedCrossWar = info.is_open_cross_war();
//	isOpenedAnabasis = info.is_open_anabasis();
//	crossServerIp = info.cross_server_ip();
//	crossServerPort = info.cross_server_port();
//	cross_war_start_time = info.cross_war_start_time();
//	anabasis_start_time = info.anabasis_start_time();
//	anabasisServerIp = info.anabasis_server_ip();
//	anabasisServerPort = info.anabasis_server_port();
//	cross_state = info.cross_state();
//	anabasis_state = info.anabasis_state();
//}
//
//void CountryCityTable::SaveTo(pb::Struct_City_Info& msg)
//{
//    msg.set_city_id(city_id);
//    msg.set_country(country_id);
//    msg.set_status(city_status);
//}
//void CountryCityTable::LoadFrom(const pb::Struct_City_Info& msg)
//{
//    country_id = msg.country();
//    city_id = msg.city_id();
//    city_status = msg.status();
//}
//
//void CountryLevelUpTaskPlayerTable::SaveTo(pb::CountyLevelUpTaskPlayer& msg)
//{
//    msg.set_playerid(player_id);
//    msg.set_country(country_id);
//    msg.set_rank(rank);
//    msg.set_statistic_kill(statisticKill);
//    msg.set_is_take_reward(isTakeReward > 0);
//}
//void CountryLevelUpTaskPlayerTable::LoadFrom(const pb::CountyLevelUpTaskPlayer& msg)
//{
//    player_id = msg.playerid();
//    country_id = msg.country();
//    rank = msg.rank();
//    statisticKill = msg.statistic_kill();
//    isTakeReward = msg.is_take_reward();
//}