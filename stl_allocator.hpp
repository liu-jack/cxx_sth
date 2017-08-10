


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
//n must be > 0
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

template<bool threads,int inst>
static void *__default_alloc_template<threads,inst>::deallocate(void *p,size_t n)
{
	obj * q = (obj *)p;
	obj * volatile * my_free_list;
	if(n > (size_t)__MAX_BYTES)
	{
		malloc_alloc::deallocate(p,n);
		return;
	}

	my_free_list = free_list + FREELIST_INDEX(n);
	q->free_list_link = *my_free_list;
	*my_free_list = q;
}

template<bool threads ,int inst>
void *__default_alloc_template<threads,inst>::refill(size_t n)
{
	int nobjs = 20;
	char *chunk = chunk_alloc(n,objs);
	obj * volatile * my_free_list;


	obj* result;
	obj *current_obj,*next_obj;
	if(1 == nobjs) return chunk;
	my_free_list = free_list + FREELIST_INDEX(n);

	result = (obj*)chunk;
	*my_free_list = next_obj = (obj*)(chunk + n);

	for(int i = 0;;++i)
	{
		current_obj = next_obj;
		next_obj = (obj*)((char*) next_obj + n);
		if(nobjs - 1 == i){
			current_obj->free_list_link = 0;
			break;
		}else {
			current_obj->free_list_link = next_obj;
		}
	}
	return( result ) ;
}


template<bool threads,int inst>
char* __default_alloc_template<threads,inst>::chunk_alloc(size_t size,int& nobjs)
{
	char * result;
	size_t total_bytes = size * nobjs;

	size_t bytes_left = end_free - start_free;

	if(bytes_left >= total_bytes){
		result = start_free;
		start_free += total_bytes;
		return (result);
	}else if(bytes >= size){
		nobjs = bytes_left/size;
		
	}
}
//////////////////////////////////////////////////////////////////////

#if 0
#include <new>
#define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
#include <iostream.h>
#define __THROW_BAD_ALLOC cerr << "out of memory " << endl;exit(1);
#endif

template <class T,class Alloc>
class simple_alloc
{
	static T* allocate(size_t n)
	{
		return 0 == n ? 0 : (T*) Alloc::allocate(n *sizeof(T));
	}
	static T*allocate(void)
	{
		return (T*)Alloc::allocate(sizeof(T));
	}

	static void deallocate(T* p,size_t n)
	{
		if(0 != n) Alloc::deallocate(p,sizeof(T)*n);
	}
	static void deallocate(T*p)
	{
		Alloc::deallocate(p,sizeof(T));
	}
};


template<int inst>
class __malloc_alloc_template{
private:
	static void *oom_malloc(size_t);
	static void *oom_realloc(void *,size_t);
	static void (*__malloc_alloc_oom_handler)();

public:
	static void * allocate(size_t n)
	{
		void *result = malloc(n);
		if(0 == result) result = oom_malloc(n);
		return result;
	}

	static void deallocate(void* p,size_t n)
	{
		free(p);
	}

	static void *reallocate(void *p,size_t n,size_t new_size)
	{
		void *result = realloc(p,new_size);
		if(0 == result) result = oom_realloc(p,new_size);
		return result;
	}

	static void (*set_malloc_handler(void (*f)()))()
	{
		void (*old)() = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = f;
		return old;
	}
};

template <int inst>
void T* __malloc_alloc_template<inst>::__malloc_alloc_oom_handler = 0;

template <int inst>
void T* __malloc_alloc_template<inst>::oom_malloc(size_t n)
{
	void (* my_malloc_handler)();
	void *result;

	for(;;)
	{
		my_malloc_handler = __malloc_alloc_oom_handler;
		if(0 == my_malloc_handler) { __THROW_BAD_ALLOC;}
		(*my_malloc_handler)();
		result = malloc(n);
		if(result) return (result);
	}
}

template <int inst>
void T* __malloc_alloc_template<inst>::oom_realloc(void *p,size_t n)
{
	void (*my_malloc_handler)();
	void * result;

	for(;;)
	{
		my_malloc_handler = __malloc_alloc_oom_handler;
		if(0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
		(*my_malloc_handler)();
		result = realloc(p,n);
		if(result) return (result);
	}
}

typedef __malloc_alloc_template<0> malloc_alloc;
