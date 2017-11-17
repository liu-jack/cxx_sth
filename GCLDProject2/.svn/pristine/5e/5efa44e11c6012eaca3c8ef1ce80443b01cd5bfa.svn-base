#pragma once

#include "def/TypeDef.h"

class StopWatch
{
public:
	StopWatch(int64 max = 0)
	:m_max(max)
	,m_tick(0)
	{
	}

	void SetMax(int64 max)
	{
		m_max = max;
	}

	void Update(int64 dt)
	{
		m_tick += dt;
	}

	void Reset()
	{
		m_tick = 0;
	}

	void SetDone()
	{
		m_tick = m_max;
	}

	bool Done() const
	{
		return m_tick >= m_max;
	}

	int64 Exceed() const
	{
		//m_tick -= m_max;
		return m_tick - m_max;
	}

	void SetUndone(int64 delay)
	{
		m_tick = m_max - delay; 
	}

	int64 GetTick()
	{
		return m_tick;
	}

private:
	int64		m_max;
	int64		m_tick;
};
