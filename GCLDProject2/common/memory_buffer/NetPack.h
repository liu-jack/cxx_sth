#pragma once

#include <map>
#include <string>
#include <vector>

#include "ByteBuffer.h"
#include "MemoryPool.h"
#include "../def/TypeDef.h"


// for protobuf
namespace google
{
	namespace protobuf
	{
		class Message ;
	}
}

enum PacketTypeEnum
{
	PACKET_TYPE_CLIENT         =0,
	PACKET_TYPE_CLIENT_CROSS   =1,		//客户端直接发给cross，其他服务器仅转发
	PACKET_TYPE_CROSS_CLIENT   =2,		//cross发送给客户端，其他服务器仅转发
	PACKET_TYPE_SERVER_HTTP    =3,

	PACKET_TYPE_SERVER_LOGIN   =19,
	PACKET_TYPE_SERVER_GAME    =23,
	PACKET_TYPE_SERVER_LOBBY   =28,
	PACKET_TYPE_SERVER_CROSS   =30,
	PACKET_TYPE_SERVER_GATEWAY =35,
	PACKET_TYPE_SERVER_DBPROXY =38,
	PACKET_TYPE_SERVER_CENTER  =50,
	PACKET_TYPE_SERVER_EVENT   =60,
	PACKET_TYPE_SERVER_WEB     =77,

	PACKET_TYPE_END
} ;


const size_t HEADER_SIZE      = sizeof(uint16) * 2 + sizeof( char ) + sizeof(uint64); // Pack len & Opcode & packetType & sessionID
const size_t SIZE_INDEX       = 0 ;
const size_t OPCODE_INDEX     = 2 ;
const size_t TYPE_INDEX       = 4 ;
const size_t SESSION_ID_INDEX = 5 ;


struct lua_State ;

class NetPack
{
	private:
		DECLARE_OBJ_POOL(NetPack)

	public:
		NetPack(uint16 opCode, int reseved = 256)
			 :m_buf( reseved + GetHeaderSize() ) {
				 InitHeadInfo() ;
				 SetOpCode( opCode ) ;
			 }

		NetPack( char type, uint16 bodySize, uint16 opCode, uint64 sessionId )
			 :m_buf( bodySize + GetHeaderSize() ) {
				 InitHeadInfo( type, bodySize, opCode, sessionId ) ;
			 }
		 
		NetPack(const NetPack& other)
			 :m_buf(other.m_buf){
			 }

		~NetPack(void)
		{
		}

		uint16 GetOpcode( void ) const
		{
			return m_buf.read<uint16>( OPCODE_INDEX ) ;
		}

		void SetOpCode( uint16 opCode )
		{
			m_buf.put( OPCODE_INDEX, opCode ) ;
		}

		const char* GetOpcodeName( void ) ;
		const char* GetPacketTypeName( void ) ;

		bool IsClientPacket( void ) const
		{
			return GetPacketType() == PACKET_TYPE_CLIENT  || IsClientToCrossPacket() || IsCrossToClientPacket();
		}

		bool IsClientToCrossPacket(void) const
		{
			return GetPacketType() == PACKET_TYPE_CLIENT_CROSS;
		}

		bool IsCrossToClientPacket(void) const
		{
			return GetPacketType() == PACKET_TYPE_CROSS_CLIENT;
		}

		void SetPacketType( char packType )
		{
			m_buf.put( TYPE_INDEX, packType ) ;
		}

		char GetPacketType( void ) const
		{
			return m_buf.read<char>( TYPE_INDEX ) ;
		}


		void SetSessionId( uint64 sessionId )
		{
			m_buf.put( SESSION_ID_INDEX, sessionId ) ;
		}

		uint64 GetSessionId( void ) const
		{
			return m_buf.read<uint64>( SESSION_ID_INDEX ) ;
		}

		void SetBodySize( uint16 size )
		{
			m_buf.put( SIZE_INDEX, size ) ;
		}

		uint16 GetBodySize( void ) const
		{
			return m_buf.read<uint16>( SIZE_INDEX ) ;
		}

		void SetDynamicBodySize( void )
		{
			SetBodySize( DynamicBodySize() ) ;
		}

