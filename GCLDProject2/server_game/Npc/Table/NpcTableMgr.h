#ifndef  NPCTABLEMGR_H_
#define  NPCTABLEMGR_H_
#include "def/TypeDef.h"
#include "datastruct/struct_npc.h"
//#include "ptr_container/PtrMap.h"
#ifndef USE_PTR_HASH_MAP
#include "ptr_container/PtrMap.h"
#else
#include "ptr_container/PtrHashMap.h"
#endif // USE_PTR_HASH_MAP


class NpcTableMgr {
#ifndef USE_PTR_HASH_MAP
	typedef PtrMap<uint32,const NpcProto> ProtoMap;
#else
	typedef PtrHashMap<uint32,const NpcProto> ProtoMap;
#endif
private:
    ProtoMap    m_ProtoLst;

    std::vector<float> m_attrs;
public:
	NpcTableMgr();
    static NpcTableMgr& Instance(){ static NpcTableMgr T; return T; }

    void Init();

    const NpcProto* GetProto(uint32 id);

    void RecalculateAttr(uint32 npcProtoId, uint32 level);
    float GetAttr(uint8 idx) const;
};
#define sNpcTableMgr (NpcTableMgr::Instance())
#endif