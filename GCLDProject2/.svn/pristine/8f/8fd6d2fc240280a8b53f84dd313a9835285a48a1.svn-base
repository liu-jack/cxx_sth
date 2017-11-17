#pragma once
#include <boost/pool/pool.hpp>
#include <boost/thread.hpp>
#include <queue>

//// comment By CaiBingjie 2016-4-23
////=====================================================
//#ifdef malloc
//#define REPLACE_MALLOC
//#undef malloc
//#undef free
//#endif
//
////=====================================================
//#ifdef REPLACE_MALLOC
//#define   malloc(s)             _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
//#define   free(p)               _free_dbg(p, _NORMAL_BLOCK)
//#undef REPLACE_MALLOC
//#endif

//#ifdef _MMO_SERVER_
//#else
//	#define DECLARE_OBJ_POOL(class_name)
//#endif


//=====================================================
class BoostPoolWrapper
{
public:
	BoostPoolWrapper(size_t chunkSize):_pool(chunkSize){}

	void* Malloc() 
	{ 
		boost::mutex::scoped_lock l(_mutex);
		return _pool.malloc(); 
	}

	void Free(void* p) 
	{ 
		boost::mutex::scoped_lock l(_mutex);
		_pool.free(p); 
	}

private:
	boost::mutex	_mutex;
	boost::pool<>	_pool;
};


//=====================================================
#define DECLARE_OBJ_POOL(class_name) \
	public:\
	void* operator new(size_t size)\
    {\
	    return OperatorNew(size);\
    }\
	void operator delete(void * p)\
    {\
	    MemPool().Free(p);\
    }\
	void* operator new(size_t size, int n, const char* file, long line)\
    {\
	    return OperatorNew(size);\
    }\
	void operator delete(void * p, int n, const char* file, long line)\
    {\
	    MemPool().Free(p);\
    }\
	private:\
	static void* OperatorNew(size_t size)\
    {\
	    if (size != sizeof(class_name))\
        {\
	    throw std::exception();\
        }\
	    return MemPool().Malloc();\
    }\
	static BoostPoolWrapper& MemPool()\
    {\
	    static BoostPoolWrapper s_memPool(sizeof(class_name));\
	    return s_memPool;\
    }


//=====================================================
#define VOID_POOL_ID -1
template <class T>
class Pool_Index{ // 自动编号，便于管理(对象要含有m_index变量，记录其内存id)
    T**	   m_arrPtr;
    int    m_num;
    std::queue<int> m_queue;
    boost::mutex	m_mutex;
public:
    Pool_Index(int num) : m_num(num){
        m_arrPtr = (T**)malloc(m_num * sizeof(T*));
        if (!m_arrPtr) return;

        T* pObj = (T*)malloc(m_num * sizeof(T));
        if (!pObj)
		{
			free(m_arrPtr);
			return;
		}

        for (int i = 0; i < m_num; ++i)
        {
            m_arrPtr[i] = pObj++;
            m_queue.push(i);
        }
    }
    bool Double(){
        T** temp = (T**)malloc(m_num * 2 * sizeof(T*));
        if (!temp) return false;

        T* pObj = (T*)malloc(m_num * sizeof(T)); // 开辟新内存块
		if (!pObj)
		{
			free(temp);
			return false;
		}

        memcpy(temp, m_arrPtr, m_num * sizeof(T*));
        free(m_arrPtr);	m_arrPtr = temp;

        for (int i = 0; i < m_num; ++i)
        {
            m_arrPtr[m_num + i] = pObj++;
            m_queue.push(m_num + i);
        }
        m_num *= 2;
        return true;
    }
    T* Alloc(){
        boost::mutex::scoped_lock l(m_mutex);
        if (m_queue.empty() && !Double()) return NULL;
        int id = m_queue.front();
        m_queue.pop();
        m_arrPtr[id]->m_index = id; // 分配时设置内存id
        return m_arrPtr[id];
    }
    void Dealloc(T* p){
        boost::mutex::scoped_lock l(m_mutex);
        m_queue.push(p->m_index);
        p->m_index = VOID_POOL_ID; // 回收后置空内存id
    }
    T* GetByID(int idx){
        if (idx < 0 || idx >= m_num) return NULL;
        return VOID_POOL_ID == m_arrPtr[idx]->m_index ? NULL : m_arrPtr[idx];
    }
};
#define DECLARE_INDEX_POOL(T, size) \
        static Pool_Index<T>& _Pool(){ static Pool_Index<T> pool(size); return pool; } \
        public: \
	    int m_index; \
	    void* operator new(size_t /*size*/){ return _Pool().Alloc(); }\
	    void* operator new(size_t /*size*/, const char* file, int line){ return _Pool().Alloc(); }\
	    void operator delete(void* p, const char* file, int line){ return _Pool().Dealloc((T*)p); }\
	    void operator delete(void* p, size_t) { return _Pool().Dealloc((T*)p); }\
        static T* FindByIdx(int idx){ return _Pool().GetByID(idx); }

