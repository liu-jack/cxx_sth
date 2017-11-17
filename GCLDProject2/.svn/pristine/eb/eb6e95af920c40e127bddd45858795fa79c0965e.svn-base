#include "QuestLogger.h"
#include "QuestManager.h"
#include "QuestRecord.h"
#include "mysqldata.h"
#include "../object/Player.h"
#include "behavior/PlayerBehaviorCounter.h"
#include "def/MmoAssert.h"
#include "Logger.h"
#include "utility/Utility.h"
#include "utility/MsgTool.h"
#include "Loot/LootList.h"

#include "server_client/DbProxyServerClient.h"

#include "Opcode.pb.h"
#include "Quest.pb.h"
#include "ServerOpcode.pb.h"
#include "Enum.pb.h"
#include "Item.pb.h"
#include "CommomDef.pb.h"
#include "../Trigger/trigger.h"
#include "seige_force/SeigeForceLog.h"
#include "BaseDefineMgr.h"
#include "BaseDefine.pb.h"
#ifdef _MMO_SERVER_
#include "../AutomaticUpdate/AutoUpdateGame.h"
#endif

QuestLogger::QuestLogger()
{

}

void QuestLogger::UpdateQuest(Player*player, const BehaviorType& behavior )
{
    for (QuestRecords::iterator it = m_questRecords.begin(); 
        it != m_questRecords.end(); ++it)
    {
        QuestRecord * record = it->second;
        if ( !record)
            continue;
        if ( !record->IsActived())
            continue;

        if( record->Proto().GetBehaviorType() == behavior)
        {
            SendUpdateMsg(player,record , SEND_MOD_CLIENT);
        }
    }
}

void QuestLogger::UpdateQuestCategory(Player*player, uint32 categroy1 )
{
    for (QuestRecords::iterator it = m_questRecords.begin(); 
        it != m_questRecords.end(); ++it)
    {
        QuestRecord * record = it->second;
        if ( !record)
            continue;
        if ( !record->IsActived())
            continue;

        if( record->Proto().GetBehaviorType().m_cat1 == categroy1)
        {
            SendUpdateMsg(player,record , SEND_MOD_CLIENT);
        }
    }
}

void QuestLogger::UpdateQuestCategory(Player*player, uint32 category1, uint32 category2 )
{
    for (QuestRecords::iterator it = m_questRecords.begin(); 
        it != m_questRecords.end(); ++it)
    {
        QuestRecord * record = it->second;
        if ( !record)
            continue;
        if ( !record->IsActived())
            continue;

        if( record->Proto().GetBehaviorType().m_cat1 == category1 
            && record->Proto().GetBehaviorType().m_cat2 == category2)
        {
            SendUpdateMsg(player,record , SEND_MOD_CLIENT);
        }
    }
}




void QuestLogger::UpdateQuest( Player* player,uint32 category1, uint32 category2, uint32 category3 /*= 0*/, uint32 condition /*= 0*/ )
{
    UpdateQuest(player, BehaviorType(category1, category2, category3, condition));
}

bool QuestLogger::TryAddQuest(Player*player, QuestID questId )
{
    const QuestProto * proto = sQuestMgr.GetQuestProto( questId);
    if ( !proto)
    {
        return false;
    }
    if ( !CanAddQuest(questId))
    {
        return false;
    }

    bool result = _AddQuest(player,proto);

    if (result)
    {
        SendUpdateMsg(player, questId);
		if(questId == GET_BASE_DEF_UINT(pb::BD_QUEST_TEMP_OPEN_ATTACK_CITY_TROOP))
		{
			player->m_SeigeForceLog->FillAllSeigeArmy();
		}
    }

    return result; 
}

bool QuestLogger::GetQuestFinished(QuestID questId)
{
	if ( questId < m_questCompleted.size() )
	{
		return m_questCompleted.test( questId);
	}
	else
	{
		return false;
	}    
}

