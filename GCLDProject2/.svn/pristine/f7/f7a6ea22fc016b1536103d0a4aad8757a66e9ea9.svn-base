#pragma once
#include "def/TypeDef.h"

class Character;

class PredCharLevelMatch
{
public:
    explicit PredCharLevelMatch( int lv);
    bool operator () (const Character * chara) const;  
private:
    int m_level; 
};


class PredCharQualityMatch
{
public:
    PredCharQualityMatch( int quality);
    bool operator () (const Character * chara) const;  
private:
    int m_quality; 
};

class PredCharReinforceMatch
{
public:
    PredCharReinforceMatch( int reinforce);
    bool operator () (const Character * chara) const;  
private:
    int m_reinforce; 
};

class PredCharSkillLvMatch
{
public:
    PredCharSkillLvMatch( int lv);
    bool operator () ( const Character * chara) const;  
private:
    int m_level; 
};