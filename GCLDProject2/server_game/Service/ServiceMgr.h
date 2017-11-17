#pragma once
#include "def/TypeDef.h"
enum ServiceEnum {
	SERVICE_COMBAT,
	SERVICE_BUFF_PLAYER,
	SERVICE_BUFF_CHAR,
	SERVICE_NPC_MOVE,
	SERVICE_CHAR_MOVE,			//武将移动
	SERVICE_CHAR_AUTO_COUNTRY_WAR_MOVE,	//武将自动国战移动
#ifdef _MMO_SERVER_
//	SERVICE_BUY_RESET_TIMER,
//	SERVICE_GOV_UPDATE_TIME,
	SERVICE_COUNTRY_CELEBRITY_NPC,//结交名仕
#endif
	SERVICE_DELIVE_MONSTER,		//坚守国境任务投放蛮族
	SERVICE_COUNTRY_ATTACK_NPC,
	SERVICE_COUNTRY_GUARD,      //禁卫军
	SERVICE_COUNT,
};
class iService;
typedef void ServiceObj;
class ServiceMgr {
    static iService* m_aService[SERVICE_COUNT];
public:
    static void RunAllService(uint32 time_elasped, uint32 timenow);
    static bool UnRegister(ServiceEnum typ, ServiceObj* pObj);
    static bool Register(ServiceEnum typ, ServiceObj* pObj);
};
