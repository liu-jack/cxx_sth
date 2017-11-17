//////////////////////////////
//�ӽ�����Ӧ�ȼ�
//////////////////////////////
#ifndef BuildingLvPro_h__
#define BuildingLvPro_h__

#include "def/TypeDef.h"
struct DB_BuildingLevelUp;


class PlayerValueOwner;

class BuildingLevel
{
public:
	BuildingLevel(const DB_BuildingLevelUp& dbBuildingLv);
	~BuildingLevel();
public:    
	uint32 id() const;
	uint32 lv() const;		//�ȼ�
	uint32 honor() const;	//������ѫ����
	uint32 exp() const;		//�������齱��
	uint32 building_id() const;	//�����ӽ���id
	uint32 lvUpTime() const;	//��������ʱ��
	uint32 Icome() const;		//�ٶȲ�����
	uint32 IcomLimit() const;	//�洢���޹���
	uint32 coinReq() const;		//��������
	uint32 woodReq() const;		//����ľ��
private:
	const DB_BuildingLevelUp& m_dbBuildingLvProto;

};

#endif //BuildingLvPro_h__

