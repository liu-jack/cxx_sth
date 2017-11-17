#pragma once
#include "def/TypeDef.h"
struct DB_FubenProto;
struct DB_OpenFog;
class Fuben {
    const DB_FubenProto&       m_dbFubenProto;
public:
    enum EType {
        Common,     //ÆÕÍ¨
        Crusade,    //Ô¶Õ÷
    };
    Fuben(const DB_FubenProto& db);
    uint32 Id() const;
    uint32 ReqLv() const;
    bool IsCommonFuben() const;
    bool IsCrusadeFuben() const;
	uint32 GetStageMode() const;
};

