#pragma once
/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#include "def/TypeDef.h"
#include "Base.h"
#include "boost/dynamic_bitset/dynamic_bitset.hpp"
struct TechnologyStudyStruct {
    uint8   starNum;						//星星数
	uint32 is_finished;
	uint64  endTime;						//研究结束时间
    TechnologyStudyStruct(){ ZeroMemoryThis; }
};


//////////////////////////zhoulunhao
enum Technology_Type{
	EXP_UP_1 =				1,	//战斗中,武将经验+50%
	FIGHT_ENFORCE1 =		2,	//兵种提升为2级,战斗力提升,征兵消耗增加
	ARMY_ENFORCE1 =			3,	//武将可多带1排士兵
	QIANGSHOU =				4,	//征收银币效果额外提升50%
	AUTOMITIC_TATIC =		5,	//战斗中可以自动选择最优战术
	ARMY_ENFORCE2 =			6,	//可多招募一个部队
	FIGHT_ENFORCE2 =		7,	//部队的攻击,防御,兵力均得到提升
	KUAISUJIAOYI =			8,	//快速交易:交易30分钟恢复一次次数
	CANGKUJIESHE=			9,	//资源区仓库上限提升20%
	FIGHT_ENFORCE3=			10,	//战力强化Ⅲ:部队技能伤害增加10%
	FIGHT_ENFORCE4=			11,	//兵种提升为3级,战斗力提升,征兵消耗增加
	ARMY_ENFORCE3=			12,	//部队可多带1排士兵
	JUNGONGBOX1=			13,	//提高军功领奖上限，奖励2级军功宝箱
	SHIQI_UP=				14,	//整点奖励数量提升
	JUNGONG_ENFORCE1=		15,	//功勋强化Ⅰ:功勋榜排名奖励中可获得耀石
	MIJI=					16,	//秘技:紫色以上精品装备技能满级且相同，开启秘技
	GUANGYUN=				17,	//官职可以对矿场产量有加成
	FIGHT_ENFORCE5=			18,	//战力强化Ⅴ:兵种提升为4级，战斗力提升，征兵消耗增加
	QIANGZHENG=				19,	//强征:增加20%征兵速度
	JUNGONGBOX2=			20,	//功勋宝箱Ⅱ:提高功勋领奖上限，奖励3级功勋宝箱
	FIGHT_ENFORCE6=			21,	//战力强化VI:部队对技能的防御增加5%
	GONGCHENGGONGXUN1=		22,	//攻城功勋I:助阵、助攻、占领城池可以获得相应功勋
	CANGKUGAIJIAN=			23,	//仓库改建:资源区仓库上限提升20%
	GONGCHENGGONGXUN2=		24,	//攻城功勋II:有概率触发连续攻城任务获得大量功勋
	JUNGONGBOX3=			25,	//功勋宝箱III:提高功勋奖励上限，奖励4级功勋宝箱
	ZENGBINGQIANGHUA1=		26,	//增兵强化I:钻石增兵时，玩家可以获得额外经验
	ARMY_ENFORCE4=			27,	//部队强化IV:可再多招募一个部队
	GUOZHANORDER=			28,	//国战号令:可以使用征召令
	YUAN_ZHENG =			29,	//远征:开启远征，完成可获得大量经验
	HENG_TONG=				30,	//亨通:黑市单次交易量翻倍
	EXP_UP_2=				31,	//经验提升II:攻占城池额外获得1万经验奖励，每日上限50万
	EXP_UP_3=				32,	//经验提升III:玩家获得的国战经验增加50%
	ATUO_GUOZHAN1=			33,	//自动国战I:部队向目标城自动派兵直到占领，持续30分钟

