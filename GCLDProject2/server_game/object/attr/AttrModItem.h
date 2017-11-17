#pragma once

class AttrModItem
{
public:
	AttrModItem(float percent =0, float modValue =0)
		:m_percent(percent),m_value(modValue)
	{
	}

	AttrModItem & operator +=(const AttrModItem& other)
	{
		m_percent += other.m_percent;
		m_value += other.m_value;
		return *this; 
	}

	AttrModItem & operator -=(const AttrModItem& other)
	{
		m_percent -= other.m_percent;
		m_value -= other.m_value;
		return *this; 
	}

    AttrModItem & operator = (const AttrModItem& other)
    {
        m_percent = other.m_percent;
        m_value = other.m_value;
		return *this; 
    }

    void UpdateByStack(float stack)
    {
        m_percent *= stack;
        m_value *= stack;
    }


	// calculate final value from original value
	template<typename T>
	T CalcValue (T oriValue) const
	{
		float val = ((float)oriValue) *(1.0f +m_percent) +m_value;
		return (T)val;
	}

private:
	float	m_percent;
	float	m_value;
};