bool QuestLogger::TryAddQuestSilently(Player* player, QuestID questId )
{
    const QuestProto * proto = sQuestMgr.GetQuestProto( questId);
    if ( !proto)
    {
        return false;
    }
    if ( !CanAddQuest(questId))
    {
        return false;
    }
    bool result = _AddQuest(player,proto);

    if (result)
    {
        SendUpdateMsg(player,questId, SEND_MOD_DB_PROXY);
    }

    return result; 
}


bool QuestLogger::CanAddQuest( QuestID questID) const
{
    if ( HasQuestCompleted( questID))
    {
        return false;
    }
    return true;
}

bool QuestLogger::_AddQuest(Player* player,const QuestProto *proto)
{
    MMO_ASSERT( proto != NULL);
    uint32 questId = proto->ID(); 
    if ( HasQuest(questId))
    {
        return false;
    }
	if(proto->GetBehaviorType().m_cat1 == QC1_GET_EXPLOIT)
	{
		if(proto->GetBehaviorType().m_cat2 == QC2_MAIN_QUEST)
			player->SetFlag(pb::PLAYER_FLAG_QUEST_GET_EXPLOIT_ACTIVE);
		else if(proto->GetBehaviorType().m_cat2 == QC2_BRANCH_QUEST)
			player->SetFlag(pb::PLAYER_FLAG_BRANCH_QUEST_GET_EXPLOIT_ACTIVE);
	}
    QuestRecord * record = CreateQuestInst(*proto, dynamic_cast<IRecordProvider&>(*player));     
    if ( record)
    {
        AddQuestRecord( record);
        if( proto->IsRepeatable())
        {
            record->ResetActiveTime();
        }
        return true;
    }
    else
    {
        return false;
    }

}

void QuestLogger::AddQuestRecord( QuestRecord * questRecord )
{
    MMO_ASSERT( questRecord != NULL );
    MMO_ASSERT( !HasQuest( questRecord->Proto().ID() ) );
    m_questRecords[ questRecord->Proto().ID()] = questRecord; 
}

void QuestLogger::AfterLoadLogger(Player* player)
{
    // 任务链型任务首任务 添加  
    // 避免有新任务增加时无法添加。 
    const QuestIDSet& ids = sQuestMgr.GetFirstMainQuests();
    for_each( ids.begin(), ids.end(), boost::bind( &QuestLogger::TryAddQuestSilently, this,player, _1));
	//支线任务
	const QuestIDSet& branchIds = sQuestMgr.GetFirstBranchQuests();
	for_each(branchIds.begin(),branchIds.end(),boost::bind(&QuestLogger::TryAddQuestSilently,this,player,_1));

    // 添加可重复完成的任务
    const QuestIDSet& repeatableIds = sQuestMgr.GetRepeatableQuests();
    for_each( repeatableIds.begin(), repeatableIds.end(), boost::bind( &QuestLogger::TryAddQuestSilently, this,player, _1));    
}

int QuestLogger::TakeQuestReward(Player* player, QuestID questId, pb::GS2C_LootList& msg,uint32 &player_exp)
{
	if(HasQuestCompleted(questId))
		return pb::ErrQuestHasFinished;
    QuestRecord* record = MutableQuestRecord(questId);
    if ( record == NULL)
        return pb::ErrQuestNotExist;
    if ( !record->IsActived())
        return pb::ErrQuestNotActive; 
    if ( !record->IsFinished() )
        return pb::ErrQuestNotFinish;
    if ( !record->IsMatchVipLv( player->GetVipLevel()))
        return pb::ErrVipLvNotMatch;
    if ( !record->IsMatchLevel( player->GetLevel()))
        return pb::ErrLvNotMatch;
    // to do take reward;
    _TakeQuestReward(player,record, msg,player_exp);
	if(sTrigger.Check(player,sFunctionOpen.GetTriggerId(ENTER_WORLD)))
	{
		if(player->GetEnterWorldTime() == 0)
		{
			player->SetEnterWorldTime(sOS.GetRealTime());
		}
	}
    return pb::ErrCommonSuccess;    
}

