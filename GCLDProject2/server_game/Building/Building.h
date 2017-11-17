//////////////////////////////
//�ӽ���
//////////////////////////////
#ifndef BuildingPro_h__
#define BuildingPro_h__
#include "def/TypeDef.h"
#ifndef USE_PTR_HASH_MAP
#include "ptr_container/PtrMap.h"
#else
#include "ptr_container/PtrHashMap.h"
#endif // USE_PTR_HASH_MAP
struct DB_BuildingProto;
class PlayerValueOwner;
class BuildingLevel;
class Building
{
public:
	Building(const DB_BuildingProto& dbBuildingProto);
	~Building();
public:    
	uint32 id() const;	//�ӽ���id
	uint32 maxLv() const { return m_maxLv; }	//���ȼ�
	uint32 moduleId() const;	//��������
	void AddBuildingLevel(BuildingLevel* buildLv);//����ӽ�����ͬ�ȼ�
	uint32 LvUpTime( uint32 curLv ) const;		//���������ĵ�ʱ��
	uint32 LvReq() const;						//���ŵȼ�
	uint32 Honor(uint32 lv) const;				//������ѫ����
	uint32 Exp(uint32 lv) const;				//�������齱��
	uint32 Icome(uint32 lv) const;				//�ָ��ٶ���
	uint32 IcomeLimit(uint32 lv) const;			//�洢����
	uint32 NextRebuildId() const;				//abandon--����˳��
	uint32 RebuildLaborNeed() const;			//abandon--������Ҫ���͹���
	uint32 RebuildNeedTime() const;				//abandon--������Ҫʱ��
	uint32 RebuildResAdd() const;				//abandon--������Դ�����ӳ�
	bool LvUpReq(uint32 lv,uint32& coinReq, uint32& woodReq) const;	//������Ҫ���ĵ�������ľ��

private:
#ifndef USE_PTR_HASH_MAP
	typedef PtrMap<uint32, BuildingLevel> BuildingLvMap;
#else
	typedef PtrHashMap<uint32, BuildingLevel> BuildingLvMap;
#endif // USE_PTR_HASH_MAP
	const DB_BuildingProto& m_dbBuildingProto;	//�ӽ�������
	BuildingLvMap  m_mapBuildingLv;		//���ӽ������еȼ���Ӧ�����б�
	uint32 m_maxLv;						//���ȼ�
};

#endif //BuildingPro_h__

