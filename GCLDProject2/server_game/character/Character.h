#ifndef Character_h__
#define Character_h__

#include "boost/scoped_ptr.hpp"

#include "memory_buffer/MemoryPool.h"
#include "memory_buffer/ValueArray.h"
#include "../Combat/CombatObj.h"
#include "../buff/Buff.h"
#include "../Combat/combat_define.h"

//////////////////////////////////////////////////////////////////////////
namespace pb
{
    class GS2C_CharacterCreate;
    class GS2C_CharacterUpdate;
	class GS2C_Dinner_Data;
	class GS2C_Dinner_Resp;
} 
class CharacterProto;
class CharacterStorage;
class City;
class Player;
class ItemEquip;

namespace Combat
{
	enum TacticEnum;
	enum CombatType;
}

typedef uint32 CharacterId;

class Character : public Combat::CombatObj
{
    DECLARE_OBJ_POOL(Character)
    friend class CharacterStorage;

public:
	Player* m_player;		//�������

    Character(const CharacterProto& proto, CharacterStorage* pStorage, Player* player);		//�����佫
	Character(const Character& ref);														//�����佫��Ӱ
    ~Character();

    inline CharacterId GetID( void ) const {    return m_charId ;    }	//��ȡ����佫ʶ���±�
    uint32 ProtoID() const;												// ����ʶ���protoID  m_dbCharProto.index

	void CopyAllFromParent(const Character& ref);			//���佫�����Ը��Ƶ���Ӱ����
    //////////////////////////////////////////////////////////////////////////
public:
	static int GetMaxLevelCanUpgrade(Player* player);		//��ȡ�佫���������ȼ���������ҵȼ����ж�

    int32 GetLevel() const;									//��ȡ�佫�ȼ�
    int32 GetCurXP() const;									//��ȡ��ǰ����
	int32 GetAttack() const;
	int32 GetQuality() const;								//��ȡ�佫��ǰƷ�ʣ���ɫ��

	int32 GetSlotEquip( uint32 slot) const;					//��ö�Ӧ��λ��װ��
	bool  IsAllSlotEquip() const;							//�Ƿ����в�λ����װ��

	const CharacterProto& Proto() const;					//��ȡ���ģ������

	void    SetUnmodified();								//�����޸�
	bool    HasUpdateInfo() const;							//�Ƿ�������޸�

	uint32	GetEnableTactics(uint32 geography = 0);			//ͨ�����λ�ÿ��õ�ս��


	//////////////////////////////////////////////////////////////////////////
    int32 GetReinforce() const;								//abandon--ǿ���Ǽ�
    int32 GetRuneInlay() const;								//abandon--������Ƕ���


    int32 GetSkillLevel( uint32 skill_index) const;			//abandon--��ü��ܵȼ�
    bool  IsSkillActive( uint32 skill_index) const;			//abandon--�����Ƿ񼤻�
    
    bool  IsRandomAttrAllZero() const;						//abandon--
   // bool  HasGoodRandomAttr() const;						//abandon--��δȷ�ϵ�����������һ����ֵ���Ҿ��Ǹ�
    int32 GetRandomAttr( uint32 attr_idx) const;			//abandon--��δȷ��ѵ��������
    int32 GetTrainingCount( uint32 training_type) const;	//abandon--�������� ʹ�ô���
    int32 GetTrainedAttr( uint32 attr_idx) const;			//abandon--����ȷ�Ϻ������ֵ

	uint32 CalculateCharacterMilitaryPower() const;
	uint32 GetCharacterMilitaryPower() const;
public:
    void    Rebirth();										//abandon--����
    bool    TryAddXP( int32 xp, bool isFromCross = false);	//�������Ӿ��飬������������¼�������
    int32   GetSumXp() const;								//abandon--����佫ȫ������
	int32	GetSumXpLvUpTo( int toLv) const;				//abandon--����佫�������ض��ȼ������ȫ������
    bool    TryInlayRune( uint32 slot);						//abandon--��Ƕ����
	int32	GetlvMaxXp(const int32 level);

    // ����
    bool    Training( uint32 training_type, uint32 addtion );//abandon--�����佫
    bool    ConfirmTraining();								 //abandon--��������

    // װ��
    bool    SetSlotEquip( uint32 slot, uint32 itemId);		//����ָ����λ��װ��

