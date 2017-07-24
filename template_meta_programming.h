

#ifndef __TEMPLATE_META_PROGRAMMING__H
#define __TEMPLATE_META_PROGRAMMING__H

namespace zhoulunhao
{
	template< typename T,size_t N>
	T power()
	{
		enum 
		{
			value *= power<T,N-1>::value,
		};
		return value;
	}

	template<T>
	T power<T,0>
	{
		enum
		{
			value = 1,
		}
		return value;
	}
}



#endif