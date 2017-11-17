#pragma once

#include "def/TypeDef.h"
#include "memory_buffer/MemoryPool.h"

class ItemArray;
class Character;
class ItemEquip;
class PlayerBaoQiLog;
class Player;

class CharacterAttr
{
    //DECLARE_OBJ_POOL(CharacterAttr)
public:
    CharacterAttr();
    void RecalculateAttr(const ItemArray& bag, const PlayerBaoQiLog& baoqi, Character& character,Player* player,const uint32 siege_army_id = 0, bool isLastAttrWork = true); //最后一个洗练属性是否生效
    int GetBattlePower() const;
    float GetAttr(uint8 idx) const;
	void SetAttr(uint8 idx,float value);

private:
    void RecalculateSelfAttr( uint32 charProto, uint32 level, uint32 quality,  uint32 reinforce,Player* player);
    void AppendRandomAttr( const Character& character );
    void AppendEquipAttr( const ItemEquip& equip, bool isLastAttrWork);
    void AppendBaoQiAttr(const PlayerBaoQiLog& baoqi);

private:
    std::vector<float> m_attrs;
};

