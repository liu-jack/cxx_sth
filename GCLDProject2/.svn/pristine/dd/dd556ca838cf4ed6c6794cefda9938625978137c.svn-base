///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 单例类一：继承auto_ptr 指针类型
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef SINGLE_H__
#define SINGLE_H__


#include <assert.h>

#include <memory>

using std::auto_ptr ;

/*
*************************继承auto_ptr 用来当做到单例的指针**************************
*/
template< class T >
class SelfPointer:public auto_ptr<T>
{
public:
	explicit SelfPointer( T *resource ):auto_ptr<T>( resource )
	{}
	virtual ~SelfPointer( void )
	{
		this->reset( NULL ) ;
	}

	operator bool( void ) const
	{
		return auto_ptr<T>::get() != NULL ;
	}

private:
	SelfPointer( const SelfPointer &lhs ) ;
	SelfPointer& operator= ( const SelfPointer &lhs ) ;
};

/*
**************************************单例类**************************************
*/
template< class Type >
class Singleton
{
public:
	Singleton( void )
	{
		/* 禁止随便创建！注意在这里够着函数new会导致无穷递归*/ 
	}

	virtual ~Singleton( void )
	{
		/* 基类需要虚析构函数！*/
		//system("PAUSE") ;
	}

	static SelfPointer<Type>& GetSingletonPtr( void )
	{
		return m_pInstance;
	}

	static Type& Instance( void )
	{
		assert( m_pInstance.get() != NULL ) ;
		return *m_pInstance;
	}

	static Type& NewInstance( void )
	{
		assert( m_pInstance.get() == NULL ) ;
		CheckInstance() ;
		return *m_pInstance;
	}

	static void DeleteInstance( void )
	{
		if( m_pInstance )
		{
			m_pInstance.~SelfPointer() ;
		}
	}

	static void CheckInstance( void )
	{
		if( m_pInstance.get() == NULL )
		{
			m_pInstance.reset( new Type ) ;
		}
	}


	// 私有成员
private:
	static SelfPointer<Type> m_pInstance ; // 静态成员所有类共享，不参与赋值


};
// 静态成员类外初始化
template< class Type >
SelfPointer<Type> Singleton<Type>::m_pInstance( NULL ) ;


#endif
