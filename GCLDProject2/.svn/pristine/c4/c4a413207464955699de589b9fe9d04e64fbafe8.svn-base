#pragma once

#include <vector>
#include <cstddef>

template<class T>
class PtrVector
{
public:
	typedef T*											value_type;
	typedef value_type&									reference;
	typedef const value_type&							const_reference;
	typedef typename std::vector<T*>::iterator			iterator;
	typedef typename std::vector<T*>::const_iterator	const_iterator;
	typedef typename std::vector<T*>::size_type			size_type ;

	PtrVector ()
	{
	}

	explicit PtrVector (int n, const value_type& val = value_type())
		:_vector(n, val)
	{
	}

	~PtrVector()
	{
		delete_all();
	}

	bool empty() const 
	{ 
		return _vector.empty(); 
	}

	size_type size() const
	{
		return _vector.size();
	}

	void resize (size_type n, value_type val = value_type())
	{
		// Replace:
		//_vector.resize(n, val);
		// With:
		if( _vector.size() > n )
		{
			iterator beg =_vector.begin();
			std::advance( beg, n ) ;
			for( iterator itr =beg; itr != _vector.end(); ++itr )
			{
				delete (*itr) ;
				//*itr =NULL ;
			}

			_vector.erase( beg, _vector.end() ) ;
		}
		else
		{
			_vector.resize( n ) ;
		}
		// End
	}

	void clear()
	{
		delete_all();
		_vector.clear();
	}

	void push_back (const value_type& val) 
	{ 
		_vector.push_back(val); 
	}
	
	reference operator[] (size_type n)
	{
		return _vector[n];
	}

	const_reference operator[] (size_type n) const
	{
		return _vector[n];
	}

	iterator erase (iterator position) 
	{ 
		delete (*position);
		return _vector.erase(position); 
	}

    void Delete(size_type n)
    {
        delete (_vector[n]);
        _vector[n] = NULL; 
    }
	
	iterator begin() 
	{ 
		return _vector.begin(); 
	}

	iterator end() 
	{ 
		return _vector.end(); 
	}

	const_iterator begin() const
	{ 
		return _vector.begin(); 
	}

	const_iterator end() const
	{ 
		return _vector.end(); 
	}

	reference back()
	{
		return _vector.back();
	}

	const_reference back() const
	{
		return _vector.back();
	}

    reference front()
    {
        return _vector.front(); 
    }

    const_reference front() const
    {
        return _vector.front();
    }


	void pop_back()
	{
		delete _vector.back();
		_vector.pop_back();
	}

	void pop_back_no_delete()
	{
		_vector.pop_back();
	}

    iterator insert(iterator _Where, const value_type& _Val)
    {
        return _vector.insert(_Where, _Val);
    }

private:
	void delete_all()
	{
		for (iterator it = begin(); it != end(); ++it)
		{
			delete (*it);
		}
	}

	std::vector<T*>	_vector;
};