int QuestLogger::GetQuestRewards(Player* player, uint32 questId , pb::GS2C_LootList& msg,uint32 &player_exp)
{
	QuestRecord* record = MutableQuestRecord(questId);
	if(!record)
		return pb::ErrQuestNotExist;
	
	_TakeQuestReward(player,record, msg,player_exp);
	if(sTrigger.Check(player,sFunctionOpen.GetTriggerId(ENTER_WORLD)))
	{
		if(player->GetEnterWorldTime() == 0)
		{
			player->SetEnterWorldTime(sOS.GetRealTime());
		}
	}
	if(questId == GET_BASE_DEF_UINT(pb::BD_QUEST_TEMP_OPEN_ATTACK_CITY_TROOP))
	{
		player->m_SeigeForceLog->ClearLogOfSeigeMap();
	}
	return pb::ErrCommonSuccess;    
}

bool QuestLogger::HasQuest( QuestID questId ) const
{
    return m_questRecords.pfind( questId) != NULL;
}

bool QuestLogger::HasQuestCompleted( QuestID questId ) const
{
    if ( questId < m_questCompleted.size() )
    {
        return m_questCompleted.test( questId);
    }
    else
    {
        return false;
    }    
}

void QuestLogger::MarkQuestCompleted(Player*player,QuestID questId,const QuestProto& proto)
{
    if ( questId >= m_questCompleted.size())
    {
        m_questCompleted.resize( questId + 1);        
    }
    m_questCompleted.set( questId);
    SendQuestCompletedMsg(player, questId);
	if(proto.GetBehaviorType().m_cat1 == QC1_GET_EXPLOIT)
	{
		if(proto.GetBehaviorType().m_cat2 == QC2_MAIN_QUEST)
		{
			player->RemoveFlag(pb::PLAYER_FLAG_QUEST_GET_EXPLOIT_ACTIVE);
			player->SetValueInt(pb::PLAYER_FIELD_MAIN_QUEST_GET_EXPLOIT,0);//主线任务完成
		}
		else if(proto.GetBehaviorType().m_cat2 == QC2_BRANCH_QUEST)
		{
			player->RemoveFlag(pb::PLAYER_FLAG_BRANCH_QUEST_GET_EXPLOIT_ACTIVE);
			player->SetValueInt(pb::PLAYER_FIELD_BRANCH_QUEST_EXPLOIT,0);//支线任务完成  
		}
	}
}

void QuestLogger::MarkQuestUnCompleted( QuestID questId )
{
	if ( HasQuestCompleted(questId))
	{
		m_questCompleted.set( questId,false);      
	}
}


QuestRecord* QuestLogger::MutableQuestRecord( QuestID questId )
{
    return m_questRecords.pfind(questId);
}

const QuestRecord* QuestLogger::GetQuestRecord( QuestID questId ) const
{
    return m_questRecords.pfind(questId);
}

void QuestLogger::RemoveQuest(Player*player, QuestID questId )
{
    m_questRecords.erase( questId);
    SendDeleteMsg(player,questId);
}

QuestRecord * QuestLogger::CreateQuestInst( const QuestProto& proto ,const IRecordProvider& provider )
{
    return new QuestRecord( proto, provider);
}

QuestRecord* QuestLogger::CreateQuestInst( const pb::GxDB_Quest& questMsg, const IRecordProvider& provider )
{
     const QuestProto* proto = sQuestMgr.GetQuestProto(questMsg.quest_id());
     if ( proto)
     {
         QuestRecord * questRecord = CreateQuestInst( *proto, provider);
         if ( questRecord)
         {
             questRecord->LoadFrom( questMsg);
         }
         return questRecord;
     }
     else
     {
         ELOG( " Quest From db can not create InstanceQuest");
         return NULL;  
     }
}


