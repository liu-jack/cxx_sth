#include "../object/Player.h"
#include "memory_buffer/NetPack.h"
#include "utility/MsgTool.h"
#include "Opcode.pb.h"
#include "Palace.pb.h"
#include "PalaceMgr.h"
#include "PalaceSkill.h"
#include "PalaceAchievementLog.h"
using namespace pb;


OpHandler(CMSG_PALACE_ALL_INFO)
{
    int countryId = GetCountryId();
    GS2C_Palace_All_Info msg;
    sPalaceMgr(countryId)->SaveTo(countryId, msg);
    Send(pb::SMSG_PALACE_ALL_INFO, msg);
}
OpHandler(CMSG_PALACE_FIGHT_NEW_OFFICE)
{
    C2GS_Palace_Fight_New_Office req; pack >> req;

    //收集武将上阵数据
	std::vector<uint32> heroIds;
    for (int i = 0; i < req.hero_id_size(); ++i)
    {
        heroIds.push_back(req.hero_id(i));
    }

    sPalaceMgr(GetCountryId())->FightNewOffice(*this, heroIds, req.office_id());
}
OpHandler(CMSG_PALACE_CANCEL_OFFICE)
{
    sPalaceMgr(GetCountryId())->CancelOffice(*this);

	GS2C_Palace_Cancel_Office retmsg;
	Send(SMSG_PALACE_CANCEL_OFFICE,retmsg);

	int countryId = GetCountryId();
	GS2C_Palace_All_Info msg;
	sPalaceMgr(countryId)->SaveTo(countryId, msg);
	Send(pb::SMSG_PALACE_ALL_INFO, msg);
}
OpHandler(CMSG_PALACE_APPLY_AIDE)
{
    C2GS_Palace_Apply_Aide req; pack >> req;

    sPalaceMgr(GetCountryId())->ApplyAide(*this, req.office_id());

	int countryId = GetCountryId();
	GS2C_Palace_All_Info msg;
	sPalaceMgr(countryId)->SaveTo(countryId, msg);
	Send(pb::SMSG_PALACE_ALL_INFO, msg);
}
OpHandler(CMSG_PALACE_CANCEL_AIDE)
{
    sPalaceMgr(GetCountryId())->CancelAide(*this);
}
OpHandler(CMSG_PALACE_CANCEL_APPLY_AIDE)
{
    sPalaceMgr(GetCountryId())->CancelApplyAide(*this);
}
OpHandler(CMSG_PALACE_AGREE_AIDE)
{
    C2GS_Palace_Agree_Aide req; pack >> req;

    sPalaceMgr(GetCountryId())->AgreeAide(*this, req.aide_id());
}
OpHandler(CMSG_PALACE_REFUSE_AIDE)
{
    C2GS_Palace_Refuse_Aide req; pack >> req;

    sPalaceMgr(GetCountryId())->RefuseAide(*this, req.aide_id());
}
OpHandler(CMSG_PALACE_FIRE_AIDE)
{
    C2GS_Palace_Fire_Aide req; pack >> req;

    sPalaceMgr(GetCountryId())->FireAide(*this, req.aide_id());

	int countryId = GetCountryId();
	GS2C_Palace_All_Info msg;
	sPalaceMgr(countryId)->SaveTo(countryId, msg);
	Send(pb::SMSG_PALACE_ALL_INFO, msg);
}
OpHandler(CMSG_PALACE_TAKE_SALARY)
{
    IntPair reward = sPalaceMgr(GetCountryId())->TakeSalary(*this);

    GS2C_Palace_Take_Salary msg;
    pb::StReward* ptr = msg.mutable_reward();
    ptr->set_type(reward.first);
    ptr->set_value(reward.second);
    Send(pb::SMSG_PALACE_TAKE_SALARY, msg);
}
OpHandler(CMSG_PALACE_SKILL_FLY_TO)
{
    C2GS_Palace_Skill_FlyTo req; pack >> req;

    PalaceSkill::FlyTo(*this, req.city_id());
}
OpHandler(CMSG_PALACE_SKILL_EMPTY)
{
    C2GS_Palace_Skill_Empty req; pack >> req;

    PalaceSkill::Empty(*this, req.city_id());
}
OpHandler(CMSG_PALACE_SKILL_CONFUSE)
{
    C2GS_Palace_Skill_Confuse req; pack >> req;

    PalaceSkill::Confuse(*this, req.city_id());
}
OpHandler(CMSG_PALACE_SKILL_MAZE)
{
    C2GS_Palace_Skill_Maze req; pack >> req;

    PalaceSkill::Maze(*this, req.city_id());
}
OpHandler(CMSG_PALACE_SKILL_TRAP)
{
    C2GS_Palace_Skill_Trap req; pack >> req;

    PalaceSkill::Trap(*this, req.city_id());
}

OpHandler(CMSG_PALACE_TAKE_REWARD)
{
#ifdef _MMO_SERVER_
	C2GS_Palace_Take_Reward req; pack >> req;
	pb::GS2C_Palace_Take_Reward_Rsp sendmsg;
	m_PalaceAchievementLog->TakePalaceAchievement(*this,req.office_id(),sendmsg);
	Send(pb::SMSG_PALACE_TAKE_REWARD_RSP,sendmsg);
#endif
}
OpHandler(CMSG_PALACE_LEVEL_INFO)
{
#ifdef _MMO_SERVER_
	GS2C_Palace_Level_Info sendmsg;
	m_PalaceAchievementLog->SaveTo(this,sendmsg);
	Send(pb::SMSG_PALACE_LEVEL_INFO_RSP,sendmsg);
#endif
}
