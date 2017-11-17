#include "PalaceMgr.h"
#include "Table/PalaceTableMgr.h"
#include "../object/Player.h"
#include "../Combat/CombatGroup.h"
#include "../Npc/NpcMgr.h"
#include "../Npc/Npc.h"
#include "../character/CharacterStorage.h"
#include "Combat.pb.h"
#include "Opcode.pb.h"
#include "Palace.pb.h"
#include "../reward/reward.h"
#include "../session/PlayerPool.h"
#include "../CrossLogic/PalaceDB.h"
#include "../object/PlayerValueOwner.h"
#include "System.h"  
//#include "AutomaticUpdate/AutomaticUpdate.h"
#include "Technology/Table/TechnologyTableMgr.h"
#include "Technology/PlayerTechnology.h"
#include "Map/country/country.h"
#include "Map/WorldMap.h"
#include "ServerOpcode.pb.h"
#include "memory_buffer/NetPack.h"
#include "utility/Utility.h"
#include "PalaceAchievementLog.h"

const int Palace_Trigger_Iron_Add = 23;      //��������ӳ�
static const int Palace_Trigger_Iron_Salary = 24;   //�滻������ٺ»

PalaceMgr::PalaceMgr()
{
    for (PalaceTableMgr::ProtoMap::iterator it = sPalaceTableMgr.m_ProtoTable.begin();
         it != sPalaceTableMgr.m_ProtoTable.end(); ++it)
    {
        if (!it->second->IsLowOffice())	//����������Ϊ0
        {
            m_OfficeMap[it->first] = OfficeInfo();		//��Ա�б�
        }
    }

	for(int i = 0; i<40; i++)
	{
		m_awardMap[i] = 0;
	}
}
void PalaceMgr::SaveTo(int countryId, pb::GS2C_Palace_All_Info& msg)
{
    for (std::map<uint8, OfficeInfo>::iterator it = m_OfficeMap.begin(); it != m_OfficeMap.end(); ++it)
    {
        SaveTo(countryId, it->first, msg);
    }
}
void PalaceMgr::SaveTo(int countryId, int officeId, pb::GS2C_Palace_All_Info& msg)
{
    std::map<uint8, OfficeInfo>::iterator it = m_OfficeMap.find(officeId);
    if (it != m_OfficeMap.end())
    {
        pb::OfficeInfo* pInfo = msg.add_office_lst();
        pInfo->set_country_id(countryId);
        pInfo->set_office_id(officeId);
        pInfo->set_skill_use_time(it->second.skillUseTime);
        Player::SaveTo(it->second.playerId, *pInfo->mutable_player());
        for (int i = 0; i < ARRAY_SIZE(it->second.aideLst); ++i)
        {
            Player::SaveTo(it->second.aideLst[i], *pInfo->add_aide_lst());
        }
        //for (int i = 0; i < ARRAY_SIZE(it->second.aideApplyLst); ++i)
        //{
        //    Player::SaveTo(it->second.aideApplyLst[i], *pInfo->add_aide_apply_lst());
        //}
    }
}

void PalaceMgr::LoadFrom(int countryId, const pb::GS2C_Palace_All_Info& msg)
{
    for (int i = 0; i < msg.office_lst_size(); ++i)
    {
        const pb::OfficeInfo& info = msg.office_lst(i);
		if(info.country_id() == countryId)
		{
			OfficeInfo temp;
			temp.playerId = info.player().player_id();
			temp.skillUseTime = info.skill_use_time();
			for (int i = 0; i < info.aide_lst_size(); ++i)
			{
				temp.aideLst[i] = info.aide_lst(i).player_id();
			}
			for (int i = 0; i < info.aide_apply_lst_size(); ++i)
			{
				temp.aideApplyLst[i] = info.aide_apply_lst(i).player_id();
			}
			//m_OfficeMap.insert(make_pair(info.office_id(), temp));
			m_OfficeMap[info.office_id()] = temp;
		}
    }
    //���� m_aideInfoMap
    for (std::map<uint8, OfficeInfo>::iterator it = m_OfficeMap.begin();
         it != m_OfficeMap.end(); ++it)
    {
        const OfficeInfo& info = it->second;
        for (int i = 0; i < ARRAY_SIZE(info.aideLst); ++i)
        {
			m_aideInfoMap.insert(std::make_pair(info.aideLst[i], it->first));
        }
        //for (int i = 0; i < ARRAY_SIZE(info.aideApplyLst); ++i)
        //{
        //    m_aideInfoMap.insert(make_pair(info.aideApplyLst[i], it->first));
        //}
    }
}



