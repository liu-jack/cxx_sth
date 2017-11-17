//////////////////////////////
//���� 
//id:1	�ʹ�
//	 2	����
//	 10 ����
//	 20 ����
//	 30 ����
//	 40 ��ʯ
//	 50 ��Ӫ
//////////////////////////////
#ifndef ModulePro_h__
#define ModulePro_h__
#include "def/TypeDef.h"
struct DB_Module;
class Module
{
public:
	Module(const DB_Module& dbModuleProto);
	~Module();
	uint32 id() const;
	void   AddBuildId(uint32 Id);		//�����ӽ���
	void   GetBuildIds(std::vector<uint32>& vecIds) const;
	uint32 TimeType() const;
	uint32 IncomeType() const;
private:
	const DB_Module&       m_dbModuleProto;
	std::set<uint32>       m_buildIdSet;	//�ӽ����б�
};
#endif //ModulePro_h__