void QuestLogger::Load(Player*player, const pb::GS2C_QuestList& questList )
{
    m_questRecords.clear();
    for( int i = 0; i < questList.quests_size(); ++i)
    {
        const pb::GxDB_Quest& questMsg = questList.quests(i);
        QuestRecord * questRecord = CreateQuestInst(questMsg, *player);
        if ( questRecord)
        {
            if ( !HasQuest(questRecord->Proto().ID()) )
            { 
                AddQuestRecord(questRecord);
            }
            else
            {
                MASSERT( false, " Duplicative Quest Id !" ) ;
            }
        }
    }
    m_questCompleted.clear();
    uint32 count = questList.finish_mask().count();
    m_questCompleted.resize( count );
    boost::from_block_range(
        questList.finish_mask().finish_masks().values().begin(),
        questList.finish_mask().finish_masks().values().end(),
        m_questCompleted);

}

void QuestLogger::Save(Player*player, pb::GS2C_QuestList& questList )
{
    questList.Clear();
    for ( QuestRecords::const_iterator it = m_questRecords.begin();
        it != m_questRecords.end(); ++it )
    {
		MsgTool::SaveToRepeatField( it->second, questList.mutable_quests());
		MarkAllPreQuestFinished(player,it->first);
    }
    pb::FinishMask* masks = questList.mutable_finish_mask();
    if (masks)
    {
        boost::to_block_range(m_questCompleted, RepeatedFieldBackInserter( masks->mutable_finish_masks()->mutable_values()));
    }
}

void QuestLogger::SendQuestCompletedMsg( Player* player,QuestID questId )
{
    if ( player)
    {
        pb::GxDB_Quest msg;
        msg.set_quest_id( questId);
        player->Send( pb::SMSG_COMPLETE_QUEST, msg);
        sDbProxyClient.SendPlayerPack( *player, pb::SG2D_COMPLETE_QUEST, msg);
    }
}   

void QuestLogger::SendDeleteMsg(Player* player, QuestID questId )
{
    if ( player)
    {
        pb::GxDB_Quest questMsg;
        questMsg.set_is_deleted( true);
        questMsg.set_quest_id( questId);

        player->Send(pb::SMSG_UPDATE_QUEST, questMsg);
        sDbProxyClient.SendPlayerPack( *player,pb::SG2D_UPDATE_QUEST, questMsg);
    }
}

void QuestLogger::SendUpdateMsg(Player* player,const QuestRecord * record, SEND_MOD mode/* = SEND_MOD_ALL*/)
{
    if ( player && record && mode)
    {
        pb::GxDB_Quest questMsg;
        record->SaveTo( questMsg );
        if (  Utility::HasFlag( mode , SEND_MOD_CLIENT) )
        {
            player->Send(pb::SMSG_UPDATE_QUEST, questMsg);
        }

        if (  Utility::HasFlag( mode , SEND_MOD_DB_PROXY) )
        {
            sDbProxyClient.SendPlayerPack( *player,pb::SG2D_UPDATE_QUEST, questMsg);
        }
    }
}

void QuestLogger::SendUpdateMsg(Player*player, QuestID questId , SEND_MOD mode /*= SEND_MOD_ALL*/ )
{
    const QuestRecord* record = GetQuestRecord(questId);
	//MMO_ASSERT(record == NULL);
	NLOG("questId = %u",questId);
	if(record == NULL){
		return;
	}
    SendUpdateMsg(player,record, mode);
}

