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

enum {__ALIGN = 8};//小区块的上调边界
enum {__MAX_BYTES = 128};//max num of CHUNK
enum {__NFREELISTS = __MAX_BYTES / __ALIGN};//fres list number
template <bool threads,int inst>
class __default_alloc_template
{
private:
	static size_t ROUND_UP(size_t bytes)
	{
		return ((bytes) + __ALIGN - 1) & ~(__ALIGN - 1);
	}
private:
	union obj{
		union obj * free_list_link;
		char client_data[1];//the client sees that.
	};

private:
	static obj * volatile free_list[__NFREELISTS];
	static obj * FREELIST_INDEX(size_t bytes)
	{
		return (((bytes) + __ALIGN -1)/__ALIGN -1);
	}
	static void *refill(size_t n);
	static char* chunk_alloc(size_t size,int &nobjs);

	static char* start_free;//just change in chunk_alloc()
	static char* end_free;//just change itself in chunk_alloc()
	static size_t heap_size;
public:
	static void *allocate(size_t n);
	static void deallocate(void* p,size_t n);
	static void *reallocate(void *p,size_t old_size,size_t new_size);
};

template <bool threads,int inst>
void * __default_alloc_template<threads, inst>::refill(size_t n)
{
	void *chunk = chunk_alloc(n,objs);
	void *volatile * my_free_list;
	
}

template <bool threads,int inst>
void __default_alloc_template<threads, inst>::deallocate(void* p,size_t n)
{
	obj* q = (obj*)p;
	obj* volatile * my_free_list;

	if(n > (size_t)__MAX_BYTES)
	{
		malloc_alloc::deallocate(p,n);
		return;
	}
	my_free_list = free_list + FREELIST_INDEX(n);
	q->free_list_link = *my_free_list;
	*my_free_list = q;
}

template <bool threads,int inst>
char * __default_alloc_template<threads,inst>::start_free = 0;

template <bool threads,int inst>
char * __default_alloc_template<threads,inst>::end_free = 0;

template<bool threads,int inst>
size_t __default_alloc_template<threads,inst>::heap_size = 0;

template <bool threads,int inst>
__default_alloc_template<threads,inst>::obj* volatile 
__default_alloc_template<threads,inst>::free_list[__NFREELISTS] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

template<bool threads,int inst>
static void *__default_alloc_template<threads,inst>::allocate(size_t n)
{
	obj* volatile * my_free_list;
	obj * result;
	if(n > (size_t)__MAX_BYTES){
		return (malloc_alloc::allocate(n));
	}

	my_free_list = free_list + FREELIST_INDEX(n);
	result = *my_free_list;
	if(result == 0)
	{
		void *r = refill(ROUND_UP(n));
		return r;
	}

	*my_free_list = result->free_list_link;
	return (result);
}

#endif