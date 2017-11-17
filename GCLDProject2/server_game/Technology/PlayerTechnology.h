#pragma once
/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#include "def/TypeDef.h"
#include "Base.h"
#include "boost/dynamic_bitset/dynamic_bitset.hpp"
struct TechnologyStudyStruct {
    uint8   starNum;						//������
	uint32 is_finished;
	uint64  endTime;						//�о�����ʱ��
    TechnologyStudyStruct(){ ZeroMemoryThis; }
};


//////////////////////////zhoulunhao
enum Technology_Type{
	EXP_UP_1 =				1,	//ս����,�佫����+50%
	FIGHT_ENFORCE1 =		2,	//��������Ϊ2��,ս��������,������������
	ARMY_ENFORCE1 =			3,	//�佫�ɶ��1��ʿ��
	QIANGSHOU =				4,	//��������Ч����������50%
	AUTOMITIC_TATIC =		5,	//ս���п����Զ�ѡ������ս��
	ARMY_ENFORCE2 =			6,	//�ɶ���ļһ������
	FIGHT_ENFORCE2 =		7,	//���ӵĹ���,����,�������õ�����
	KUAISUJIAOYI =			8,	//���ٽ���:����30���ӻָ�һ�δ���
	CANGKUJIESHE=			9,	//��Դ���ֿ���������20%
	FIGHT_ENFORCE3=			10,	//ս��ǿ����:���Ӽ����˺�����10%
	FIGHT_ENFORCE4=			11,	//��������Ϊ3��,ս��������,������������
	ARMY_ENFORCE3=			12,	//���ӿɶ��1��ʿ��
	JUNGONGBOX1=			13,	//��߾����콱���ޣ�����2����������
	SHIQI_UP=				14,	//���㽱����������
	JUNGONG_ENFORCE1=		15,	//��ѫǿ����:��ѫ�����������пɻ��ҫʯ
	MIJI=					16,	//�ؼ�:��ɫ���Ͼ�Ʒװ��������������ͬ�������ؼ�
	GUANGYUN=				17,	//��ְ���ԶԿ󳡲����мӳ�
	FIGHT_ENFORCE5=			18,	//ս��ǿ����:��������Ϊ4����ս����������������������
	QIANGZHENG=				19,	//ǿ��:����20%�����ٶ�
	JUNGONGBOX2=			20,	//��ѫ�����:��߹�ѫ�콱���ޣ�����3����ѫ����
	FIGHT_ENFORCE6=			21,	//ս��ǿ��VI:���ӶԼ��ܵķ�������5%
	GONGCHENGGONGXUN1=		22,	//���ǹ�ѫI:����������ռ��ǳؿ��Ի����Ӧ��ѫ
	CANGKUGAIJIAN=			23,	//�ֿ�Ľ�:��Դ���ֿ���������20%
	GONGCHENGGONGXUN2=		24,	//���ǹ�ѫII:�и��ʴ����������������ô�����ѫ
	JUNGONGBOX3=			25,	//��ѫ����III:��߹�ѫ�������ޣ�����4����ѫ����
	ZENGBINGQIANGHUA1=		26,	//����ǿ��I:��ʯ����ʱ����ҿ��Ի�ö��⾭��
	ARMY_ENFORCE4=			27,	//����ǿ��IV:���ٶ���ļһ������
	GUOZHANORDER=			28,	//��ս����:����ʹ��������
	YUAN_ZHENG =			29,	//Զ��:����Զ������ɿɻ�ô�������
	HENG_TONG=				30,	//��ͨ:���е��ν���������
	EXP_UP_2=				31,	//��������II:��ռ�ǳض�����1���齱����ÿ������50��
	EXP_UP_3=				32,	//��������III:��һ�õĹ�ս��������50%
	ATUO_GUOZHAN1=			33,	//�Զ���սI:������Ŀ����Զ��ɱ�ֱ��ռ�죬����30����

