#ifndef _Build_define_h_
#define _Build_define_h_

#define  AUTOLVUPHAMMERTIMES  20
#define  MAXBUILDINGLVUPHAMMERS  4
#define  TIMERBYMINUTESECONDS 60
#define  TIMERBYHOURSECONDS 3600
#define  TIMERBYFOURHOURSECONDS 14400

enum TimerTimeType
{
	TIMERBYMINUTE = 1,
	TIMERBYHOUR,
	TIMERBYFOURHOUR,
};

enum ModuleType
{
	MODULE_FORREBUILDTYPE = 0,
	MODULE_IMPERIAL = 1,//皇城
	MODULE_CHENGMEN = 2,//城门
	MODULE_MINJU = 10, //名居  银矿
	MODULE_MUCHANG = 20, //木场  晶矿
	MODULE_LONGCHANG = 30,//农场	气矿
	MODULE_KUANGCHANG = 40,//矿场	铁矿
	MODULE_BINGYING = 50,//兵营		兵力
};

typedef std::set<uint32> BuildingSet;
typedef std::map<uint32,BuildingSet> ModuleLvUpInfo;
#endif // !_Stage_define_h_

