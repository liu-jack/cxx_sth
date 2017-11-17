#pragma once

#include <map>
#include <deque>
#include <queue>

#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include <boost/function.hpp>
#include <boost/lockfree/queue.hpp>

#include "Mutex.h"
#include "../def/MmoAssert.h"
#include "../utility/STLUtility.h"

#include <iostream>


#define ServerQueue BLFQueue
//#define ServerQueue LockFreeQueue 

template <class T>
class LockFreeQueue 
{
	public:
		LockFreeQueue( void ) {}

		~LockFreeQueue( void )
		{
		}

	public:
		bool IsEmpty() const
		{
			MutexGuard lock( m_mutex );

			return m_queue.empty();
		}

		void Enqueue( const T& item)
		{
			MutexGuard lock( m_mutex );

			m_queue.push(item);
		}

		bool Dequeue(T& item)
		{
			MutexGuard lock( m_mutex );

			if (m_queue.empty())
			{
				return false;
			}

			item = m_queue.front();
			m_queue.pop();

			return true;
		}
		int Size() const {return m_queue.size();}

	private:
		std::queue<T> m_queue;
		mutable Mutex         m_mutex;
};

template<typename T>
void DeleteAllPtrInLFQueue(LockFreeQueue<T>& c)
{
	T p;

	while (c.Dequeue(p))
	{
		delete p;
	}
}

// boost lock free queue
template <class T>
class BLFQueue 
{
	public:
		BLFQueue( int capacity =1024 ) :m_deque( capacity )
	{
	}

		~BLFQueue( void )
		{
		}

	public:
		bool IsEmpty() const
		{
			return m_deque.empty();
		}

		inline void Enqueue( T const &item)
		{
			bool isOk =m_deque.push( item ) ;
			ASSERT( isOk ) ;
		}

		inline bool Dequeue(T& item)
		{
			return m_deque.pop( item ) ;
		}

		//int Size() const {return m_deque.size();}

	private:
		boost::lockfree::queue<T,boost::lockfree::fixed_sized<false>> m_deque ;
};



template<typename T>
void DeleteAllPtrInLFQueue(BLFQueue <T>& c)
{
	T p;

	while (c.Dequeue(p))
	{
		delete p;
	}
}


//template< class T >
//class BufferLockQueue
//{
//	public:
//		BufferLockQueue( void ) { }
//
//		BufferLockQueue( const BufferLockQueue &rhs )
//			: m_deque( rhs.m_deque ) { }
//
//		virtual ~BufferLockQueue( void ) { }
//
//	public:
//		inline void Enqueue( const T &t )
//		{
//			MutexGuard lock( m_mutex ) ;
//			m_deque.push_back( t );
//		}
//
//		inline bool GetAll( std::deque<T> &table )
//		{
//			ASSERT( table.empty() ) ;
//
//			MutexGuard lock( m_mutex ) ;
//			table.swap( m_deque ) ;
//
//			return !table.empty() ;
//		}
//
//	protected:
//		Mutex         m_mutex ;
//		std::deque<T> m_deque ;
//} ;
//
//
//template< class T >
//class PointBufferLockQueue : public BufferLockQueue< T >
//{
//	public:
//		typedef BufferLockQueue<T> Base ;
//
//	public:
//		PointBufferLockQueue( void ) { }
//
//		virtual ~PointBufferLockQueue( void )
//		{
//			{
//				std::for_each( Base::m_deque.begin(), Base::m_deque.end(), DeleteTheValue() ) ;
//				Base::m_deque.clear() ;
//			}
//		}
//} ;


class Spinlock
{
	private:
		typedef enum {Locked, Unlocked} LockState;
		boost::atomic<LockState> state_;

	public:
		Spinlock() : state_(Unlocked) {}

		void lock()
		{
			while (state_.exchange(Locked, boost::memory_order_acquire) == Locked) {
				/* busy-wait */
			}
		}
		void unlock()
		{
			state_.store(Unlocked, boost::memory_order_release);
		}
} ;

class SpinlockGuard
{
	public:
		explicit SpinlockGuard( Spinlock &sp ) :m_FreePurify( sp )
	{
		m_FreePurify.lock() ;
	}

		~SpinlockGuard( void )
		{
			m_FreePurify.unlock() ;
		}

	private:
		Spinlock &m_FreePurify ;
} ;