	JUNGONG_ENFORCE2=		34,	//��ѫǿ��II:�����Ƽ��󣬿��Դӹ�ѫ���л�þ���
	IRONSMITH=				35,	//ҫʯ����:���׳���ҫʯ��Դ
	SUIT=					36,	//��װ:���Ժϳ���װ
	HUANYINGLICHANG=		37,	//��Ӱ����:������ֻ�Ӱ�������ɻ�������������
	JUNGONGBOX4=			38,	//��ѫ����IV:��߹�ѫ�콱���ޣ�����5����ѫ����
	JUNGONG_ENFORCE3=		39,	//��ѫǿ��III:��ѫ������������ҫʯ�����������50%
	ZENGBINGQIANGHUA2=		40,	//����ǿ��II:��ʯ����ʱ����ҿ��Ի�ö��⾭��
	YUAN_ZHENG2=			41,	//Զ��:Զ�����������Զ����������ɿɻ�ö���50%����
	EXP_UP_4=				42,	//��������IV:��ռ�ǳض�����2���齱����ÿ������100��
	NUEDUOSHENGJI=			43,	//�Ӷ�����:�Ӷ��²ʱ���и��ʻ���Ӷ���������
	EXP_UP_5=				44,	//��������V:��һ�õĹ�ս��������30%
	GAOJIYUANZHENG=			45,	//�߼�Զ��:���Զ����������þ������50%
	TUJING_QIANGHUA=		46,	//ͻ��ǿ��:�����������ڵз�������������ͻ��
	BUWENDINGLICHANG=		47,	//���ȶ�����:��Ӱ�������ֵĴ�������10��
	JUNGONG_ENFORCE4=		48,	//��ѫǿ��IV:��ѫ������������ҫʯ�����������50%
	ZENGBINGQIANGHUA3=		49,	//����ǿ��III:��ʯ����ʱ����ҿ��Ի�ö��⾭��
	BAOSHIHECHENG1=			50,	//��ʯ�ϳ�I:���ű�ʯ�����8������������ֱ�ʯ����
	DIYUJUBEN=				51,	//�����籾:��������Զ������ɿɻ�ô�������
	JUNGONGBOX5=			52,//��ѫ����V:��߹�ѫ�������ޣ�����6����ѫ����
	LIANBINZENGYI=			53,//��������:��������������������Զ�ת��Ϊ����
	ZHANSHENGJUBEN=			54,	//����Զ��:���Զ����������ú������飬�������50%
	ZENGBINGQIANGHUA4=		55,	//����ǿ��IV:��ʯ����ʱ����ҿ��Ի�ö��⾭��
	BAOSHIHECHENG2=			56,	//��ʯ�ϳ�II:����10����ʯ����ʯ����
	JISUZENGBING=			57,	//��������:��������������30��
	BAOSHIHECHENG3=			58,	//��ʯ�ϳ�III:����12����ʯ
	GUANYUANFULI=			59,	//��Ա����:��Աÿ�վ��ʸ�Ϊҫʯ����
	ZHANGETONGKUAI=			60,	//ս��ʹ��:�Զ���ս��ʼ����ȡ��
	ZHUCHENGGAIZAO=			61,	//���Ǹ���:�������Ǹ��죬��������Դ�����Ͳ���ʵ��
	ZENGBINGQIANGHUA5=		62,	//����ǿ��V:��ʯ����ʱ����ҿ��Ի�ö��⾭��
	BAOSHIHECHENG4=			63,	//��ʯ�ϳ�IV:����14����ʯ
	JUNGONGBOX6=			64,	//��ѫ����VI:��߹�ѫ�콱���ޣ�����7����ѫ����
	JINGDAXISUAN=			65,	//����ϸ��:��Ӱ�о�ʱ�����50%
	ZENGBINSHANGXIAN=		66,	//��������:������������������40��
	ZHENGDIANJIANGLI=		67,	//���㽱��:���㽱����������50%
	BAOSHIHECHENG5=			68,	//��ʯ�ϳ�V:����15����ʯ����ʯ����
	BOXSHANGXIAN=			69,	//��������:������������50��
	ZENGBINGQIANGHUA6=		70,	//����ǿ��VI:��ʯ����ʱ����ҿ��Ի�ö��⾭��
	ZHAOMUFUJIANG=			71,	//��ļ����:�ɶ���ļһ�����ӣ���Ϊ�����ĵ����ű�����
	BUDUIJUEXING=			72,	//���Ӿ���:120�����ϲ��ӿ��Ծ���
	SHENGSHIJINGHUA=		73,	//��ʯ����:����21����ʯ����Ϊ��ʯ
	ZHANGONGHEHE=			74,	//ս���պ�:��ѫ��8000�󽫿����߼���ѫ����
	ZENGBINGQIANGHUA7=		75,	//����ǿ��VII:��ʯ����ʱ����ҿ��Ի�ö��⾭��
	SHENGSHIJINGHUA2=		76,	//��ʯ����II:����30����ʯ��10������ʯ��
	GONGGAOLIZHONG=			77,	//��������I:�߼���ѫ�����Ѷ�����������Ʒ�ʽ���
	ZENGBINGQIANGHUA8=		78,	//����ǿ��VIII:��ʯ����ʱ����ҿ��Ի�ö��⾭��
	SHENGSHIJINGHUA3=		79,	//��ʯ����III:����20������ʯ
	GONGGAOLIZHONG2	=		80	//��������II:��߹�ѫ���ޣ���߹�ѫ����
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
    StudyMap    m_StudyInfo;				//�����о��ĿƼ���
	bool TechIdIsInvalid(const uint32 tech_id);

public:
    PlayerTechnology();
    void SaveTo(pb::GxDB_Technology_All_Info& msg);	//server<->DB
	void SaveTo(Player& player, pb::GS2C_Technology_All_Info& msg);	//client->server
    void LoadFrom(const pb::GxDB_Technology_All_Info& msg);

	void SaveTo(pb::GS2C_Technology_Finish& msg);			//�о������Ϣ
	void SaveTo(pb::GS2C_Technology_UpStar& msg);			//������Ϣ
	void SaveTo(pb::GS2C_Technology_SpeedUp& msg);			//������Ϣ

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