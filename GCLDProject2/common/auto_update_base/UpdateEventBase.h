#pragma once

#include "boost/function.hpp"
#include "def/TypeDef.h"
#include "boost/shared_ptr.hpp"

namespace auto_update_enum{
	enum timeCheckPeriod		//��ʱ��ʱ���	��24СʱΪ��׼����������ַ�24��Լ��
	{
		PER_SECOND,			//ÿ��
		PER_HALF_MINUTE,	//ÿ30��
		PER_MINUTE,			//ÿ����
		PER_HALF_HOUR,		//ÿ��Сʱ
		PER_HOUR,			//ÿСʱ
		PER_TWO_HOUR,		//��Сʱ
		PER_THREE_HOUR,		//��Сʱ
		PER_FOUR_HOUR,		//��Сʱ
		PER_SIX_HOUR,		//��Сʱ
		PER_EIGHT_HOUR,		//��Сʱ
		PER_TWELVE_HOUR,	//ʮ��Сʱ
		PER_DAY,			//һ�� 24Сʱ
		PER_WEEK,			//һ�� 7��
		PER_MONTH,			//һ���� 30��
		TIME_NULL,			//��Чʱ��
	};

	enum timeCheckPoint		//��ʱ��׼ʱ���
	{
		TIME_ZERO,			//0��
		TIME_FOUR,			//4��
		TIME_TWENTYTWO,		//22��
		TIME_TWENTYTHREE_AND_TEN,	//23��10��
		TIME_TWENTYTHREE, //23
	};

	enum dayOfWeek
	{
		Sunday,
		Monday,
		Tuesday,
		Wednesday,
		Thursday,
		Friday,
		Saturday,
	};
}

struct IUpdateEventBase
{
	virtual ~IUpdateEventBase() {}

	virtual boost::function<void()>& Handler() = 0;
	virtual void Cancel() = 0;
	virtual auto_update_enum::timeCheckPeriod GetCheckPeriod() const = 0;
	virtual auto_update_enum::timeCheckPoint GetCheckPoint() const = 0;
};

struct UpdateEventBase : public IUpdateEventBase
{
public:
	UpdateEventBase(boost::function<void()>& func, auto_update_enum::timeCheckPeriod checkPeriod, auto_update_enum::timeCheckPoint checkPoint)
		:m_handler(func)
		,m_checkPeriod(checkPeriod)
		,m_checkPoint(checkPoint)
		,m_lastUpdateTime(0)
	{
	}

	void Execute()
	{
		m_handler();
	}

	virtual void Cancel()
	{

	}

	virtual auto_update_enum::timeCheckPeriod GetCheckPeriod() const
	{
		return m_checkPeriod;
	}

	virtual auto_update_enum::timeCheckPoint GetCheckPoint() const
	{
		return m_checkPoint;
	}

	time_t GetLastUpdateTime()
	{
		return m_lastUpdateTime;
	}

	void SetLastUpdateTime(uint64 timenow)
	{
		m_lastUpdateTime = timenow;
	}

	virtual boost::function<void()>& Handler() 
	{
		return m_handler;
	}

private:
	boost::function<void()>		m_handler;
	auto_update_enum::timeCheckPeriod				m_checkPeriod;
	auto_update_enum::timeCheckPoint				m_checkPoint;
	time_t						m_lastUpdateTime;
};