template< class T >
class DoubleBufferLockQueue
{
	public:
		DoubleBufferLockQueue( void ) 
			:m_writeIndex( 0 )
		{
			int index =m_writeIndex.load( boost::memory_order_acquire ) ;
			SetReadQueue( 1 - index ) ;
		}

		virtual ~DoubleBufferLockQueue( void ) { }

	public:
		inline void Enqueue( const T &t )
		{
			int index =m_writeIndex.load( boost::memory_order_acquire ) ;
			m_queue[ index ].push_back( t );
		}

		//////////////////////////////////////////////////
		// Get until empty.
		inline bool Dequeue( T &t )
		{
			//ASSERT( m_check[ boost::this_thread::get_id() ] >=0 && m_check.size() <= 2 ) ; 

			if( m_pReadQueue->empty() )
			{
				SwapBuffer() ;
				if( m_pReadQueue->empty() )
				{
					return false ;
				}
			}

			t = m_pReadQueue->front();
			m_pReadQueue->pop_front();
			return true ;
		}

		//////////////////////////////////////////////////
		// Get Some
		inline void SwapBuffer( void )
		{
			int orignIndex =m_writeIndex.load( boost::memory_order_acquire ) ;

			ASSERT( m_pReadQueue->empty() ) ;
			int nowInddex = 1 - orignIndex ;

			m_writeIndex.store( nowInddex, boost::memory_order_release ) ;

			SetReadQueue( orignIndex ) ;

			//cout << "swap id:" << boost::this_thread::get_id() << ", " << "index: " << m_writeIndex << endl ;
		}

		inline bool DequeueRead( T &t )
		{
			//ASSERT( m_check[ boost::this_thread::get_id() ] >=0 && m_check.size() <= 2 ) ; 

			if( m_pReadQueue->empty() )
			{
				return false ;
			}

			t = m_pReadQueue->front();
			m_pReadQueue->pop_front();
			return true ;
		}

		inline size_t Size( void )
		{
			//ASSERT( m_check[ boost::this_thread::get_id() ] >=0 && m_check.size() <= 2 ) ; 
			return m_pReadQueue->size() ;
		}


		inline bool GetIsLockFree( void )
		{
			return m_writeIndex.is_lock_free() ;
		}

		inline std::deque<T>* GetReadQueue( void )
		{
			return m_pReadQueue ;
		}

	private:
		inline void SetReadQueue( int index )
		{
			m_pReadQueue = m_queue + index ;
		}

	protected:
		std::map< boost::thread::id, int > m_check ;

		boost::atomic<int>          m_writeIndex ; 
		std::deque<T>               m_queue[2] ;
		std::deque<T>              *m_pReadQueue ;
} ;


template< class T >
class PtrDoubleBufferLockQueue : public DoubleBufferLockQueue< T >
{
	public:
		typedef DoubleBufferLockQueue<T> Base ;

	public:
		PtrDoubleBufferLockQueue( void ) {}

		virtual ~PtrDoubleBufferLockQueue( void )
		{
			std::for_each( Base::m_queue[0].begin(), Base::m_queue[0].end(), DeleteTheValue() ) ;
			Base::m_queue[0].clear() ;

			std::for_each( Base::m_queue[1].begin(), Base::m_queue[1].end(), DeleteTheValue() ) ;
			Base::m_queue[1].clear() ;
		}
} ;


template< class T, class LockQueue =DoubleBufferLockQueue<T> >
class MultiBufferLockQueue
{
	public:
		typedef std::map< boost::thread::id, LockQueue* >   WriteQueueTable ;
		typedef typename WriteQueueTable::iterator          WriteQueueTableItr ;

	public:
		MultiBufferLockQueue( void ) 
		{
			m_pReadQueue  =NULL ;
			m_writeItr    =m_writeQueues.end() ;
		}

		~MultiBufferLockQueue( void )
		{
			std::for_each( m_writeQueues.begin(), m_writeQueues.end(), DeleteSecondValue() ) ;
		}

	public:
		inline LockQueue* InitById( const boost::thread::id &id )
		{
			LockQueue *pLq =new LockQueue() ;
			m_newQueue.Enqueue( std::make_pair( id, pLq ) ) ;

			//cout << "end init " << endl ;
			return pLq ;
		}

