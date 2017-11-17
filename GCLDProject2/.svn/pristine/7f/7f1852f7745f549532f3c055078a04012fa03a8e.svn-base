#include "ServiceMgr.h"
#include "Service.h"


extern uint32 _Service_NpcMove(void* p);
extern uint32 _Service_Combat(void* p);
extern uint32 _Service_Country_AttackNpc(void* p);
extern uint32 _Service_Char_Buff(void* p);
extern uint32 _Service_Player_Buff(void* p);
extern uint32 _Service_CharMove(void* p);
extern uint32 _Service_CharAutoCountryWarMove(void* p);
extern uint32 _Service_Delivering_Monsters(void* p);
#ifdef _MMO_SERVER_
//extern uint32 _Service_Player_UpdateBuyTime(void* p);
//extern uint32 _Service_GovnermentAffairsIncTimes(void *p);
extern uint32 _Service_Country_Celebrity_Npc(void *p);
#endif
extern uint32 _Service_Country_JinWeiNpc(void* p);


iService* ServiceMgr::m_aService[SERVICE_COUNT] = {
	/* Combat */new cServiceMap(_Service_Combat),
	/* Service_Buff_Player */new cServiceList(_Service_Player_Buff),
	/* Service_Buff_Char */new cServiceList(_Service_Char_Buff),
    /* NpcMove */new cServiceMap(_Service_NpcMove),
	/* charMove */new cServiceMap(_Service_CharMove),
	/* charMove */new cServiceMap(_Service_CharAutoCountryWarMove),
#ifdef _MMO_SERVER_
//	new cServiceList(_Service_Player_UpdateBuyTime),
//	new cServiceList(_Service_GovnermentAffairsIncTimes),
	new cServiceList(_Service_Country_Celebrity_Npc),
#endif
							new cServiceList(_Service_Delivering_Monsters),
	/* Country_AttackNpc */new cServiceList(_Service_Country_AttackNpc),
							new cServiceList(_Service_Country_JinWeiNpc),
};
void ServiceMgr::RunAllService(uint32 time_elasped, uint32 timenow)
{
	for (int i = 0; i < SERVICE_COUNT; ++i)
	{
		m_aService[i]->RunSevice(time_elasped, timenow);
	}
}
bool ServiceMgr::UnRegister(ServiceEnum typ, ServiceObj* pObj)
{
    return m_aService[typ]->UnRegister(pObj);
}
bool ServiceMgr::Register(ServiceEnum typ, ServiceObj* pObj)
{
    return m_aService[typ]->Register(pObj);
}