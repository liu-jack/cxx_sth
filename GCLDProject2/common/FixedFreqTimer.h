#pragma once

#include "def/MmoAssert.h"

class FixedFreqTimer
{
public:
	FixedFreqTimer(int freq)
	:m_freq(freq)
	,m_tick(0)
	{
		ASSERT(freq >0);
	}

	void Update(int dt)
	{
		ASSERT(dt >= 0);
		m_tick += dt;
	}

	int Count() const
	{
		return m_tick / m_freq;
	}

	void ResetCount()
	{
		m_tick = m_tick % m_freq;
	}

	void Reset()
	{
		m_tick = 0;
	}

    int Trigger(int dt)
    {
        m_tick += dt;
        int count = m_tick / m_freq;
        if(count)
            ResetCount();
        return count;
    }

private:
	int		m_freq;
	int		m_tick;
};