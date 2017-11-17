//////////////////////////////
//大建筑 
//id:1	皇宫
//	 2	副本
//	 10 银矿
//	 20 晶矿
//	 30 气矿
//	 40 曜石
//	 50 兵营
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
	void   AddBuildId(uint32 Id);		//设置子建筑
	void   GetBuildIds(std::vector<uint32>& vecIds) const;
	uint32 TimeType() const;
	uint32 IncomeType() const;
private:
	const DB_Module&       m_dbModuleProto;
	std::set<uint32>       m_buildIdSet;	//子建筑列表
};
#endif //ModulePro_h__