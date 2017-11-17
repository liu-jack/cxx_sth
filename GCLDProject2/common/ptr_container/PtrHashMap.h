#pragma once

#include <boost/unordered_map.hpp>

template<class TKey, class TValue>
class PtrHashMap
{
public:
	typedef TKey											key_type;
	typedef TValue*											mapped_type;
	typedef typename boost::unordered_map<TKey,TValue*>::iterator		iterator;
	typedef typename boost::unordered_map<TKey,TValue*>::const_iterator	const_iterator;

	PtrHashMap()
	{
		clear() ;
	}

	~PtrHashMap()
	{
		clear() ;
		//delete_all();
	}

	bool empty() const 
	{ 
		return _map.empty(); 
	}

	size_t size() const
	{
		return _map.size();
	}

	void clear()
	{
		delete_all();
		_map.clear();
	}

	iterator find (const key_type& k)
	{
		return _map.find(k);
	}

	const_iterator find (const key_type& k) const
	{
		return _map.find(k);
	}

	mapped_type pfind (const key_type& k) const
	{
		const_iterator it = _map.find(k);
		if (it == _map.end())
		{
			return NULL;
		}
		return it->second;
	}

	mapped_type& operator[] (const key_type& k)
	{
		return _map[k];
	}

	void erase (iterator position) 
	{ 
		delete position->second;
		_map.erase(position); 
	}

	TValue* erase_and_pick (iterator position) 
	{ 
		TValue* p = position->second;
		_map.erase(position); 

		return p;
	}

	size_t erase (const key_type& k)
	{
		iterator it = find(k);
		if (it == end())
			return 0;

		erase(it);
		return 1;
	}

	iterator begin() 
	{ 
		return _map.begin(); 
	}

	const_iterator begin() const
	{ 
		return _map.begin(); 
	}

	iterator end() 
	{ 
		return _map.end(); 
	}

	const_iterator end() const
	{ 
		return _map.end(); 
	}
private:
	void delete_all()
	{
		for (iterator it = begin(); it != end(); ++it)
		{
			delete it->second;
		}
	}

	boost::unordered_map<TKey,TValue*>	_map;
};
