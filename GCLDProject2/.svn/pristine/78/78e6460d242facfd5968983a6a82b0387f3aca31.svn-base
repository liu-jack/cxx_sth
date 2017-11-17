#pragma once

#include "def/TypeDef.h"

class PlayerBuffTable;
namespace pb
{
    class GS2C_Buff_All_Info;
}
class CBuff {
public:
    enum Type { // 见buff表备注
        Add_Exp_Percent     = 1,    //获得经验增加百分比
        Add_Food_Percent,           //粮食产量加成百分比
        Add_Silver_Percent,         //银币产量加成百分比
        Add_Wood_Percent,           //木材产量加成百分比
        Add_Soldier_Percent,        //兵营产量加成百分比
        Inc_Exp_Dec_Fight_Percent,//获得经验增加百分比、战斗力减少百分比
        Inc_Combat_To_Shadow_Percent,//对幻影战斗力增加百分比
        Dec_Skill_Hurt_Percent,//战法威力降低百分比
        Inc_Fight_Percent,//武将战斗力加成百分比
        Dec_Soldier_Food_Percent,//征兵耗粮减少百分比
        Inc_Kill_Cnt_Percent,//杀敌数增加百分比
        SecKill_Rate,   //秒杀敌方的概率
        Crit_Rate,      //暴击敌方的概率
        Inc_Soldier_Num,//增加兵力属性数值
        Inc_Attack_Num,//增加攻击属性数值
        Inc_Defence_Num,//增加防御属性数值
        Inc_AttackEx_Num,//增加穿刺属性数值
        Inc_DefenceEx_Num,//增加格挡属性数值
        Inc_Skill_Attack_Num,//增加破阵属性数值
        Inc_Skill_Defence_Num,//增加无懈属性数值
        Inc_Tong_Num,//增加统属性数值
        Inc_Yong_Num,//增加勇属性数值
        Inc_Hilly_Fight_Percent,//山地地形战斗力增加百分比
        Inc_Plain_Fight_Percent,//平原地形战斗力增加百分比
        Inc_City_Fight_Percent,//城池地形战斗力增加百分比
        Inc_Water_Fight_Percent,//水域地形战斗力增加百分比
        Inc_Forest_Fight_Percent,//密林地形战斗力增加百分比
        Inc_Mine_Output_Percent,//矿场产量加成百分比

        _Type_Max
    };

public:
    uint Size() const { return m_buff.size(); }
    bool HaveBuff(uint32 id) const;
    bool HaveBuff(Type typ) const;
    void AddBuff(const PlayerBuffTable* table, uint32 lastMsec = 0);
    void DelBuff(uint32 id);

    void DelOneTypeBuff(Type typ);
    void Refresh(uint64 timeNowMsec);

    void SaveToDB(pb::GS2C_Buff_All_Info& msg);
    void SaveToClient(pb::GS2C_Buff_All_Info& msg);
    void LoadFrom(const pb::GS2C_Buff_All_Info& msg);

private:
    std::map<uint32, uint64> m_buff; // <buffId, endTime>
};
