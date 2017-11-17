#pragma once

#include "ByteBuffer.h"

class DoubleBuff
{
public:
	DoubleBuff(void) :m_inputIdx(0)
	{
	}

	ByteBuffer& InBuff()
	{
		return m_buf[m_inputIdx];
	}

	ByteBuffer& OutBuff()
	{
		return m_buf[1- m_inputIdx];
	}

	void SwapInOut()
	{

		m_inputIdx = 1 -m_inputIdx;
	}

private:
	int                m_inputIdx;
	ByteBuffer m_buf[2];
};
