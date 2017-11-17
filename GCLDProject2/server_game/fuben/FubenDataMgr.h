#ifndef __FubenMgr_h__
#define __FubenMgr_h__
#include "def/TypeDef.h"
#include <boost/unordered_map.hpp>
#include "ptr_container/PtrMap.h"
#include "Fuben.h"
#include "FubenStage.h"

typedef PtrMap<uint32, Fuben> FubenMap;
typedef PtrMap<uint32, FubenStage> StageMap;
typedef	std::map<uint32, std::vector<const FubenStage*> > FubenStageMap;

class FubenDataMgr {

    FubenMap        m_fubenMap;			//������Ϣ ����
    StageMap		m_stageMap;			//�ؿ���Ϣ ����
    FubenStageMap   m_FubenStageList;	//����Ĺؿ���Ϣ��ָ��  ���ݸ���id������ ��ͨ�ء���Դ�ء����Ź�
	FubenStageMap	m_CrusadeStageList;	//�����Զ����Ϣ
	FubenStageMap	m_BranchStageList;
	FubenStageMap	m_HideStageList;
public:
    static FubenDataMgr& Instance(){ static FubenDataMgr T; return T; }	//��̬����

    void Init();
    const Fuben*      GetFuben(uint32 fubenId) const;	//���ָ�򸱱���Ϣ��ָ��
    const FubenStage* GetStage(uint32 stageId) const;	//���ָ��ؿ���Ϣ��ָ��
    const Fuben&      GetFirstFuben() const;	
	const FubenMap& GetFubenMap() const;
    const FubenStage& GetFirstStage() const;
    const Fuben*      GetFubenByStageID(uint32 stageId) const;	//���ݹؿ�id���ָ�򸱱���Ϣ��ָ��
    const std::vector<const FubenStage*>* GetFubenStageList(uint32 fubenId) const;
	const std::vector<const FubenStage*> *GetCrusadeFubenStageList(uint32 fubenId) const;
	const std::vector<const FubenStage*> *GetBranchFubenStageList(uint32 fubenId) const;
	const std::vector<const FubenStage*> *GetHideFubenStageList(uint32 fubenId) const;

	const FubenStage* GetFirstCrusadeStageByFubenID(uint32 fubenID) const;//ͨ������id��õ�ǰԶ����һ���ؿ�
	const FubenStage* GetFirstCrusadeStage() const;//Ĭ�ϵ�һ��Զ������
	const uint32	  GetPrestageID(uint32 stageid) const;

   // bool IsResourceStage(uint32 stageId);
	const uint32 GetBranchFubenGroupId(const uint32 stageId) const;
	bool GetNextStageID(uint32& rFubenId, uint32& rStageId) const;
	uint32 GetNextBranchStageId(uint32 FubenId,const uint32 stageId) const;
	void GetUnlockCharacter(const uint32 rFubenId,const uint32 rStageId,std::set<uint32>& unlockChar) const;
	void initCrusadeTable();
	//bool GetFubenFogReward(const uint32 num,IntPair* pair);
	const StageMap& GetStageMap() const;


	uint32 GetCurrentFubenStageNum(uint32 stageID);//��ǰ�ؿ�Id��Ӧ��Զ���������ܹۿ���������Զ������
	uint32 GetCurrentCrusadeStageNum(uint32 stageId);//��ǰ�ؿ�Id��Ӧ��Զ��������Զ���ؿ���
	uint32 GetCurrentCrusadeRewardStageNum(uint32 stageId);//��ǰ�ؿ�Id��Ӧ��Զ��������Զ�������ؿ���
};
#define sFubenDataMgr FubenDataMgr::Instance()		//��̬����

#endif //__FubenMgr_h__