	JUNGONG_ENFORCE2=		34,	//功勋强化II:升级科技后，可以从功勋榜中获得经验
	IRONSMITH=				35,	//耀石交易:交易出现耀石资源
	SUIT=					36,	//套装:可以合成套装
	HUANYINGLICHANG=		37,	//幻影力场:世界出现幻影力场，可获得免费增兵次数
	JUNGONGBOX4=			38,	//功勋宝箱IV:提高功勋领奖上限，奖励5级功勋宝箱
	JUNGONG_ENFORCE3=		39,	//功勋强化III:功勋榜排名奖励中耀石产出数量提高50%
	ZENGBINGQIANGHUA2=		40,	//增并强化II:钻石增兵时，玩家可以获得额外经验
	YUAN_ZHENG2=			41,	//远征:远征升级，提高远征奖励，完成可获得额外50%经验
	EXP_UP_4=				42,	//经验提升IV:攻占城池额外获得2万经验奖励，每日上限100万
	NUEDUOSHENGJI=			43,	//掠夺升级:掠夺俘虏时，有概率获得掠夺升级经验
	EXP_UP_5=				44,	//经验提升V:玩家获得的国战经验增加30%
	GAOJIYUANZHENG=			45,	//高级远征:提高远征奖励，获得经验提高50%
	TUJING_QIANGHUA=		46,	//突进强化:己方人数大于敌方人数两倍即可突进
	BUWENDINGLICHANG=		47,	//不稳定力场:幻影力场出现的次数增加10次
	JUNGONG_ENFORCE4=		48,	//功勋强化IV:功勋榜排名奖励中耀石产出数量提高50%
	ZENGBINGQIANGHUA3=		49,	//增并强化III:钻石增兵时，玩家可以获得额外经验
	BAOSHIHECHENG1=			50,	//宝石合成I:开放宝石（最高8级），世界出现宝石商人
	DIYUJUBEN=				51,	//地狱剧本:开启地狱远征，完成可获得大量经验
	JUNGONGBOX5=			52,//功勋宝箱V:提高功勋奖励上限，奖励6级功勋宝箱
	LIANBINZENGYI=			53,//练兵增益:开放新练兵令，旧练兵令自动转化为经验
	ZHANSHENGJUBEN=			54,	//进阶远征:提高远征奖励，获得海量经验，经验提高50%
	ZENGBINGQIANGHUA4=		55,	//增并强化IV:钻石增兵时，玩家可以获得额外经验
	BAOSHIHECHENG2=			56,	//宝石合成II:开放10级宝石，宝石征收
	JISUZENGBING=			57,	//极速征兵:征兵令上限增加30个
	BAOSHIHECHENG3=			58,	//宝石合成III:开放12级宝石
	GUANYUANFULI=			59,	//官员福利:官员每日军资改为耀石奖励
	ZHANGETONGKUAI=			60,	//战个痛快:自动国战初始耗粮取消
	ZHUCHENGGAIZAO=			61,	//主城改造:开放主城改造，大幅提高资源产量和部队实力
	ZENGBINGQIANGHUA5=		62,	//增兵强化V:钻石增兵时，玩家可以获得额外经验
	BAOSHIHECHENG4=			63,	//宝石合成IV:开放14级宝石
	JUNGONGBOX6=			64,	//功勋宝箱VI:提高功勋领奖上限，奖励7级功勋宝箱
	JINGDAXISUAN=			65,	//精打细算:幻影研究时间减少50%
	ZENGBINSHANGXIAN=		66,	//增兵上限:免费增兵次数上限提高40个
	ZHENGDIANJIANGLI=		67,	//整点奖励:整点奖励气矿增加50%
	BAOSHIHECHENG5=			68,	//宝石合成V:开放15级宝石，宝石升阶
	BOXSHANGXIAN=			69,	//宝箱上限:宝箱上限增加50个
	ZENGBINGQIANGHUA6=		70,	//增兵强化VI:钻石增兵时，玩家可以获得额外经验
	ZHAOMUFUJIANG=			71,	//招募副将:可多招募一个部队，作为主将的第五排兵上阵
	BUDUIJUEXING=			72,	//部队觉醒:120级以上部队可以觉醒
	SHENGSHIJINGHUA=		73,	//神石进化:开放21级晶石进化为神石
	ZHANGONGHEHE=			74,	//战功赫赫:功勋满8000后将开启高级功勋宝箱
	ZENGBINGQIANGHUA7=		75,	//增兵强化VII:钻石增兵时，玩家可以获得额外经验
	SHENGSHIJINGHUA2=		76,	//神石进化II:开放30级神石（10级技能石）
	GONGGAOLIZHONG=			77,	//功高礼重I:高级功勋宝箱难度提升，宝箱品质进阶
	ZENGBINGQIANGHUA8=		78,	//增兵强化VIII:钻石增兵时，玩家可以获得额外经验
	SHENGSHIJINGHUA3=		79,	//神石进化III:开放20级技能石
	GONGGAOLIZHONG2	=		80	//功高礼重II:提高功勋上限，提高功勋奖励
};

class Player;
namespace pb {
    class GxDB_Technology_All_Info;
	class GS2C_Technology_All_Info;
	class GS2C_Technology_Finish;
	class GS2C_Technology_UpStar;
	class GS2C_Technology_SpeedUp;
	class GS2C_Technology_Study;
}
class PlayerTechnology {
    typedef std::map<uint32, TechnologyStudyStruct>     StudyMap;
private:
    StudyMap    m_StudyInfo;				//正在研究的科技表
	bool TechIdIsInvalid(const uint32 tech_id);

public:
    PlayerTechnology();
    void SaveTo(pb::GxDB_Technology_All_Info& msg);	//server<->DB
	void SaveTo(Player& player, pb::GS2C_Technology_All_Info& msg);	//client->server
    void LoadFrom(const pb::GxDB_Technology_All_Info& msg);

	void SaveTo(pb::GS2C_Technology_Finish& msg);			//研究完成消息
	void SaveTo(pb::GS2C_Technology_UpStar& msg);			//升星消息
	void SaveTo(pb::GS2C_Technology_SpeedUp& msg);			//加速消息

    bool Have(uint32 id);
    bool Finish(Player* player, uint32 id);
    void UpStar(Player& player, uint32 id, pb::GS2C_Technology_UpStar& msg);
    bool Study(Player* player, uint32 id,pb::GS2C_Technology_Study& msg);
	void SpeedUp(Player& player, uint32 id, pb::GS2C_Technology_SpeedUp& msg);
	
	void OnAward(Player& player,uint32 id);
	
	void GM_Finish(Player* player,uint32 tid);

	uint32 HasFinishUpStar(uint32 tec_id);
	uint32 HaveTec(uint32 tec_id);
	uint32 StudyTec(Player* player,uint32 tec_id);
};