	int  GetEquipSlot( uint32 bagSolt );					//���ݵ���id��ò�λ
	// ����
    bool    TryAddSkillLevel( uint32 skill_index);			//abandon--�����佫���ܵȼ�
    bool    TryActiveSkill( uint32 skill_index);			//abandon--�����佫����


    void    GM_SetAttr( int level , int quality, int reinforce);	//GMָ����������
	void GM_SetLevel(int level){ LevelUpTo(level);}

protected:
    void    LevelUpTo( int32 level);						//������ָ���ȼ�
	void	ExpUpTo( int32 xp );
	void    SetLevel( int32 level);							//���õȼ�
    void    SetCurXP( int32 xp);							//���õ�ǰ����
    void    SetQuality( int32 quality);						//abandon--�����佫�ȼ�
    void    SetReinforce( int32 renforce);					//abandon--
    void    SetRuneInlay( int32 runeInlay);					//abandon--
	void	SetEnableTactic(const std::string& tacs);		//��������ս��
    //////////////////////////////////////////////////////////////////////////
public:
    void    SaveTo( pb::GS2C_CharacterCreate& msg) const;
    void    LoadFrom( const pb::GS2C_CharacterCreate& msg) ;

    void    WriteUpdateInfoTo( pb::GS2C_CharacterUpdate& msg) const;
    void    ApplyUpdateInfoFrom( const pb::GS2C_CharacterUpdate& msg);
	void	SaveTo(pb::GS2C_CharacterUpdate& msg) const;
public:
    inline void SetId( CharacterId charId ) {    m_charId = charId ;   }		//����charid;
    const ValueArray& Values() const;											//���valueֵ
    ValueArray& MutableValues();												//��ÿ��޸ĵ�valueֵ

    //////////////////////////////////////////////////////////////////////////
	void    NotifyStorageModified();											//���øı�������
	void CalculateAttr(const uint32 siege_army_id = 0);														//��ս��״̬�����������

private:
    void    ClearRune();												//abandon--
    //bool    IsAllInlayed() const;										//abandon--
    static bool  IsRuneInlayed(int32 inlayInfo , uint32 slot);			//abandon--
    static float GenerateTrainingFactor( uint32 training_type, uint32 addtion );//abandon--
    static int GenerateTrainingCount( uint32 training_type);			//abandon--
    static int GetTrainingValue( float maxValue, float factor);			//abandon--

//slg�佫���------------------------------------------------------------
public:
    //�����ͼ�ƶ����
    uint32 GetCurCity(){ return m_curCityId; }				//����佫��ǰ���ڳ���
    uint32 GetCountryId();									//����佫��������
    void _Move();											//�佫�ƶ�����
    bool FlyTo(uint32 cityId, bool is_jump = true);			//�佫�ƶ���Ŀ�����
    bool SetMap(CMap* p);									//abandon--�����佫���ڵ�ͼ�������ͼ��
	bool CharacterMoved();									//abandon--�佫�Ƿ��ƶ���
	void initPath();										//�����佫�ƶ�·��

	void _AutoCountryWarMove();								//�佫�Զ���ս�ƶ�

    bool RecoverFullSoldier();								//�ָ�������
	//bool CleanAllSoldiers();