		uint8* HeaderRawBuffer()
		{
			return m_buf.buffer();
		}

		uint8* BodyRawBuffer( void )
		{
			return m_buf.buffer( GetHeaderSize() ) ;
		}

		size_t DynamicTotalSize( void ) const
		{
			return m_buf.size() ;
		}

		static size_t GetHeaderSize( void )
		{
			return HEADER_SIZE ;
		}

		size_t DynamicBodySize( void ) const
		{
			return DynamicTotalSize() - HEADER_SIZE ;
		}

		//void AppendTo(ByteBuffer& buf) const
		//{
		//	buf << (uint16)Size();
		//	buf << m_pOpCode;
		//	buf << m_pPacketType ;
		//	buf << m_pSessionId;
		//	buf.append(m_buf);
		//}

		void Resize(size_t size)
		{
			m_buf.resize( size ) ;
			m_buf.rpos( GetHeaderSize() ) ;
		}

		template<class T>
		NetPack& operator << (const T& data)
		{
			m_buf << data;
			return *this;
		}

		template<class T>
		NetPack& operator >> (T& data)
		{
			m_buf >> data;
			return *this;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////
		// 整个结构体读写
		template<class T>
		NetPack& AppendStruct(const T& data)
		{
			m_buf.append( reinterpret_cast<const uint8*>( &data ), sizeof( data )  ) ;
			return *this;
		}
		template<class T>
		NetPack& ReadStruct(T& data)
		{
			m_buf.read( reinterpret_cast<uint8*>( &data ), sizeof( data ) ) ;
			return *this;
		}
		NetPack& Append(const void* data, size_t len)
		{
			m_buf.append( reinterpret_cast<const uint8*>( data ), len  ) ;
			return *this;
		}

		void Clear( void )
		{
			m_buf.clear() ;
			InitHeadInfo() ;
		}

		void Swap( NetPack &packet )
		{
			m_buf.Swap( packet.m_buf ) ;
		}

	private:
		void InitHeadInfo( void ) ;
		void InitHeadInfo( char type, uint16 bodySize, uint16 opCode, uint64 sessionId ) ;

    protected:
		ByteBuffer  m_buf ;


		////////////////////////////////////////////////////////////////////////////////////////////////
		// static
	public:
		static const char* GetPacketTypeName( int type ) ;
		static const bool IsNoticeRecvAndSend( NetPack &packet ) ;

	public:
		typedef std::map< int, std::string > PackTypeNameMap ;

		static PackTypeNameMap GetInitTypeName( void ) ;

		static void ClientOpCodeNoticeInit( void ) ;

		static void ServerOpCodeNoticeInit( void ) ;

	private:
		static std::string          s_unkouwName ;
		static PackTypeNameMap      s_typeName ;
		static std::vector<char>    s_clientOpCodeIsNotice ;
		static std::vector<char>    s_serverOpCodeIsNotice ;


    // for lua
    public:
        bool GetBool( void ) {   return m_buf.read<bool>() ;   }
        void SetBool( bool value ) {   m_buf.append( value );   }

        int32 GetInt32( void ) {   return m_buf.read<int32>() ;   }
        void SetInt32( int32 value ) {   m_buf.append( value ) ;   }

        uint32 GetUInt32( void ) {   return m_buf.read<uint32>() ;   }
        void SetUInt32( uint32 value ) {   m_buf.append( value ) ;   }

        int64 GetInt64( void ) {   return m_buf.read<int64>() ;   }
        void SetInt64( int64 value ) {   m_buf.append( value ) ;   }

        uint64 GetUInt64( void ) {   return m_buf.read<uint64>() ;   }
        void SetUInt64( uint64 value ) {   m_buf.append( value ) ;   }

        // lua 那边单线程使用的， 所以用静态。
        const char* GetString( void ) {   static std::string str ;   m_buf >> str ;    return str.c_str() ;   }
        void SetString( const char *pStr ) {   m_buf << pStr ;   }

        void GetMsg( google::protobuf::Message *pMsg ) ;
        void SetMsg( google::protobuf::Message *pMsg ) ;

};
