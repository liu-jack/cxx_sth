
#ifndef _BYTEBUFFER_H
#define _BYTEBUFFER_H

#include <stdlib.h>
#include <cstdlib>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <stdexcept>

#ifndef _TOOL
#include <google/protobuf/message.h>
#endif

#include "Logger.h"
#include "MemoryPool.h"
#include "def/TypeDef.h"
#include "def/MmoAssert.h"

// Notice:
// The packet header had write into buffer now, but in calculate max size not reduce it,
// because i am think that is umimportance .
#define MAX_INPUT_PACKET_SIZE   64*1024
#define MAX_OUTPUT_PACKET_SIZE  64*1024

class ByteBuffer
{
	private:
		DECLARE_OBJ_POOL( ByteBuffer )

	////////////////////////////////////////////////////////////////////////////////////////////////
	// member data
	protected:
		// read and write positions
		size_t _rpos, _wpos;
        std::vector<uint8> _storage;



	////////////////////////////////////////////////////////////////////////////////////////////////
	// error
	public:
		bool has_error() const {return _error;}
		void clear_error() {_error = false;}

	private:
		void set_error() const { _error = true; }
		mutable bool	_error;



		////////////////////////////////////////////////////////////////////////////////////////////////
		// protobuf
	public:
#ifndef _TOOL
		ByteBuffer& operator<<(const ::google::protobuf::Message& msg)
		{
			size_t msgSize = msg.ByteSize() ;
			if ( msgSize + sizeof( uint16 ) + size() > MAX_OUTPUT_PACKET_SIZE )
			{
				//throw std::runtime_error("Message size more than 64k bytes");
				ELOG( "ByteBuffer& operator<<() -> Message size more than 64k bytes" ) ;
				set_error() ;
				ASSERT( false ) ;
				return *this ;
			}

			append<uint16>((uint16)msgSize);

			if (msgSize >0)
			{
				if (_storage.size() < _wpos + msgSize)
				{
					_storage.resize(_wpos + msgSize);
				}

				if (!msg.SerializeToArray(&_storage[_wpos], msgSize))
				{
					//throw new std::exception();
					ELOG( "ByteBuffer& operator<<() -> msg.SerializeToArray() faild!!" ) ;
					set_error() ;
					ASSERT( false ) ;
					return *this ;
				}

				_wpos += msgSize;
			}

			return *this;
		}

		ByteBuffer& operator>>(::google::protobuf::Message& msg)
		{
			uint16 msgSize =read<uint16>();

			if (msgSize >= MAX_INPUT_PACKET_SIZE)
			{
				ELOG("ByteBuffer operator>> Message size INPUT more than 64k bytes");
				//ASSERT( false ) ;
			}

			if (_rpos + msgSize > _storage.size())
			{
				ELOG("ByteBuffer operator>>(::google::protobuf::Message& msg) -> _rpos + msgSize > _storage.size() ") ;
				set_error();
				//ASSERT(false);
				return *this;
			}

			if (msgSize >0)
			{
				bool parseOk = msg.ParseFromArray(&_storage[_rpos], msgSize);
				if (!parseOk)
				{
					ELOG( "ByteBuffer operator>>(::google::protobuf::Message& msg) -> msg.ParseFromArray(&_storage[_rpos], msgSize) faild??" ) ;
					set_error();
					//ASSERT(false);
					return *this;
				}

				//ASSERT (msgSize == msg.ByteSize());
				if(msgSize != msg.ByteSize())
				{
					ELOG( "ByteBuffer operator>>  msg  Size Not Match !!!" ) ;
				}
				_rpos += msgSize;
			}

			return *this;
		}
#endif

	////////////////////////////////////////////////////////////////////////////////////////////////
	// construct
	public:
		const static size_t DEFAULT_SIZE = 0x1000;

		ByteBuffer()
			: _rpos(0)
			  , _wpos(0) 
			  ,_error(false) {
				  _storage.reserve(DEFAULT_SIZE);
			  }

		ByteBuffer(size_t res)
			: _rpos(0)
			  , _wpos(0) 
			  ,_error(false) {
				  _storage.reserve(res);
			  }

