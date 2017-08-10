#ifndef __BASE_HH
#define __BASE_HH

#include <static_assert>
#include <vector>
/* Some usefull function and ideas.
*/
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
static_assert(sizeof(digits) == 20);

const char digitsHex[] = "0123456789ABCDEF";
static_assert(sizeof(digitsHex) == 17);
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

///缩减vector
template <class T>
void shrinkCapcacity(std::vector<T> v)
{
	std::vector<T> tmp(v);//copy elements into a new vector
	v.swap(tmp);		//swap internal vector data
}
///This can remove all = val 
std::vector<int> int_vec;
int_vec.erase(remove(int_vec.begin(),int_vec.end(),val),int_vec.end());

std::vector<int>::iterator pos;
pos = std::find(int_vec.begin(),int_vec.end(),val);
if(pos != int_vec.end())
{
	int_vec.erase(pos);
}

///An empty class used in STL std::allocator
///The Standard C++ language definition says:
///A class with an empty sequence of members and base class objects is an empty class. Complete objects and member subobjects of an empty class type shall have nonzero size. 
// See more http://www.cantrip.org/emptyopt.html
template <typename T>
class allocator
{
	static T* allocate(size_t n)
	{
		return (T*) ::operator new(n * sizeof(T));
	}
};

template <class T,class Alloc = allocator<T> >
class list
{
	struct Node{ .... };

	struct P : public Alloc {
		P(Alloc const& a) : Alloc(a),p(0){}
		Node* p;
	};
	P head_;
public:
	explicit list(Alloc const &a = Alloc())
		:head_(a){....}
	....
};

unsigned int murMurHash(const void *key, size_t len,const int seed)
{
	const unsigned int m = 0x5bd1e995;
	const int r = 24;
	unsigned int h = seed ^ len;
	// Mix 4 bytes at a time into the hash
	const unsigned char *data = (const unsigned char *)key;
	while(len >= 4)
	{
		unsigned int k = *(unsigned int *)data;
		k *= m; 
		k ^= k >> r; 
		k *= m; 
		h *= m; 
		h ^= k;
		data += 4;
		len -= 4;
	}
	// Handle the last few bytes of the input array
	switch(len)
	{
	case 3: h ^= data[2] << 16;
	case 2: h ^= data[1] << 8;
	case 1: h ^= data[0];
		h *= m;
	};
	// Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.
	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;
	return h;
}

erase(iter++);
or
iter = erase(iter);

std::map的operator[]的作用与下面的模板函数相等
template <typename KeyType,typename ValueType>  
void InsertOrUpdateItem( std::map<KeyType, ValueType>& mapList, KeyType key, ValueType tValue )  
{  
	typename std::map<KeyType, ValueType>::iterator iter = mapList.find( key );  
	if ( iter != mapList.end() )  
	{  
		iter->second = tValue;   
		return;  
	}  
	else  
	{  
		mapList.insert( iter, std::make_pair( key, tValue ) );  
	}  
}



#endif