#pragma once

#include "def/TypeDef.h"
#include "def/MmoAssert.h"
//#include "../../../../base/gamedef/ObjectDefines.h"
#include "OS.h"
#include "Logger.h"

#include <boost/smart_ptr.hpp>

template<typename T>
class ObjectPtr
{
public:
	ObjectPtr()
		:_p(NULL)
		 ,_guid(0)
		 ,_threadId(0)
	{
		_mapCount.reset() ;
	}

	explicit ObjectPtr(T* p)
		:_p(NULL)
		 ,_guid(0)
		 ,_threadId(0)
	{
		_mapCount.reset() ;

		Set(p);
	}

	void operator=(T* p)
	{
		Set(p);
	}

	T* Get() const
	{
		if (!IsValid())
		{
			return NULL;
		}

		return _p;
	}


	bool IsValid() const
	{
		if( _p == NULL || _mapCount.expired() )
		{
			return false ;
		}

		boost::shared_ptr<int> sp =_mapCount.lock() ;
		if( !sp || *sp % 2 == 0 )
		{
			return false ;
		}

	/*	if( _threadId != _p->GetCurThreadId() )
		{
			return false ;
		}*/

		MMO_ASSERT( _p->GetGuid() == _guid /*&& *_p->InOutMapCount() == *sp*/ ) ;
		return true ;
	}


	T* operator->()
	{
		MMO_ASSERT(IsValid());

		return _p;
	}

	const T* operator->() const
	{
		MMO_ASSERT(IsValid());

		return _p;
	}

	static bool IsValid(T* p)
	{
		return p != NULL 
			&& p->GetGuid() !=0 
			//&& p->InOutMapCount()
			//&& *p->InOutMapCount() % 2 != 0 // odd number means in map
			//&& p->GetCurThreadId() == sOS.GetCurrentThreadID()
			;
	}

	void SetGuid( ObjGUID guid )
	{
		_guid =guid ;
	}

	ObjGUID GetGuid() const
	{
		return _guid;
	}

	

private:
	void Set(T* p)
	{
		if (IsValid(p))
		{
			_p = p;
			_guid = _p->GetGuid();
			/*_threadId = _p->GetCurThreadId() ;*/
			/*_mapCount = _p->InOutMapCount();*/

			MMO_ASSERT( _threadId != 0 ) ;
		}
		else
		{
			_p = NULL;
			_guid = 0;
			_threadId = 0;
			_mapCount.reset();
		}
	}

private:
	T*                   _p;
	ObjGUID              _guid;
	unsigned long        _threadId;
	boost::weak_ptr<int> _mapCount;
};

