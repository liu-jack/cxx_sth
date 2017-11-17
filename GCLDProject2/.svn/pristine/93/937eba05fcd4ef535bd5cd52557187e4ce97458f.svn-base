
#include "struct_achievement.h"

Achievement::Achievement()
{
	INIT_MEMBER_DATA;
	for(int i=0; i<MAX_TREE_NUM; i++)
	{
		m_trees[i] = -1;
	}

	m_typeNum = 1;

	for(int i=0; i<MAX_TYPE_NUM; i++)
	{
		m_type[i] = 0;//0 means no limit
		m_targetType[i] = 0;
		m_targetValue[i] = 1;
		m_curValue[i] = 0;
		m_str[i] = 0;
	}
	m_str[MAX_TYPE_NUM] = 0;

	m_bonus = 10;
	m_completed = false;
	m_level = 0;
}
//
//Achievement::~Achievement()
//{
//
//}



bool Achievement::Completed()
{
	return m_completed;
}

bool Achievement::UpdateProcess( int type, int targetType, int addValue , bool add)
{
	if(m_completed)
		return false;

	//bool achieve = true;
	for(int i=0; i<m_typeNum; i++)
	{
		if(m_type[i] == 0 || (m_type[i] == type && m_targetType[i] == targetType) || (m_type[i] == type && m_targetType[i] == 0))
		{
			if(add)
			{
				m_curValue[i] += addValue;
			}
			else
			{
				m_curValue[i] = addValue;
			}
		}
	}
	for(int i=0; i<m_typeNum; i++)
	{
		if(m_curValue[i] < m_targetValue[i])
		{
			return false;
		}
	}
	m_completed = true;
	return true;
}

void Achievement::LoadFromAchievement( const Achievement* other )
{
	if(Id != other->Id)
	{
		return;	
	};
	for(int i=0; i<MAX_TREE_NUM; i++)
	{
		m_trees[i] = other->m_trees[i];
	}
	m_typeNum = other->m_typeNum;
	for(int i=0; i<MAX_TYPE_NUM; i++)
	{
		m_type[i] = other->m_type[i];
		m_targetType[i] = other->m_targetType[i];
		m_targetValue[i] = other->m_targetValue[i];
		m_str[i] = other->m_str[i];
	}
	m_str[MAX_TYPE_NUM] = other->m_str[MAX_TYPE_NUM];
	m_bonus = other->m_bonus;
	m_level = other->m_level;
}

Achievement_Type::Achievement_Type()
{
	INIT_MEMBER_DATA;
};