    //CombatObj���virtual func
	void    ClearCombatStatus();							//����ս��״̬�����¼�������
	void	InitCombatDataSecond(uint32 geography = 0);		//���ν���ս����ʼ��
    void    OnDead(CombatObj* killer);						//�佫����
	void    MoveTo(uint32 cityId);							//�����ƶ���ָ���ǳ�
    uint64  GetPlayerId();									//���������ҵ�id
    const string& GetPlayerName();							//���������ҵ�����
    uint32  GetTableId(){ return ProtoID(); }				//����佫���protoid����charid
    float   SkillRatio();									//����佫�����˺�ϵ��
    uint8   SkillRange();									//��ü����˺�����
	uint32 GetSolderColNum();								//���ʿ��������
	uint32 GetSolderRowNum();								//���ʿ������
////----------------------------------
	uint32 GetPlayerSolderRowNum();							//���ʿ������
/////------------------------------
	void SetCurSolderNum(int num);							//���õ�ǰʿ��������Ѫ����
	void    InitCombatData();								//��ʼ������ս������
	uint32 GetMaxRow();										//����������
	void reCalcPower(Combat::CombatType type = Combat::CT_Country);	//���¼���ս���������	Ĭ�Ϲ�ս
	void ReFreshCombatData(const uint32 siege_army_id= 0);								//ˢ���佫����
	bool   IsFullHp();										//����Ѫ��
	virtual int GetMaxHp();											//������Ѫ��
	int GetZhaoMuNeedCost() const;							//�����ļ��Ҫ���ĵ���Դ
	uint32 NeedHp();										//�ָ���Ѫ����Ҫ��hp
	uint32 RecoverHp(uint32 solders);						//�ָ�һ������hp
	virtual int GetTotalFightPercent();
	virtual int GetTotalSkillPercent(const Combat::CombatType type,const uint32 obj_type = 0);
	virtual int GetTotalExpPercent(Combat::CombatType type );
	virtual int GetTotalReduceCostHP() ;
	virtual int GetTotalDoubleKill(Combat::CombatType type) ;
	virtual int GetTotalDeSkillPercent() ;
	virtual uint32 GetType() const;
	virtual void AddSkillPercentBySeigeArmySpecificId(int add_percent, int enemyType = 0);
	virtual uint32 GetTerrDebuffAddtion(uint32 terrType);
	virtual uint32 GetPlayerLevel();
	virtual double GetTerrTalentAddtion(uint32 terrType);

	//��������
    int EscapeFromDungeon();								//��������ʱ��
    void SetDungeonOwner(uint64 playerId);					//���ñ�˭����

	bool isInPrison();										//�Ƿ񱻲���
	bool CharIsIdle();//�佫�Ƿ����
	bool GetIsTunTian();
	void SetIsTunTian(bool is_tuntian);

    //��Ӱ
    Character* CreateShadow() const;						//������Ӱ
	//bool IsShadow(){ return m_IsShadow; }					//�Ƿ��ǻ�Ӱ

	static void DeleteShadow(Character* p);					//ɾ����Ӱ
	static void _DeleteShadow(); //��������֡����ʱ����
private:
	void FreshAttr(const uint32 siege_army_id = 0);										//ս����ˢ������  �Ƽ�ID
public:

	uint32 SkillID();										//����id
	bool NewAutoTactic(Combat::CombatType cTyp = Combat::CT_Country, Combat::TacticEnum tTyp = Combat::Attack);	//ѡ��ս��

	//�Զ���ս�ƶ�
	void AutoMoveTo(uint32 desCity);

public:
    bool _ExitCity(City& city);								//�뿪�ǳ�
    bool _EnterCity(City& city,bool is_jump = true);		//����ǳ�
    void _ReEnterCurCity();									//����ʱ���½��뵱ǰ�ǳ�
	bool _GetHeroCanMove() { return m_canMove; }			//�佫�Ƿ����ƶ�

public:
	void _SetEscapeTime(uint64 time) { m_escapeTime = time; }
	uint64 _GetEscapeTime() { return m_escapeTime; }

	void SetIsInAutoCountryWar(bool value) { m_isInAutoCountryWar = value; }
	bool GetIsInAutoCountryWar() { return m_isInAutoCountryWar; }

	void GetNextCityId(uint32& cityid);

private:
    uint32  m_curCityId;
    bool    m_IsShadow;

    std::queue<uint32> m_path; //·�߳���
	static std::vector<Character*>  m_deleteShadow;

    uint64  m_DungeonOwner;
	uint32	m_CurSoliderColNum;
	//uint32  m_CurSoliderRowNum;

	uint64  m_lastMoveTime;		//ms
	bool    m_canMove;
//slg�佫���------------------------------------------------------------

private:
    uint32                      m_charId;
    const CharacterProto&       m_proto;
    CharacterStorage*           m_pStorage;
    ValueArray                  m_values;		//���һλΪ����ӳ�
	IntPairVec					m_enableTacs;
	bool						m_hasShodowTactic;		//��Ӱ�Ƿ��Ѿ�ѡ��
	Combat::TacticEnum			m_shadowTactic;	//��Ӱ��ս��ѡ��ѡ��֮���ܱ�

	bool						m_isReloadMove; //��ʶ�佫�Ƿ��ƶ�
	bool						m_isTuntian;//�Ƿ�����

	uint64						m_escapeTime;

	bool						m_isInAutoCountryWar;
};





#endif // Character_h__