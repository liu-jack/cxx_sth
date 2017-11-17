#ifndef BuildingLogPro_h__
#define BuildingLogPro_h__
#include "def/TypeDef.h"
namespace pb
{
	class GxDB_Building;
}

class PlayerValueOwner;
class Building;
class BuildingLog
{
public:
	BuildingLog(const Building* buildingProto);
	~BuildingLog();
public:    
	uint32 id() const;
	uint32 ModuleId() const;		//�ӽ�����������id
	uint32 LvUpTime() const;		//��������ʱ��
	void SaveTo(pb::GxDB_Building& msg) const;
	void LoadFrom(const pb::GxDB_Building& msg);
	uint32 BuildLvUp();		//�ӽ�������
	uint32 CurLv() const { return m_curBuildLv;}	//��ǰ�ӽ����ȼ�
	bool  IsRechMaxLv() const;		//�Ƿ�ﵽ���ȼ�
	uint32 PlayerLvReq() const;		//���ӽ������ŵȼ�
	uint32 Icome() const;			//���ӽ�����ǰ�ȼ�����
	uint32 IcomeLimit() const;		//�洢����
	uint32 SetRebuildState(bool isRebuild);	//abandon--����
	bool LvUpReq(uint32& coinReq, uint32& woodReq) const;	//���������������ľ��
	void CheckBuildingIsUnlock(uint32 player_lv);		//����Ƿ�ȼ��ﵽ����Ҫ��
	bool IsUnlock() const;		//�Ƿ����
private:
	const Building* m_pBuildingProto;
	uint32 m_curBuildLv;
	time_t m_lvUpStartTime;
	bool m_isRebuild;
	bool m_isUnlock;
};

#endif //BuildingLogPro_h__

