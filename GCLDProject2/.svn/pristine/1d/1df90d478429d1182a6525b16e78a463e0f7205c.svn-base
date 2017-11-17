
#include "Technology.pb.h"
#include "PlayerData.h"
#include "data/DbProxyDataMgr.h"
#include "utility/MsgTool.h"
#include "data/TechnologyTable.h"
#include "boost/typeof/typeof.hpp"

bool PlayerData::save_technology_record()
{
    m_TechnologyStudyTable.SaveMod();
    return true;
}
void PlayerData::WritePlayerTechnologyInfo(pb::GxDB_Technology_All_Info& msg)
{

    for (BOOST_AUTO(it, m_TechnologyStudyTable.Begin()); it != m_TechnologyStudyTable.End(); ++it)
    {
        MsgTool::SaveToRepeatField(it->second, msg.mutable_study_info());
    }
}

Msg_Realize(SG2D_UPDATA_TECHNOLOGY_STUDY, TechnologyStudy)
{
    if (TechnologyStudyTable* pCell = m_TechnologyStudyTable.GetElement(msg.tec_id()))		//如果表中有数据，则更新
    {
        SetIsDirty(DIRTY_TECHNOLOGY);			//设置更新
        pCell->LoadFrom(msg);
        m_TechnologyStudyTable.SetModifiedID(msg.tec_id());
    }
    else
    {
        TechnologyStudyTable cell;
        cell.player_id = GetPlayerId();
        cell.LoadFrom(msg);
        m_TechnologyStudyTable.AddAndAddToCache(&cell);
    }
}
