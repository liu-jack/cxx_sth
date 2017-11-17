#include "memory_buffer/NetPack.h"
#include "utility/MsgTool.h"
#include "Opcode.pb.h"
#include "../../object/Player.h"
#include "Country.pb.h"
#include "../PlayerMapLogic.h"
#include "country.h"
#include "task/CountryTaskCity.h"
#include "../WorldMap.h"

using namespace pb;

OpHandler(CMSG_COUNTRY_INFO)
{
    C2GS_Country_Info msg; pack >> msg;

    GS2C_Country_Info msgBack;
    if (msg.id() == 0)
    {
        sWorldMap.SaveAllCountryInfo(msgBack);
    }
    else
    {
        if (Country* country = sWorldMap.GetCountry(msg.id()))
        {
			country->SaveTo(*msgBack.add_country());
        }
    }
    Send(SMSG_COUNTRY_INFO, msgBack);
}
OpHandler(CMSG_COUNTRY_TASK_CITY)
{
    GS2C_Country_Task_Info msg;
    sCountryTaskCity.SaveTo(msg);
    Send(SMSG_COUNTRY_TASK_CITY, msg);
}
OpHandler(CMSG_COUNTRY_TASK_PLAYER)
{
    GS2C_Country_Task_Player msg;
    sCountryTaskCity.SaveTo(msg, *this);
    Send(SMSG_COUNTRY_TASK_PLAYER, msg);
}
OpHandler(CMSG_COUNTRY_TASK_REWARD)
{
    C2GS_Country_Task_Reward req; pack >> req;

    bool ret = sCountryTaskCity.TakeReward(*this, req.task_idx());
	UpdateAllDailyQuestInfo(COUNTRY_TASK,1,0);
    GS2C_Country_Task_Reward_Ret msg;
	msg.set_issuccess(ret);
    Send(SMSG_COUNTRY_TASK_REWARD_RET, msg);
}