void QuestLogger::FillDBReward(Player*player, const DB_QuestReward* dbReward , LootList& lootList )
{
	//暂未开启
    //std::vector<int> triggerLst;
   //Utility::SplitStr(dbReward->reward_trigger, triggerLst, ',');
    //if (!sTrigger.Check(m_pPlayer, triggerLst)) return;

    switch( dbReward->reward_category1) {
    case REWARD_TYPE_EXP:
        {
			switch (dbReward->reward_category2) {
			case QUEST_REWARD_PLAYER_EXP:
				lootList.AddPlayerExp(dbReward->reward_count);
				NLOG("reward_count = %d  **************************************************************",dbReward->reward_count);
				break;
			case QUEST_REWARD_CHAR_EXP:
				lootList.AddCharExp(dbReward->reward_count);
				break;
			default:
				MMO_ASSERT(0);
				break;
			}
        }
        break;
    case REWARD_TYPE_CURR:
        {
            lootList.AddMoney( dbReward->reward_category2, dbReward->reward_count);
        }
        break;
    case REWARD_TYPE_ACHIEVE_POINT:
        {
            if ( PlayerBehaviorCounter * counter = player->m_behaviorCounter.get())
            {
                counter->TriggerBehavior(player, QC1_PLAYER, QC2_PLAYER_ACHIEVEMENT_POINT,  QC3_UNDEFINED, 0, dbReward->reward_count);
            }
        }
        break;
    case REWARD_TYPE_ITEM:
        {
            lootList.AddItem( dbReward->reward_category2, dbReward->reward_count);
        }
        break;
	case REWARD_TYPE_EQUIP:
		{
			IntV3Vec retMap;
			ParseItems(dbReward->reward_equip_id,dbReward->reward_count,retMap);
			for(IntV3Vec::const_iterator iterBeg = retMap.begin(); iterBeg != retMap.end(); ++iterBeg)
			{
				lootList.AddItem(iterBeg->first,iterBeg->second);
			}
			player->CreateMailItem(pb::IR_QUEST_REWARD_GET,retMap);
		}
		break;
    default:
        break;
    }
}

void QuestLogger::HandleGM(Player*player, QUEST_GM_CMD gmCmd, int param )
{
    switch( gmCmd)
    {
    case QUEST_GM_ACCEPT:
        {
			for(int i = 1;i <= param;++i)
			{
				const QuestProto * proto = sQuestMgr.GetQuestProto( i);
				if ( proto)
				{
					_AddQuest(player,proto);
					SendUpdateMsg(player,i);
				}
			}
        }
        break;
    case QUEST_GM_FINISH:
        {
			uint32 toQuestId = (uint32)param;
			const uint32 NearestId = sQuestMgr.FindNearestId(toQuestId);
			MarkQuestUnCompleted(NearestId);
			 const QuestIDSet& ids = sQuestMgr.GetFirstMainQuests();
			 for (QuestIDSet::const_iterator iterIds = ids.begin(); iterIds != ids.end(); ++iterIds )
			 {
				  const QuestProto * proto = sQuestMgr.GetQuestProto((*iterIds));
				  if (proto)
				  {
					  uint32 curQuestId = proto->ID();
					  if (proto->NextQuest() == 0)
					  {
						  break;
					  }
					  while(curQuestId < NearestId)
					  {
						  MarkQuestCompleted(player,curQuestId,*proto);
						  if ( proto)
						  {
							  const RewardList& rewardList = proto->GetRewardList();
							  LootList lootList;
							  pb::GS2C_LootList msg;
							  for ( RewardList::const_iterator it = rewardList.begin();
								  it != rewardList.end(); ++it)
							  {
								  FillDBReward(player, *it, lootList);
							  }
							  lootList.PickAllMoneyAndItems(player, msg, pb::IR_QUEST_REWARDS_GET);
							  curQuestId = proto->NextQuest();
							  if(sTrigger.Check(player,sFunctionOpen.GetTriggerId(ENTER_WORLD)))
							  {
								  if(player->GetEnterWorldTime() == 0)
								  {
										player->SetEnterWorldTime(sOS.GetRealTime());
								  }
							  }
						  }
						  else
						  {
							  break;
						  }
						  if (curQuestId == 0)
						  {
							  break;
						  }
						  proto = sQuestMgr.GetQuestProto(curQuestId);
					  }
				  }
			 }

			 std::set<uint32> recordIds;
			 for (QuestRecords::iterator it = m_questRecords.begin(); 
				 it != m_questRecords.end(); ++it)
			 {

				 uint32 rId = (uint32)it->first;
				 if (GetQuestFinished(rId))
				 {
					 recordIds.insert(rId);
				 }
			 }
			 for (std::set<uint32>::iterator rbiter =  recordIds.begin(); rbiter != recordIds.end(); ++rbiter)
			 {
				 m_questRecords.erase((*rbiter));
				 SendDeleteMsg(player,(*rbiter));
			 }

			QuestRecord* record = MutableQuestRecord(NearestId);
			if (record)
			{
				if ( record->IsRepeatable())
				{
					record->SetInactiveUntilNextPeriod();
					SendUpdateMsg( player,record);
				}
				else if ( record->IsSequentQuest() )
				{
					SendUpdateMsg(player, record);
				}
			}
			else
			{
				const QuestProto * proto = sQuestMgr.GetQuestProto((NearestId));
				if(proto)
				{
					TryAddQuest(player, proto->ID());
				}
			}
			
        }
        break;
    default:
        break;
    }
}

