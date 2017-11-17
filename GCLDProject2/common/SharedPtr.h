#pragma once

#include "def/MmoAssert.h"

template<typename T>
class SharedPtr
{
	struct RefPointer
	{
		RefPointer(T* p):_p(p),_ref(1)
		{
		}

		~RefPointer() 
		{ 
			MMO_ASSERT(_ref == 0);
			delete _p; 
		}

		T*		_p;
		int		_ref;
	};

public:
	SharedPtr(T* p)
	{
		MMO_ASSERT(p);
		_data = new RefPointer(p);
	}

	SharedPtr(const SharedPtr& other)
		:_data(other._data)
	{
		_data->_ref ++;
	}

	~SharedPtr()
	{
		MMO_ASSERT(_data->_ref > 0);
		_data->_ref --;

		if (_data->_ref == 0)
		{
			delete _data;				
		}
	}

	T* operator->()
	{
		return _data->_p;
	}

	const T* operator->() const
	{
		return _data->_p;
	}

private:
	RefPointer*		_data;
};