		ByteBuffer(const ByteBuffer &buf)
			: _rpos(buf._rpos)
			  , _wpos(buf._wpos)
			  , _storage(buf._storage) 
			  ,_error(buf._error) { 
			  }

		virtual ~ByteBuffer() {}



	////////////////////////////////////////////////////////////////////////////////////////////////
	// common
	public:
		inline void clear() {
			_storage.clear();
			_rpos = _wpos = 0;
			clear_error();
		}

		inline uint8 operator[](size_t pos) {
			return read<uint8>(pos);
		}

		inline uint8* buffer() {
			return &_storage[0];
		}

		inline uint8* buffer( size_t index ) {
			ASSERT( index < _storage.size() ) ;
			return &_storage[ index ] ;
		}

		inline const uint8* contents() const {
			return &_storage[0];
		}

		inline size_t size() const {
			return _storage.size();
		}

		inline void resize(size_t newsize) {
			_storage.resize(newsize);
			_rpos = 0;
			_wpos = size();
		}

		inline void reserve(size_t ressize) {
			if (ressize > size()) _storage.reserve(ressize);
		}

		inline void reverse() {
			std::reverse(_storage.begin(), _storage.end());
		}

		inline void Swap( ByteBuffer &buf ) {
			_storage.swap( buf._storage ) ;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////
		// read/write pos
	public:
		inline size_t rpos() {
			return _rpos;
		};

		inline size_t rpos(size_t rpos) {
			_rpos = rpos;
			return _rpos;
		};

		inline size_t wpos() {
			return _wpos;
		}

		inline size_t wpos(size_t wpos) {
			_wpos = wpos;
			return _wpos;
		}



		////////////////////////////////////////////////////////////////////////////////////////////////
		// put
	public:
		template <typename T> inline void put(size_t pos,T value) {
			put(pos,(uint8 *)&value,sizeof(value));
		}

		inline void put(size_t pos, const uint8 *src, size_t cnt) {
			memcpy(&_storage[pos], src, cnt);
		}




		////////////////////////////////////////////////////////////////////////////////////////////////
		// append
	public:
		template <typename T> inline void append(T value) {
			append((uint8 *)&value, sizeof(value));
		}

		// appending to the end of buffer
		//inline void append(const std::string& str) {

		//	append((uint8 *)str.c_str(),str.size());
		//}

		//inline void append(const char *src, size_t cnt) {
		//	return append((const uint8 *)src, cnt);
		//}

		inline void append(const uint8 *src, size_t cnt) {

			if (!cnt) {    return;   }

			if( size() + cnt >= MAX_OUTPUT_PACKET_SIZE )
			{
				ELOG( "append(const uint8 *src, size_t cnt)ByteBuffer& operator<<() -> append(const uint8 *src, size_t cnt" ) ;
				set_error() ;
				ASSERT( false ) ;
				return ;
			}

			// this way hopefully people will report the callstack after it "crashes"
			if (_storage.size() < _wpos + cnt)
			{
				_storage.resize(_wpos + cnt);
			}

			memcpy(&_storage[_wpos], src, cnt);
			_wpos += cnt;
		}


		inline void append(const ByteBuffer& buffer) {
			if(buffer.size() > 0) append(buffer.contents(),buffer.size());
		}




		////////////////////////////////////////////////////////////////////////////////////////////////
		// read
	public:

		template <typename T> inline T read() {
			T r=read<T>(_rpos);
			_rpos += sizeof(T);
			return r;
		}

		template <typename T> inline T read(size_t pos) const {
			//GLL_ASSERT(pos + sizeof(T) <= size());
			if(pos + sizeof(T) > size())
			{
				set_error();
				return (T)0;
			} else {
				return *((T*)&_storage[pos]);
			}
		}

		inline void read(uint8 *dest, size_t len) {
			if ( LIKELY( _rpos + len <= size( ) ) ) {
				memcpy(dest, &_storage[_rpos], len);
			} else {
				//throw error();
				memset(dest, 0, len);
			}
			_rpos += len;
		}



		////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// stream like operators for storing data
	public:
		ByteBuffer &operator<<(bool value) {
			append<char>((char)value);
			return *this;
		}


		ByteBuffer &operator<<(char value) {
			append<char>((char)value);
			return *this;
		}


		// unsigned
		ByteBuffer &operator<<(uint8 value) {
			append<uint8>(value);
			return *this;
		}

		ByteBuffer &operator<<(uint16 value) {
#ifdef USING_BIG_ENDIAN
			append<uint16>(swap16(value));
#else
			append<uint16>(value);
#endif
			return *this;
		}

		ByteBuffer &operator<<(uint32 value) {
#ifdef USING_BIG_ENDIAN
			append<uint32>(swap32(value));
#else
			append<uint32>(value);
#endif
			return *this;
		}

		ByteBuffer &operator<<(uint64 value) {
#ifdef USING_BIG_ENDIAN
			append<uint64>(swap64(value));
#else
			append<uint64>(value);
#endif
			return *this;
		}


		// signed as in 2e complement
		ByteBuffer &operator<<(int8 value) {
			append<int8>(value);
			return *this;
		}

		ByteBuffer &operator<<(int16 value) {
#ifdef USING_BIG_ENDIAN
			append<int16>(swap16(value));
#else
			append<int16>(value);
#endif
			return *this;
		}

		ByteBuffer &operator<<(int32 value) {
#ifdef USING_BIG_ENDIAN
			append<int32>(swap32(value));
#else
			append<int32>(value);
#endif
			return *this;
		}

		ByteBuffer &operator<<(int64 value) {
#ifdef USING_BIG_ENDIAN
			append<int64>(swap64(value));
#else
			append<int64>(value);
#endif
			return *this;
		}

		ByteBuffer &operator<<(float value) {
#ifdef USING_BIG_ENDIAN
			append<float>(swapfloat(value));
#else
			append<float>(value);
#endif
			return *this;
		}

		ByteBuffer &operator<<(double value) {
#ifdef USING_BIG_ENDIAN
			append<double>(swapdouble(value));
#else
			append<double>(value);
#endif
			return *this;
		}

		ByteBuffer &operator<<(const std::string &value) {
			unsigned short len =value.length() ;
			*this << len ;
			append((uint8 *)value.c_str(), value.length());
			//append((uint8)0);
			return *this;
		}

		ByteBuffer &operator<<(const char *str) {
			unsigned short len =strlen(str) ;
			*this << len ;
			append((uint8 *)str, len);
			//append((uint8)0);
			return *this;
		}

		// stream like operators for reading data
		ByteBuffer &operator>>(bool &value) {
			value = read<char>() > 0 ? true : false;
			return *this;
		}

		//unsigned
		ByteBuffer &operator>>(uint8 &value) {
			value = read<uint8>();
			return *this;
		}

		ByteBuffer &operator>>(uint16 &value) {
#ifdef USING_BIG_ENDIAN
			value = swap16(read<uint16>());
#else
			value = read<uint16>();
#endif
			return *this;
		}

		ByteBuffer &operator>>(uint32 &value) {
#ifdef USING_BIG_ENDIAN
			value = swap32(read<uint32>());
#else
			value = read<uint32>();
#endif
			return *this;
		}

		ByteBuffer &operator>>(uint64 &value) {
#ifdef USING_BIG_ENDIAN
			value = swap64(read<uint64>());
#else
			value = read<uint64>();
#endif
			return *this;
		}

		//signed as in 2e complement
		ByteBuffer &operator>>(int8 &value) {
			value = read<int8>();
			return *this;
		}

		//signed as in 2e complement
		ByteBuffer &operator>>(char &value) {
			value = read<char>();
			return *this;
		}

		ByteBuffer &operator>>(int16 &value) {
#ifdef USING_BIG_ENDIAN
			value = swap16(read<int16>());
#else
			value = read<int16>();
#endif
			return *this;
		}

		ByteBuffer &operator>>(int32 &value) {
#ifdef USING_BIG_ENDIAN
			value = swap32(read<int32>());
#else
			value = read<int32>();
#endif
			return *this;
		}

		ByteBuffer &operator>>(int64 &value) {
#ifdef USING_BIG_ENDIAN
			value = swap64(read<int64>());
#else
			value = read<int64>();
#endif
			return *this;
		}

		ByteBuffer &operator>>(float &value) {
#ifdef USING_BIG_ENDIAN
			value = swapfloat(read<float>());
#else
			value = read<float>();
#endif
			return *this;
		}

		ByteBuffer &operator>>(double &value) {
#ifdef USING_BIG_ENDIAN
			value = swapdouble(read<double>());
#else
			value = read<double>();
#endif
			return *this;
		}

		ByteBuffer &operator>>(std::string& value) {
			uint16 len = read<uint16>();
			value.assign((const char*)&_storage[_rpos], len);
			_rpos += len;
			return *this;
		}

		void appendPackGUID(uint64 guid)
		{
			size_t mask_position = wpos();
			*this << uint8(0);
			for(uint8 i = 0; i < 8; i++)
			{
				if(guid & 0xFF)
				{
					_storage[mask_position] |= (1<<i);
					*this << ((uint8)(guid & 0xFF));
				}

				guid >>= 8;
			}
		}

		uint64 unpackGUID()
		{
			uint64 guid = 0;
			uint8 mask;
			uint8 temp;
			*this >> mask;
			for( uint8 i = 0; i < 8; ++i )
			{
				if( mask & ( 1 << i ) )
				{
					*this >> temp;
					guid |= uint64( temp << uint64( i << 3 ) );
				}
			}
			return guid;
		}

		void hexlike()
		{
			uint32 j = 1, k = 1;
			printf("STORAGE_SIZE: %u\n", (unsigned int)size() );
			for(uint32 i = 0; i < size(); i++)
			{
				if ((i == (j*8)) && ((i != (k*16))))
				{
					if (read<uint8>(i) <= 0x0F)
					{
						printf("| 0%X ", read<uint8>(i) );
					}
					else
					{
						printf("| %X ", read<uint8>(i) );
					}

					j++;
				}
				else if (i == (k*16))
				{
					rpos(rpos()-16);	// move read pointer 16 places back
					printf(" | ");	  // write split char

					for (int x = 0; x < 16; x++)
					{
						printf("%c", read<uint8>(i-16 + x) );
					}

					if (read<uint8>(i) <= 0x0F)
					{
						printf("\n0%X ", read<uint8>(i) );
					}
					else
					{
						printf("\n%X ", read<uint8>(i) );
					}

					k++;
					j++;
				}
				else
				{
					if (read<uint8>(i) <= 0x0F)
					{
						printf("0%X ", read<uint8>(i) );
					}
					else
					{
						printf("%X ", read<uint8>(i) );
					}
				}
			}
			printf("\n");
		}

};




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T> ByteBuffer &operator<<(ByteBuffer &b, const std::vector<T> &v)
{
	b << (uint32)v.size();
	for (typename std::vector<T>::const_iterator i = v.begin(); i != v.end(); ++i) {
		b << *i;
	}
	return b;
}

template <typename T> ByteBuffer &operator>>(ByteBuffer &b, std::vector<T> &v)
{
	uint32 vsize;
	b >> vsize;
	v.clear();
	while(vsize--) {
		T t;
		b >> t;
		v.push_back(t);
	}
	return b;
}

template <typename T> ByteBuffer &operator<<(ByteBuffer &b, std::list<T> v)
{
	b << (uint32)v.size();
	for (typename std::list<T>::iterator i = v.begin(); i != v.end(); ++i) {
		b << *i;
	}
	return b;
}

template <typename T> ByteBuffer &operator>>(ByteBuffer &b, std::list<T> &v)
{
	uint32 vsize;
	b >> vsize;
	v.clear();
	while(vsize--) {
		T t;
		b >> t;
		v.push_back(t);
	}
	return b;
}

template <typename K, typename V> ByteBuffer &operator<<(ByteBuffer &b, std::map<K, V> &m)
{
	b << (uint32)m.size();
	for (typename std::map<K, V>::iterator i = m.begin(); i != m.end(); ++i) {
		b << i->first << i->second;
	}
	return b;
}

template <typename K, typename V> ByteBuffer &operator>>(ByteBuffer &b, std::map<K, V> &m)
{
	uint32 msize;
	b >> msize;
	m.clear();
	while(msize--) {
		K k;
		V v;
		b >> k >> v;
		m.insert(make_pair(k, v));
	}
	return b;
}

#endif