void QuestLogger::_TakeQuestReward(Player*player, QuestRecord* record, pb::GS2C_LootList& msg,uint32 &player_exp )
{
    const RewardList& rewardList = record->Proto().GetRewardList();
    LootList lootList;
    for ( RewardList::const_iterator it = rewardList.begin();
        it != rewardList.end(); ++it)
    {
        FillDBReward(player, *it, lootList);
    }
	player_exp = lootList.GetPlayerExp();
	lootList.SetPlayerExp(0);
    lootList.PickAllMoneyAndItems(player, msg, pb::IR_QUEST_REWARDS_GET);
	msg.set_player_exp(player_exp);
    if ( record->IsRepeatable())
    {
        record->SetInactiveUntilNextPeriod();
        SendUpdateMsg(player, record);
    }
    else if ( record->IsSequentQuest() )
    {
		MarkQuestCompleted( player,record->Proto().ID(),record->Proto());
		if (record->Proto().NextQuest() != 0)
		{
			TryAddQuest(player, record->Proto().NextQuest());
		}
		RemoveQuest( player,record->Proto().ID());
    }
}

void QuestLogger::MarkAllPreQuestFinished(Player* player, const QuestID id )
{
	uint32 quest_id = id;
	const QuestProtoMap& quest_proto_map = sQuestMgr.GetQuestProtoMap();
	QuestProtoMap::const_reverse_iterator iter = quest_proto_map.rbegin();
	for(;iter != quest_proto_map.rend();++iter )
	{
		if(iter->second->NextQuest() == quest_id)
		{
			if(!HasQuestCompleted(iter->first))
			{
				MarkQuestCompleted(player,iter->first,*iter->second);
				quest_id = iter->first;
			}
		}
	}
}

bool QuestLogger::CheckCategory(uint32 category1,uint32 category2,uint32 category3)
{
	for(QuestRecords::iterator iter = m_questRecords.begin();iter != m_questRecords.end();++iter)
	{
		if(iter->second->CheckCategory(category1,category2,category3))
			return true;
	}
	return false;
}

bool QuestLogger::ParseItems(const std::string& equip_id, const uint32 count,IntV3Vec& retMap) const
{
	std::vector<string> retVecId;
	Utility::SplitStr(equip_id, retVecId, ',');
	size_t idCount = retVecId.size();
	if (idCount != 2)
	{
		return false;
	}
	IntV3 tmp;
	tmp.first = atoi(retVecId[0].c_str());
	tmp.third = retVecId[1];
	tmp.second = count;
	retMap.push_back(tmp);
	return true;
}
