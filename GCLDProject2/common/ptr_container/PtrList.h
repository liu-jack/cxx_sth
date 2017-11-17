#pragma once

#include <list>

template<class T>
class PtrList
{
public:
	typedef T*										value_type;
	typedef typename std::list<T*>::iterator		iterator;
	typedef typename std::list<T*>::const_iterator	const_iterator;

	~PtrList()
	{
		delete_all();
	}

	bool empty() const 
	{ 
		return _list.empty(); 
	}

	void clear()
	{
		delete_all();
		_list.clear();
	}

	void push_back (const value_type& val) 
	{ 
		_list.push_back(val); 
	}
	
	iterator erase (iterator position) 
	{ 
		delete (*position);
		return _list.erase(position); 
	}
	
	iterator begin() 
	{ 
		return _list.begin(); 
	}

	iterator end() 
	{ 
		return _list.end(); 
	}

private:
	void delete_all()
	{
		for (iterator it = begin(); it != end(); ++it)
		{
			delete (*it);
		}
	}

	std::list<T*>	_list;
};