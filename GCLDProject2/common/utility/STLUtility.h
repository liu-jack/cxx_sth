
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Creator:
// This file add by CaiBingjie 2010-10-16
//
// Porpuse:
// To add some use in STL 
//
//
///

#ifndef SHARED_STL_UTILITY_H__
#define SHARED_STL_UTILITY_H__


/*
 * 头文件
 */
#include <map>
#include <vector>
#include <set>
#include <iterator>
#include <algorithm>
#include <functional>

// 结构
using std::map ;
using std::pair ;
using std::vector ;
using std::iterator ;
using std::output_iterator_tag ;


// 方法
using std::find_if ;
using std::upper_bound ;
using std::lower_bound ;
using std::equal_range ;
using std::binary_search ;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 迭代器别名
#define DeqItr( Type )                  deque< Type >::iterator

#define VecItr( Type )                  vector< Type >::iterator
#define cVecItr( Type )                 vector< Type >::const_iterator
#define RVecItr( Type )                 vector< Type >::reverse_iterator
#define VecPairItr( first, second )     vector< pair<first, second> >::iterator

#define MapItr( first, second )         map< first, second >::iterator


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template< typename T>
void Get_InA_NotInB(std::set<T>& vecA, std::set<T>& vecB, std::vector<T>& outVec)
{
    outVec.clear();
    size_t largerSize = vecA.size() > vecB.size() ? vecA.size() : vecB.size(); 
    outVec.resize(largerSize);
    typename std::vector<T>::iterator it = std::set_difference(vecA.begin(), vecA.end(), vecB.begin(), vecB.end(),outVec.begin());
    outVec.resize(it - outVec.begin());     
}
/*
 * Begin
 */
// 用于 for_each 中 delete 资源
struct DeleteTheValue
{
	template< typename T >
	void operator()( T &pointer ) const
	{
		if( pointer )
		{
			delete pointer ;
			pointer = NULL ;
		}
	}
};

// 使用
// vector< pair< SuitId, EquipmentSuitInfo* > >
// std::for_each( m_equipmentSuitTable.begin(), m_equipmentSuitTable.end(), DeleteSecondValue() ) ;

struct DeleteSecondValue
{
	template< typename T >
	void operator()( T &pair ) const
	{
		if( pair.second )
		{
			delete pair.second ;
			pair.second = NULL ;
		}
	}
};



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 减少空间
template< class Container >
inline void ReduceCapacity( Container &container )
{
	if( container.capacity() > container.size() )
	{
		Container( container ).swap( container ) ;
	}
}


/*
 * End
 */




/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数对象适配器

/*
 * Begin
 */
///////////////////////////////////////////////////////////////////////////////////////////////////
// 将 pair 的第一个值，传给一个函数对象当多参数
template< class FunObj >
struct CSelectFirst
{
public:
	CSelectFirst( FunObj &funObj ):m_funObj( funObj ) {}

	template< class Pair >
	typename FunObj::result_type operator()( Pair &pair ) const
	{
		return m_funObj( pair.first ) ;
	}

public:
	const FunObj &m_funObj ;
};