void PalaceMgr::UpdateOfficalAwardsPerHour()
{
	for (PalaceTableMgr::ProtoMap::iterator it = sPalaceTableMgr.m_ProtoTable.begin();
		it != sPalaceTableMgr.m_ProtoTable.end(); ++it)
	{
		int id = it->first;
		int maxHour = it->second->master_draught_reward;

		if(maxHour >0)		//ֻ�д���0�Ĳŷ��Ž���
		{
			if(++m_awardMap[id] >= (uint32)maxHour)
			{
				const OfficeInfo& refInfo = m_OfficeMap[id];
				Player* dstHost = sPlayerPool.GetByPlayerId(refInfo.playerId);
				if(dstHost)
				{
					m_awardMap[id] = 0;
					sReward.Change(*dstHost, Reward::AddSoldierTimes, 1);
				}
			}

		}
	}
}

int PalaceMgr::GetOfficeId(uint64 playerId)
{
    for (std::map<uint8, OfficeInfo>::iterator it = m_OfficeMap.begin();
         it != m_OfficeMap.end(); ++it)
    {
        if (it->second.playerId == playerId) return it->first;
    }
    return 0;
}
PalaceMgr::OfficeInfo* PalaceMgr::GetOfficeInfo(uint64 playerId)
{
    for (std::map<uint8, OfficeInfo>::iterator it = m_OfficeMap.begin();
         it != m_OfficeMap.end(); ++it)
    {
        if (it->second.playerId == playerId) return &it->second;
    }
    return NULL;
}
Player* PalaceMgr::GetOfficePlayer(int officeId)
{
    std::map<uint8, OfficeInfo>::iterator it = m_OfficeMap.find(officeId);
    if (it == m_OfficeMap.end()) return NULL;

    return sPlayerPool.GetByPlayerId(it->second.playerId);
}

bool PalaceMgr::IsPlayerOffical(uint64 playerId)
{
	for (std::map<uint8, OfficeInfo>::iterator it = m_OfficeMap.begin();
		it != m_OfficeMap.end(); ++it)
	{
		if (it->second.playerId == playerId) return true;
	}
	if (Player* player = sPlayerPool.GetByPlayerId(playerId))
	{
		if(player->GetPlaceLowOfficeId() > 0) return true;
	}
	return false;
}