		inline void InitByVec( const std::vector< boost::thread::id > &threadIds )
		{
			std::for_each( threadIds.begin(), threadIds.end(), boost::bind( &MultiBufferLockQueue::InitById, this, _1 ) ) ;
		}

		inline void Enqueue( const T &t )
		{
			m_writeQueueLock.lock() ;
			WriteQueueTableItr itr =m_writeQueues.find( boost::this_thread::get_id() ) ;
			m_writeQueueLock.unlock() ;

			if( itr != m_writeQueues.end() )
			{
				itr->second->Enqueue( t ) ;
			}
			else
			{
				LockQueue *pLq =InitById( boost::this_thread::get_id() ) ;
				ASSERT( pLq != NULL ) ;
				if( pLq != NULL )
				{
					pLq->Enqueue( t ) ;
				}
			}

		}

		//////////////////////////////////////////////////
		// Get until empty.
		inline bool Dequeue( T &t )
		{
			CheckInit() ;
			if( m_pReadQueue == NULL )
			{
				return false ;
			}

			if( m_pReadQueue->empty() )
			{
				if( !SetReadQueue() )
				{
					return false ;
				}
			}

			t = m_pReadQueue->front();
			m_pReadQueue->pop_front();
			return true ;
		}

		//////////////////////////////////////////////////
		// Get Some
		inline void SwapBuffer( void )
		{
			if( m_pReadQueue != NULL )
			{
				ASSERT( m_pReadQueue->empty() ) ;
				SetReadQueue() ;
			}
		}

		inline bool DequeueRead( T &t )
		{
			CheckInit() ;
			if( m_pReadQueue == NULL || m_pReadQueue->empty() )
			{
				return false ;
			}

			t = m_pReadQueue->front();
			m_pReadQueue->pop_front();
			return true ;
		}

		size_t Size( void )
		{
			return m_pReadQueue->size() ;
		}

	private:
		void CheckInit( void )
		{
			if( m_newQueue.IsEmpty() )
			{
				return ;
			}

			SpinlockGuard lock( m_writeQueueLock ) ;

			std::pair< boost::thread::id, LockQueue*> value ;
			while( m_newQueue.Dequeue( value ) )
			{
				std::pair< WriteQueueTableItr, bool > pValue =m_writeQueues.insert( value ) ;
				if( !pValue.second )
				{
					T t ;

					while( value.second->Dequeue( t ) )
					{
						pValue.first->second->Enqueue( t ) ;
					}

					delete value.second ;
				}
			}

			if( m_pReadQueue == NULL )
			{
				SetReadQueue() ;
			}
		}

		bool SetReadQueue( void )
		{
			size_t size =m_writeQueues.size() ;
			const WriteQueueTableItr endItr =m_writeQueues.end() ;
			for( size_t i =0; i < size; ++i )
			{
				if( m_writeItr == endItr )
				{
					m_writeItr =m_writeQueues.begin() ;
				}

				LockQueue *pLq =m_writeItr->second ;
				//ASSERT( pLq->GetReadQueue()->empty() ) ;
				//m_pReadQueue =pLq->GetReadQueue() ;
				//if( !m_pReadQueue->empty() )
				//{
				//	++m_writeItr ;
				//	return true ;
				//}

				pLq->SwapBuffer() ;
				m_pReadQueue =pLq->GetReadQueue() ;

				++m_writeItr ;

				if( !m_pReadQueue->empty() )
				{
					return true ;
				}
			}

			return false ;
		}

		// read
	protected:
		std::deque<T>             *m_pReadQueue ;

		// write
	protected:
		WriteQueueTableItr         m_writeItr    ;
		WriteQueueTable            m_writeQueues ;

	private:
		Spinlock                   m_writeQueueLock ;
		LockFreeQueue< std::pair< boost::thread::id, LockQueue* > > m_newQueue ;
};

template< class T, class LockQueue =PtrDoubleBufferLockQueue<T> >
class PtrMultiBufferLockQueue : public MultiBufferLockQueue< T >
{
	public:
		typedef MultiBufferLockQueue<T> Base ;

	public:
		PtrMultiBufferLockQueue( void ) { }

		virtual ~PtrMultiBufferLockQueue( void )
		{
			if( Base::m_pReadQueue != NULL )
			{
				std::for_each( Base::m_pReadQueue->begin(), Base::m_pReadQueue->end(), DeleteTheValue() ) ;
				Base::m_pReadQueue->clear() ;
			}
		}
} ;



