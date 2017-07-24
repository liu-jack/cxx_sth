

#ifndef __TEMPLATE_META_PROGRAMMING__H
#define __TEMPLATE_META_PROGRAMMING__H


///No I'm wrong.Function template can't be partial specialization.
namespace zhoulunhao
{
	template< size_t N>
	struct Factorial
	{
		enum { value = N * Factorial<N-1>::value};
	};
	template<>
	struct Factorial<0>
	{
		enum { value = 1};
	};

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