bool PalaceMgr::IsOfficalCanFight(Player& player,int officeid, int& errcode)
{
	NLOG("PalaceMgr::IsOfficalCanFight officeid=%d",officeid);
	std::map<uint8, OfficeInfo>::iterator refInfoa = m_OfficeMap.begin();
	for(; refInfoa != m_OfficeMap.end(); refInfoa++)
	{
		NLOG("check officemap[%d] playerid=%ld,isInfighting=%d ",refInfoa->first,refInfoa->second.playerId,refInfoa->second.isInFighting);
	}


	uint64 playerId = player.GetGuid();

	const uint8 myOfficeId = GetOfficeId(playerId);

	//if (myOfficeId > 0){
	//	if (myOfficeId <= officeid) return false; //ֻ�ܴ�ǰ���
	//}else{
	//	int lowOfficeId = player.GetPlaceLowOfficeId();
	//	if (lowOfficeId > 0 && lowOfficeId <= officeid) return false;
	//}

	if (m_aideInfoMap.find(playerId) != m_aideInfoMap.end()) 
	{
		errcode = (int)pb::AIDE_CANT_FIGHT;
		return false;
	}

	NLOG("11");

	const DB_Palace_Proto* table = sPalaceTableMgr.GetProto(officeid);
	if (table == NULL) 
	{
		errcode = (int)pb::OTHER_ERR;
		return false;
	}

	NLOG("2");
	const OfficeInfo& refInfo = m_OfficeMap[officeid];
	Player* dstHost = sPlayerPool.GetByPlayerId(refInfo.playerId);
	if (dstHost && !table->IsLowOffice()) {
		//�Է��ȼ������Լ�20�������ܴ�
		if (dstHost->GetLevel() > player.GetLevel() + 20) 
		{
			errcode = (int)pb::LEVEL_ILLIGAL;
			return false;
		}
	}

	if(refInfo.isInFighting)	
	{
		errcode = (int)pb::IS_IN_FIGNTING;
		return false;
	}
	else
		return true;
}