// 使用简化函数
// 	for_each( m_randomRecords.begin(), m_randomRecords.end(), SelectTheSecond( mem_fun_ref( &RandomRecord::SaveToDatabase ) ) );
template< class FunObj >
inline CSelectFirst< FunObj > SelectFirst( FunObj &funObj )
{
	return CSelectFirst< FunObj >( funObj ) ;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 使用间例：
// 将 pair 的第二个值，传给一个函数对象当多参数
template< class FunObj >
struct CSelectSecond
{
public:
	CSelectSecond( const FunObj &funObj ):m_funObj( funObj ) {}

	template< class Pair >
	typename FunObj::result_type operator()( Pair &pair ) const
	{
		return m_funObj( pair.second ) ;
	}

public:
	const FunObj &m_funObj ;
};

// 使用简化函数
// map< uint32, RandomRecord >
// void RandomRecord::SaveToDatabase( void )
// for_each( m_randomRecords.begin(), m_randomRecords.end(), SelectTheSecond( mem_fun_ref( &RandomRecord::SaveToDatabase ) ) );
template< class FunObj >
inline CSelectSecond< FunObj > SelectSecond( const FunObj &funObj )
{
	return CSelectSecond< FunObj >( funObj ) ;
}
/*
 * End
 */



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// copy 使用配合插入迭代器

/*
 * Begin
 */
// 赋值第一个
template< class OutPutTarget >
class CCopyFirst:public iterator< output_iterator_tag, void, void, void, void >
{
public:
	explicit CCopyFirst( OutPutTarget &target ):m_target( target ) { /**/ }

	CCopyFirst& operator*() {   return *this ;   }
	CCopyFirst& operator++() {   return *this ;   }
	CCopyFirst& operator++(int) {   return *this ;   }

	template< class Pair >
	CCopyFirst& operator=( const Pair &pair )
	{
		m_target =pair.first ;
		return *this ;
	}

private:
	OutPutTarget &m_target ;
} ;

// 使用简化函数
// std::map<uint32, BaseSoldier*>
// copy( pTargetSet->begin(), pTargetSet->end(), CopyFirst( back_inserter( targets ) ) ) ;
template< class AssignObject >
inline CCopyFirst<AssignObject> CopyFirst( AssignObject &object )
{
	return CCopyFirst<AssignObject>( object ) ;
}

// 赋值第二个
template< class OutPutTarget >
class CCopySecond:public iterator< output_iterator_tag, void, void, void, void >
{
public:
	explicit CCopySecond( OutPutTarget &target ):m_target( target ) { /**/ }

	CCopySecond& operator*() {   return *this ;   }
	CCopySecond& operator++() {   return *this ;   }
	CCopySecond& operator++(int) {   return *this ;   }

	template< class Pair >
	CCopySecond& operator=( const Pair &pair )
	{
		m_target =pair.second ;
		return *this ;
	}

private:
	OutPutTarget &m_target ;
} ;

// 使用简化函数
// map<uint32,uint32>
// copy( m_newestSkill.begin(), m_newestSkill.end(), CopySecond( back_inserter( vecSkillList ) ) ) ; // 学习技能
template< class AssignObject >
inline CCopySecond<AssignObject> CopySecond( AssignObject &object )
{
	return CCopySecond<AssignObject>( object ) ;
}

/*
 * End
 */



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 泛型使用 << 迭代操作

/*
 * Begin
 */
// char delimiter =',';
// std::vector<int> 
// std::copy( m_curSkills.begin(), m_curSkills.end(), SeeOutValue<stringstream>( strStream, &delimiter ) ) ;
template <class SeeOutObject, class Delimiter =char>
class SeeOutValue:public iterator< output_iterator_tag, void, void, void, void >
{
public:
	SeeOutValue( SeeOutObject &obj, const Delimiter *pDem =NULL ):m_seeOutObject( obj ), m_pDelimiter( pDem ) {   /**/   }

	~SeeOutValue() {   /**/   }

	template< class Value >
	SeeOutValue& operator= (const Value &value )
	{
		m_seeOutObject<< value;
		if ( m_pDelimiter != NULL )
		{
			m_seeOutObject<< *m_pDelimiter;
		}
		return *this;
	}

	SeeOutValue& operator*() { return *this; }
	SeeOutValue& operator++() { return *this; }
	SeeOutValue& operator++(int) { return *this; }

private:
	const Delimiter *m_pDelimiter ;
	SeeOutObject &m_seeOutObject ;
};


// 使用：
// map<uint32, SkillInfo*>
// copy( m_skillTable.begin(), m_skillTable.end(), SeeOutFirst<WorldPacket>( pack ) ) ;
template <class SeeOutObject, class Delimiter =char>
class SeeOutFirst:public iterator< output_iterator_tag, void, void, void, void >
{
public:

	SeeOutFirst( SeeOutObject &obj, const Delimiter *pDem =NULL ):m_seeOutObject( obj ), m_pDelimiter( pDem ) {   /**/   }

	~SeeOutFirst() {   /**/   }

	template< class Pair >
	SeeOutFirst& operator= (const Pair& pair)
	{
		m_seeOutObject<< pair.first;
		if ( m_pDelimiter != NULL )
		{
			m_seeOutObject<< *m_pDelimiter;
		}
		return *this;
	}

	SeeOutFirst& operator*() { return *this; }
	SeeOutFirst& operator++() { return *this; }
	SeeOutFirst& operator++(int) { return *this; }

private:
	const Delimiter *m_pDelimiter ;
	SeeOutObject &m_seeOutObject ;
};


// 使用：
// map<uint32,uint32>
// std::copy( m_newestSkill.begin(), m_newestSkill.end(), SeeOutSecond<stringstream>( strStream, &delimiter ) ) ;
template <class SeeOutObject, class Delimiter =char>
class SeeOutSecond:public iterator< output_iterator_tag, void, void, void, void >
{
public:
	SeeOutSecond( SeeOutObject &obj, const Delimiter *pDem =NULL ):m_seeOutObject( obj ), m_pDelimiter( pDem ) {   /**/   }

	~SeeOutSecond() {   /**/   }

	template< class Pair >
	SeeOutSecond& operator= (const Pair& pair)
	{
		m_seeOutObject<< pair.second;
		if ( m_pDelimiter != NULL )
		{
			m_seeOutObject<< *m_pDelimiter;
		}
		return *this;
	}

	SeeOutSecond& operator*() { return *this; }
	SeeOutSecond& operator++() { return *this; }
	SeeOutSecond& operator++(int) { return *this; }

private:
	const Delimiter *m_pDelimiter ;
	SeeOutObject &m_seeOutObject ;
};
/*
 * End
 */




/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 查找辅助

/*
 * Begin
 */
///////////////////////////////////////////////////////////////////////////////////////////////////
// 二分查找 ( 需要可以使用默认模板参数指定比较器 )
// itr =BSearch( vPair.begin(), vPair.end(), 5, less<int>() ) ;
template< class Itr, class Key, class LessCmp >
Itr BSearch( Itr &beg, Itr &end, const Key &key, const LessCmp &less )
{
	Itr notFind( end ) ;

	int diff =( ( end - beg )/2 ) ;
	Itr mid = beg + diff ;

	while( beg < end )
	{
		if( !less( *mid, key ) && !less( key, *mid ) )
		{
			return mid ;
		}

		if( less( *mid, key ) )
		{
			beg =mid + 1 ;
		}
		else
		{
			end =mid ;
			
		}

		mid = beg + ( end - beg ) / 2 ;
	}

	return notFind ;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Tips: 用在还没有排序的
// 根据键值来查找元素, 使用给 find_if 使用, 没有排序的
template< class FirstType >
struct CompareFirst
{
public:
	CompareFirst( FirstType &firstType ): first( firstType ) {}

	template< class PairType >
	bool operator()( const PairType &pair ) const
	{
		return pair.first == first ;
	}

public:
	FirstType &first ;
};
/*
 * End
 */


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 循环

/*
 * Begin
 */
///////////////////////////////////////////////////////////////////////////////////////////////////
// 条件遍历
template< class ITR, class HandleObj, class IfObj >
inline void for_each_if( ITR &beg, ITR &end, HandleObj &handleObj, IfObj &ifObj )
{
	for( ITR cur =beg ; cur != end; ++cur )
	{
		if( ifObj( *cur ) )
		{
			handleObj( *cur ) ;
		}
	}
}
/*
 * End
 */



/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * Begin
 */
//--- 以顺序容易代替Map使用的pair比较函数
template< class firstType, class secondType >
class PairCompare
{
private:
	typedef pair< firstType, secondType > Pair ;

public: 
	 // comparison func  for sorting
	bool operator()( const Pair &lhs, const Pair &rhs ) const
	{
		return keyLess( lhs.first, rhs.first) ; // keyLess is below
	}

	// comparison func for lookups  (form 1)
	bool operator()( const Pair &lhs, const typename Pair::first_type &k ) const
	{
		return keyLess( lhs.first, k );
	}

	// comparison func for lookups (form 2)
	bool operator()( const typename Pair::first_type &k, const Pair &rhs ) const
	{
		return keyLess( k, rhs.first );
	}

private:
	// the "real" comparison function
	bool keyLess( const typename Pair::first_type &k1, const typename Pair::first_type &k2 ) const
	{
		return k1 < k2;
	}
};

// TODO: 为指针特化一个版本



///////////////////////////////////////////////////////////////////////////////////////////////////
// 排序
// SortVecPair( Vector< pair > ) ;
template< class VecPair >
inline void SortVecPair( VecPair &vec )
{
	sort( vec.begin(), vec.end(), PairCompare< typename VecPair::value_type::first_type, typename VecPair::value_type::second_type>() ) ;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// Tips: 用在已排序之后
// 是否存在,( 使用二分查找请在sort之后使用，否则 使用 CanFindInVecPair 或者 FindInVecPair )
// IsInVecPair( Vector< pair >, key ) ;
template< class VecPair >
inline bool IsInVecPair( VecPair &vec, typename VecPair::value_type::first_type &key )
{
	return binary_search( vec.begin(), vec.end(),key, PairCompare< typename VecPair::value_type::first_type, typename VecPair::value_type::second_type>() ) ;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Tips: 用在还没有排序的
// CanFindInVecPair( Vector< pair >, key ) ;
template< class VecPair >
inline bool CanFindInVecPair( VecPair &vec, typename VecPair::value_type::first_type &key )
{
	return find_if( vec.begin(), vec.end(), CompareFirst< typename VecPair::value_type::first_type >( key ) ) != vec.end() ;
}

template< class VecPair >
inline typename VecPair::iterator FindInVecPair( VecPair &vec, typename VecPair::value_type::first_type &key )
{
	return find_if( vec.begin(), vec.end(), CompareFirst< typename VecPair::value_type::first_type >( key ) ) ;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// 以下在 Vector< pair > 模仿 multimap 的时候使用
// 找到 键值 >= key 的第一个数
// GetVecPairLower( Vector< pair >, key, Vector< pair >::iterator ) ;
template< class VecPair >
inline void GetVecPairLower( VecPair &vec, const typename VecPair::value_type::first_type &key, typename VecPair::iterator &itr )
{
	itr =lower_bound( vec.begin(), vec.end(), key, PairCompare< typename VecPair::value_type::first_type, typename VecPair::value_type::second_type>() ) ;
}


// 找到 键值 > key 的第一个数
// GetVecPairUpper( Vector< pair >, key, Vector< pair >::iterator ) ;
template< class VecPair >
inline void GetVecPairUpper( VecPair &vec, const typename VecPair::value_type::first_type &key, typename VecPair::iterator &itr )
{
	itr =upper_bound( vec.begin(), vec.end(), key, PairCompare< typename VecPair::value_type::first_type, typename VecPair::value_type::second_type>() ) ;
}


// 找到一段范围的数
// pair< itr, itr > pairItr ;
// GetVecPairRange( Vector< pair >, key, pairItr ) ;
template< class VecPair >
inline void GetVecPairRange( VecPair &vec, const typename VecPair::value_type::first_type &key, pair< typename VecPair::iterator, typename VecPair::iterator> &pariItr )
{
	pariItr =equal_range( vec.begin(), vec.end(), key, PairCompare< typename VecPair::value_type::first_type, typename VecPair::value_type::second_type>() ) ;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// 以下在 Vector< pair > 模仿 map 的时候使用
// 简化查询的过程( 经过测试两个效率差不多 )
template< class VecPair >
inline bool VecPairFind( VecPair &vec, const typename VecPair::value_type::first_type &key, typename VecPair::iterator &itr )
{
	itr =lower_bound( vec.begin(), vec.end(), key, PairCompare< typename VecPair::value_type::first_type, typename VecPair::value_type::second_type>() ) ;
	if( itr != vec.end() && key != itr->first )
	{
		itr =vec.end() ;
	}

	return itr == vec.end() ;
}

template< class VecPair >
inline typename VecPair::iterator VecPairSearch( VecPair &vec, const typename VecPair::value_type::first_type &key )
{
	return BSearch( vec.begin(), vec.end(), key, PairCompare< typename VecPair::value_type::first_type, typename VecPair::value_type::second_type>() ) ;
}
/*
 * End
 */



///////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * Begin
 */
// 2012-10-12 Add By CaiBingjie
// 智能指针
template< typename T >
class SmartPointer
{
public:
	//SmartPointer( void ) ;
	SmartPointer( T *ptr = NULL ) ;
	~SmartPointer( void ) ;
	SmartPointer( const SmartPointer &rhs ) ;
	SmartPointer& operator= ( const SmartPointer &rhs ) ;

	//转换操作符
	operator bool( void )  const
	{
		return m_pResource != NULL ;
	}

	void SetSelfResource( T *newResource ) ;
	//void SetShareResource( T *newResource ) ; // 不是二级指针, 想错了

	T& operator* ( void ) ;
	T& operator* ( void ) const ;

	T* operator->( void ) ;
	T* operator->( void ) const ;

	T* GetResource( void ) ; // 2012-12-21 

private:
	void destory( void ) ;
	//void CreateNewOne( void ) ;    //如果没有那个类的话就是用默认构造函数直接创建一个

private:
	T *m_pResource ;
	size_t *m_pUseCounter ;  //注意--要使用指针
};


//构造函数
//template< typename T >
//SmartPointer<T>::SmartPointer( void ): m_pResource( NULL ), m_pUseCounter( new size_t( 1 ) ) {  /**/ }
template< typename T >
SmartPointer<T>::SmartPointer( T *ptr ): m_pResource( ptr ), m_pUseCounter( new size_t( 1 ) ) { /**/ }

//复制构造
template< typename T >
SmartPointer<T>::SmartPointer( const SmartPointer &rhs ): m_pResource( rhs.m_pResource ), m_pUseCounter( rhs.m_pUseCounter ) 
{
	++*m_pUseCounter ; 
}

template< typename T >
inline SmartPointer<T>::~SmartPointer()
{
	destory() ;
}

//赋值函数
template< typename T >
inline SmartPointer<T>& SmartPointer<T>::operator= ( const SmartPointer &rhs )
{
	++*rhs.m_pUseCounter ;                    // protect against self-assignment
	destory() ;                         // decrement use count and delete pointers if needed
	m_pResource = rhs.m_pResource ;
	m_pUseCounter = rhs.m_pUseCounter ;
	return *this ;
}


//设置自己指向新的资源
template< typename T >
inline void SmartPointer<T>::SetSelfResource( T *newResource )
{
	if( !newResource || m_pResource == newResource )
	{
		return ; //唔好玩耶~！！
	}
	destory() ;  //减少原来的使用计数

	m_pUseCounter = new size_t( 1 )  ;
	m_pResource = newResource ;
}

template< typename T >
inline T* SmartPointer<T>::GetResource( void ) // 2012-12-21 
{
	return m_pResource ;
}

// 销毁
template< typename T >
inline void SmartPointer<T>::destory( void )
{
	if( m_pUseCounter && --*m_pUseCounter <= 0 )
	{
		if( m_pResource )
		{
			delete m_pResource ;
		}
		delete m_pUseCounter ;
	}
	m_pUseCounter = NULL ;
	m_pResource = NULL ;
}

template< typename T >
inline T& SmartPointer<T>::operator* ( void )
{
	//if( !m_pResource )
	//{
	//	CreateNewOne() ;
	//}
	return *m_pResource ;
}

template< typename T >
inline T& SmartPointer<T>::operator* ( void ) const
{
	//if( !m_pResource )
	//{
	//	CreateNewOne() ;
	//}
	return *m_pResource ;
}

template< typename T >
inline T* SmartPointer<T>::operator->( void )
{
	//if( !m_pResource )
	//{
	//	CreateNewOne() ;
	//}
	return m_pResource ;
}

template< typename T >
inline T* SmartPointer<T>::operator->( void ) const
{
	//if( !m_pResource )
	//{
	//	CreateNewOne() ;
	//}
	return m_pResource ;
}


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


/*
*END
*/

#endif
