#ifndef __BASE_HH
#define __BASE_HH

// Clears internal memory of an STL object.
// STL clear()/reserve(0) does not always free internal memory allocated
// This function uses swap/destructor to ensure the internal memory is freed.
template<class T>
void STLClearObject(T* obj) 
{
	T tmp;
	tmp.swap(*obj);
	// Sometimes "T tmp" allocates objects with memory (arena implementation?).
	// Hence using additional reserve(0) even if it doesn't always work.
	obj->reserve(0);
}
const char digits[] = "9876543210123456789";
const char* zero = digits + 9;
BOOST_STATIC_ASSERT(sizeof(digits) == 20);

const char digitsHex[] = "0123456789ABCDEF";
BOOST_STATIC_ASSERT(sizeof(digitsHex) == 17);
template<typename T>
size_t convert(char buf[],T value)
{
	T i = value;
	char* p = buf;

	do 
	{
		int lsd = static_cast<int>(i % 10);
		i /= 10;
		*p++ = zero[lsd];
	} while ( i != 0);

	if(value < 0)
	{
		*p++ = '-';
	}
	*p='\0';
	std::reverse(buf,p);
	return p -buf;
}


size_t converHex(char buf[],uintptr_t value)
{
	uintptr_t i = value;
	char* p = buf;

	do 
	{
		int lsd = static_cast<int>(i %16);
		i /= 16;
		*p++ = digitsHex[lsd];
	} while (i != 0);
	*p = '\0';
	std::reverse(buf,p);
	return p - buf;
}


#endif