void PalaceMgr::FightNewOffice(Player& player, std::vector<uint32>& heroIds, int newOfficeId)    //���¹�ְ
{
	std::map<uint8, OfficeInfo>::iterator refInfoa = m_OfficeMap.begin();
	for(; refInfoa != m_OfficeMap.end(); refInfoa++)
	{
		NLOG("before officemap[%d] playerid=%ld,isInfighting=%d ",refInfoa->first,refInfoa->second.playerId,refInfoa->second.isInFighting);
	}

    uint64 playerId = player.GetGuid();

 //   const uint8 myOfficeId = GetOfficeId(playerId);
	//if (myOfficeId > 0){
	//	if (myOfficeId <= newOfficeId) return; //ֻ�ܴ�ǰ���
	//}else{
	//	int lowOfficeId = player.GetPlaceLowOfficeId();
	//	if (lowOfficeId > 0 && lowOfficeId <= newOfficeId) return;
	//}


    //�̴Ӳ���ֱ�Ӵ�
    if (m_aideInfoMap.find(playerId) != m_aideInfoMap.end()) return;

    const DB_Palace_Proto* table = sPalaceTableMgr.GetProto(newOfficeId);
    if (table == NULL) return;

    //����������
    std::vector<Combat::CombatObj*> attack;
	for (std::vector<uint32>::iterator it = heroIds.begin(); it != heroIds.end(); ++it)
    {
        Character* pHero = player.m_characterStorage->MutableCharacter(*it);
        MMO_ASSERT(pHero);
        if (pHero) {
            attack.push_back(pHero);
        }
    }

    //�������ط�
	std::set<Combat::CombatObj*> defence;
    OfficeInfo& refInfo = m_OfficeMap[newOfficeId];

	if(refInfo.isInFighting)	return;		//����ս���в�������

    Player* dstHost = sPlayerPool.GetByPlayerId(refInfo.playerId);
    if (dstHost && !table->IsLowOffice()) {
        //�Է��ȼ������Լ�20�������ܴ�
        if (dstHost->GetLevel() > player.GetLevel() + 20) return;

        struct _Temp_Add_Hero //������������
        {
			std::set<Combat::CombatObj*>& _lst;

			_Temp_Add_Hero(std::set<Combat::CombatObj*>& lst) : _lst(lst) {}
            bool operator()(const Character* p)
            {
                _lst.insert(p->CreateShadow());
                return true;
            }
        };
        _Temp_Add_Hero  objFunc(defence);
        //for (int i = 0; i < ARRAY_SIZE(refInfo.aideLst); ++i){
        //    if (Player* dstAide = sPlayerPool.GetByPlayerId(refInfo.aideLst[i])){
        //        dstAide->m_characterStorage->ForEachBattleCharacter(objFunc);
        //    }
        //}
        dstHost->m_characterStorage->ForEachBattleCharacter(objFunc);
    } else {
        if (Npc* pNpc = sNpcMgr.CreateNpc(table->monster_id, table->monster_level))
        {
            defence.insert(pNpc);
        }
    }

    if (Combat::CombatGroup* pGroup = Combat::CombatGroup::CreateGroup(attack, defence, Combat::CT_Official,System::Rand(1,6)))
    {
        pGroup->Watch(player.GetGuid());
        pb::GS2C_Combat_Pre_Begin msg;
		pGroup->SaveTo(msg);
		msg.set_isautocombat(player.IsAutoCombat());
        player.Send(pb::SMSG_COMBAT_PRE_BATTLE, msg);
        pGroup->AddCallBack_OnEnd(boost::bind(&PalaceMgr::_OnFightEnd, this, _1, _2, &player, newOfficeId));
		pGroup->AddCallBack_OnGroupRelease(boost::bind(&PalaceMgr::_OnFightCanceled, this, _1, newOfficeId));
		if(!table->IsLowOffice())
			refInfo.isInFighting = true;
    }

	std::map<uint8, OfficeInfo>::iterator refInfob = m_OfficeMap.begin();
	for(; refInfob != m_OfficeMap.end(); refInfob++)
	{
		NLOG("after officemap[%d] playerid=%ld,isInfighting=%d ",refInfob->first,refInfob->second.playerId,refInfob->second.isInFighting);
	}

}
void PalaceMgr::_OnFightEnd(Combat::CombatGroup* pGroup, bool isAttackWin, Player* player, int newOfficeId)
{
    //���չ�Ա���̴ӵĻ�Ӱ
    for (Combat::GroupLst::iterator it = pGroup->m_group_defence.begin(); it != pGroup->m_group_defence.end(); ++it)
    {
		if ((*it)->IsShadow()) Character::DeleteShadow((Character*)*it);
    }

	//struct _Temp_Hero_Func //������������
	//{
	//	bool operator()(Character* p)
	//	{
	//		p->CalculateAttr();
	//		return false;
	//	}
	//};
	//_Temp_Hero_Func objFunc;
	//player->m_characterStorage->ForEachBattleCharacterInCombat(objFunc,pGroup->m_unique_id);

    if (isAttackWin)
    {
        if (sPalaceTableMgr.GetProto(newOfficeId)->IsLowOffice())
        {
			//1������Լ��ľɹ�ְ
			player->SetPlaceLowOfficeId(0);
			uint8 oldOfficeId = GetOfficeId(player->GetGuid());
			if (oldOfficeId > 0) {
				ClearOffice(oldOfficeId);
				PalaceDB::UpdateOfficeInfoToDb(player->GetCountryId(), oldOfficeId);
			}

            player->SetPlaceLowOfficeId(newOfficeId);
        }
        else
            ChangeOffice(*player, newOfficeId);
#ifdef _MMO_SERVER_
		player->m_PalaceAchievementLog->SetPalaceLevel(*player,newOfficeId,HAS_ACHIEVE);
#endif
		int countryId = player->GetCountryId();
		pb::GS2C_Palace_All_Info msg;
		SaveTo(countryId, msg);
		player->Send(pb::SMSG_PALACE_ALL_INFO, msg);

		m_awardMap[newOfficeId] = 0;
    }

	OfficeInfo& refInfo = m_OfficeMap[newOfficeId];
	refInfo.isInFighting = false;

	//֪ͨclient
	pGroup->BroadcastStatisticData(isAttackWin);
	//pGroup->SaveAllStaticData();

	std::map<uint8, OfficeInfo>::iterator refInfoa = m_OfficeMap.begin();
	for(; refInfoa != m_OfficeMap.end(); refInfoa++)
	{
		NLOG("end officemap[%d] playerid=%ld,isInfighting=%d ",refInfoa->first,refInfoa->second.playerId,refInfoa->second.isInFighting);
	}
}

void PalaceMgr::_OnFightCanceled(Combat::CombatGroup* pGroup, int officeId)
{
	NLOG("PalaceMgr::_OnFightCanceled officeId=%d",officeId);
	//���չ�Ա���̴ӵĻ�Ӱ
	for (Combat::GroupLst::iterator it = pGroup->m_group_defence.begin(); it != pGroup->m_group_defence.end(); ++it)
	{
		if ((*it)->IsShadow()) Character::DeleteShadow((Character*)*it);
	}

	OfficeInfo& refInfo = m_OfficeMap[officeId];
	refInfo.isInFighting = false;
}
void PalaceMgr::ChangeOffice(Player& player, uint8 newOfficeId)
{
    if (newOfficeId <= 0 || (uint32)newOfficeId > sPalaceTableMgr.GetMaxOfficeId()){
        MMO_ASSERT(0);
        return;
    }
    uint64 playerId = player.GetGuid();

    //1������Լ��ľɹ�ְ
    player.SetPlaceLowOfficeId(0);
    uint8 oldOfficeId = GetOfficeId(playerId);
    if (oldOfficeId > 0) {
        ClearOffice(oldOfficeId);
        PalaceDB::UpdateOfficeInfoToDb(player.GetCountryId(), oldOfficeId);
    }

    //2�����Ŀ��Ĺ�ְ
    ClearOffice(newOfficeId);

    //3���Լ�ռ��Ŀ���ְ
    m_OfficeMap[newOfficeId].playerId = playerId;
    PalaceDB::UpdateOfficeInfoToDb(player.GetCountryId(), newOfficeId);

    //TODO:zhoumf:֪ͨ������ң���ְ�䶯���


	//֪ͨ���
	if (Country* country = sWorldMap.GetCountry(player.GetCountryId()))
	{
		LLOG("[1002] NOtice cross palace change");
		pb::GS2C_Palace_All_Info msg;

		SaveTo(player.GetCountryId(),msg);
		msg.set_unique_country_id(country->cross_uniqueID);
		country->SendCrossServerMsg(pb::SG2R_OFFICE_INFO,msg,PACKET_TYPE_SERVER_GAME);
	}
}
void PalaceMgr::CancelOffice(Player& host)                //������ְ
{
	uint64 hostId = host.GetGuid();
	int officeId = GetOfficeId(hostId);
	if (officeId > 0)		//�й�ְ�������ְ
	{
		host.SetPlaceLowOfficeId(0);

		int officeId = GetOfficeId(host.GetGuid());
		if (officeId <= 0) return;
		ClearOffice(officeId);

		PalaceDB::UpdateOfficeInfoToDb(host.GetCountryId(), officeId);

		//֪ͨ���
		if (Country* country = sWorldMap.GetCountry(host.GetCountryId()))
		{
			LLOG("[1002] NOtice cross palace change");
			pb::GS2C_Palace_All_Info msg;

			SaveTo(host.GetCountryId(),msg);
			msg.set_unique_country_id(country->cross_uniqueID);
			country->SendCrossServerMsg(pb::SG2R_OFFICE_INFO,msg,PACKET_TYPE_SERVER_GAME);
		}

	}
	else
	{
		officeId = host.GetPlaceLowOfficeId();
		if(officeId>0)
		{
			host.SetPlaceLowOfficeId(0);
		}

		std::map<uint64, uint8>::iterator it = m_aideInfoMap.find(hostId);
		if (it == m_aideInfoMap.end()) return;

		OfficeInfo& info = m_OfficeMap[it->second];

		for (int i = 0; i < ARRAY_SIZE(info.aideLst); ++i)
		{
			if (info.aideLst[i] == hostId) {
				info.aideLst[i] = 0;
				PalaceDB::UpdateOfficeInfoToDb(host.GetCountryId(), it->second);
				m_aideInfoMap.erase(it);
				break;
			}
		}
	}
}
void PalaceMgr::ApplyAide(Player& player, int officeId)   //�����̴�
{
    uint64 playerId = player.GetGuid();

    if (CanApplyAide(officeId, player))
    {
        OfficeInfo& info = m_OfficeMap[officeId];
        for (int i = 0; i < ARRAY_SIZE(info.aideLst); ++i) {
            if (info.aideLst[i] == 0) {
                info.aideLst[i] = playerId;
				m_aideInfoMap.insert(std::make_pair(playerId, officeId));
                PalaceDB::UpdateOfficeInfoToDb(player.GetCountryId(), officeId);
				break;
            }
        }
    }
}
void PalaceMgr::CancelAide(Player& aide)                   //�����̴�
{
    uint64 playerId = aide.GetGuid();

    std::map<uint64, uint8>::iterator it = m_aideInfoMap.find(playerId);
    if (it == m_aideInfoMap.end()) return;

    OfficeInfo& info = m_OfficeMap[it->second];

    for (int i = 0; i < ARRAY_SIZE(info.aideLst); ++i)
    {
        if (info.aideLst[i] == playerId) {
            info.aideLst[i] = 0;
            PalaceDB::UpdateOfficeInfoToDb(aide.GetCountryId(), it->second);
            m_aideInfoMap.erase(it);
        }
    }
}
void PalaceMgr::CancelApplyAide(Player& player)            //ȡ������
{
    uint64 playerId = player.GetGuid();

    std::map<uint64, uint8>::iterator it = m_aideInfoMap.find(playerId);
    if (it == m_aideInfoMap.end()) return;

    OfficeInfo& info = m_OfficeMap[it->second];

    for (int i = 0; i < ARRAY_SIZE(info.aideApplyLst); ++i)
    {
        if (info.aideApplyLst[i] == playerId) {
            info.aideApplyLst[i] = 0;
            PalaceDB::UpdateOfficeInfoToDb(player.GetCountryId(), it->second);
            m_aideInfoMap.erase(it);
        }
    }
}
void PalaceMgr::AgreeAide(Player& host, uint64 aideId)   //ͬ������
{
    uint64 hostId = host.GetGuid();
    int officeId = GetOfficeId(hostId);
    if (officeId > 0)
    {
        OfficeInfo& info = m_OfficeMap[officeId];

        for (int i = 0; i < ARRAY_SIZE(info.aideApplyLst); ++i)
        {
            if (info.aideApplyLst[i] == aideId) {
                info.aideApplyLst[i] = 0;

                if (AddAide(officeId, aideId))
                {
                    //TODO:֪ͨclient
                }
                PalaceDB::UpdateOfficeInfoToDb(host.GetCountryId(), officeId);
            }
        }
    }
}
void PalaceMgr::RefuseAide(Player& host, uint64 aideId)  //�ܾ�����
{
    uint64 hostId = host.GetGuid();
    int officeId = GetOfficeId(hostId);
    if (officeId > 0)
    {
        OfficeInfo& info = m_OfficeMap[officeId];

        for (int i = 0; i < ARRAY_SIZE(info.aideApplyLst); ++i)
        {
            if (info.aideApplyLst[i] == aideId) {
                info.aideApplyLst[i] = 0;
                m_aideInfoMap.erase(aideId);
                PalaceDB::UpdateOfficeInfoToDb(host.GetCountryId(), officeId);
            }
        }
    }
}
void PalaceMgr::FireAide(Player& host, uint64 aideId)    //����̴�
{
    uint64 hostId = host.GetGuid();
    int officeId = GetOfficeId(hostId);
    if (officeId > 0)
    {
        OfficeInfo& info = m_OfficeMap[officeId];

        for (int i = 0; i < ARRAY_SIZE(info.aideLst); ++i)
        {
            if (info.aideLst[i] == aideId) {
                info.aideLst[i] = 0;
                m_aideInfoMap.erase(aideId);
                PalaceDB::UpdateOfficeInfoToDb(host.GetCountryId(), officeId);
            }
        }
    }
}
IntPair PalaceMgr::TakeSalary(Player& player)  //��ȡٺ»
{
    IntPair reward;
    uint64 playerId = player.GetGuid();

    if (sOS.IsToday(player.GetPalaceTakeSalaryTime()))
    {
        return reward;
    }

	if(player.HaveTechnology(GUANYUANFULI)) {//�Ƽ�GUANYUANFULI
		if (m_aideInfoMap.find(playerId) != m_aideInfoMap.end())
		{
			const DB_Palace_Proto* table = sPalaceTableMgr.GetProto(m_aideInfoMap[playerId]);
			reward.first = Reward::Iron;
			reward.second = table->aide_iron_salary;
		}
		else
		{
			int officeId = GetOfficeId(playerId);
			if (officeId <= 0) officeId = player.GetPlaceLowOfficeId();
			const DB_Palace_Proto* table = sPalaceTableMgr.GetProto(officeId);
			reward.first = Reward::Iron;
			reward.second = table->master_iron_salary;
		}
	}
	else

	{
		if (m_aideInfoMap.find(playerId) != m_aideInfoMap.end())
		{
			const DB_Palace_Proto* table = sPalaceTableMgr.GetProto(m_aideInfoMap[playerId]);
			reward.first = Reward::Coin_Silver;
			reward.second = table->aide_silver_salary;
		}
		else
		{
			int officeId = GetOfficeId(playerId);
			if (officeId <= 0) officeId = player.GetPlaceLowOfficeId();
			const DB_Palace_Proto* table = sPalaceTableMgr.GetProto(officeId);
			reward.first = Reward::Coin_Silver;
			reward.second = table->master_silver_salary;
		}
	}

    
    player.SetPalaceTakeSalaryTime(sOS.TimeSeconds());
    sReward.Change(player, reward.first, reward.second);
    return reward;
}
void PalaceMgr::ClearOffice(int officeId)
{
    std::map<uint8, OfficeInfo>::iterator it = m_OfficeMap.find(officeId);
    if (it == m_OfficeMap.end()) return;

    OfficeInfo& info = it->second;
    //ClearAideApplyLst(info);
	ClearAideLst(info);
    info.clear();
}
void PalaceMgr::ClearAideApplyLst(OfficeInfo& info)
{
    for (int i = 0; i < ARRAY_SIZE(info.aideApplyLst); ++i)
    {
        m_aideInfoMap.erase(info.aideApplyLst[i]);
    }
    memset(info.aideApplyLst, 0, sizeof(info.aideApplyLst));
}
void PalaceMgr::ClearAideLst(OfficeInfo& info)
{
	for (int i = 0; i < ARRAY_SIZE(info.aideLst); ++i)
	{
		m_aideInfoMap.erase(info.aideLst[i]);
	}
	memset(info.aideLst, 0, sizeof(info.aideLst));
}
bool PalaceMgr::CanApplyAide(int officeId, Player& player)
{
    if (m_aideInfoMap.find(player.GetGuid()) != m_aideInfoMap.end())
    {
        return false;
    }
    if (GetOfficePlayer(officeId) == NULL)
    {
        return false;
    }
    if (player.GetPlaceLowOfficeId() > 0)
    {
        return false;
    }
    return true;
}
bool PalaceMgr::AddAide(int officeId, uint64 playerId)
{
    OfficeInfo& info = m_OfficeMap[officeId];
    const DB_Palace_Proto* table = sPalaceTableMgr.GetProto(officeId);
    for (uint i = 0; i < table->aide_num; ++i) {
        if (info.aideLst[i] == 0) {
            info.aideLst[i] = playerId;
			m_aideInfoMap.insert(std::make_pair(playerId, officeId));
            return true;
        }
    }
    return false;
}