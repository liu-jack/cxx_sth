
#include <sstream>

#include "JsonTool.h"
#include "Logger.h"

#include <iostream>

#include <boost/exception/all.hpp>

typedef boost::error_info<struct tag_err_no, int> err_no;

typedef boost::error_info<struct tag_err_str, std::string> err_str;

JsonTool::JsonTool( void )
{
	m_needDelete =true ;
	m_pTree      = new Tree() ;
}

JsonTool::JsonTool( TreePtr pTree ) : m_pTree( pTree )
{
	m_needDelete =false ;
}

JsonTool::~JsonTool( void )
{
	if( m_needDelete && m_pTree != NULL )
	{
		delete m_pTree ;
		m_pTree      =NULL ;
		m_needDelete =false ;
	}
}


bool JsonTool::SimpleCheckIsJsonData( const std::string &str )
{
	std::string::size_type fPos =str.find_first_not_of( "\n \"" ) ;
	if( std::string::npos == fPos || str[fPos] != '{' )
	{
		return false ;
	}

	std::string::size_type mPos =str.find( ':', fPos + 1 ) ;
	if( std::string::npos == mPos )
	{
		return false ;
	}

	std::string::size_type lPos =str.rfind( '}' ) ;
	if( std::string::npos == lPos || lPos < mPos )
	{
		return false ;
	}

	return true ;
}

JsonTool JsonTool::GetChild( const std::string &str )
{
	Tree tmpTree ;

	/** Get the child at the given path, or return @p default_value. */
	Tree &retTree =m_pTree->get_child( str, tmpTree );
	if( &retTree != &tmpTree )
	{
		return JsonTool( &retTree ) ;
	}

	return JsonTool( NULL ) ;
}

void JsonTool::PutChild(const std::string& key,JsonTool& jsont)
{
	if(jsont.GetIsOk())
	{
		m_pTree->put_child(key,*jsont.GetTreePtr());
	}
}

void JsonTool::PushBack(JsonTool& jsont)
{
	if(jsont.GetIsOk())
	{
		m_pTree->push_back(std::make_pair("", *jsont.GetTreePtr()));
	}
}

bool JsonTool::GetIsHadElement( void )
{
	m_itr =m_pTree->begin() ;
	return m_itr != m_pTree->end() ;
}

JsonTool JsonTool::GetElement( void )
{
	if( m_itr !=  m_pTree->end() )
	{
		return JsonTool( &(m_itr++)->second ) ;
	}

	return JsonTool( NULL ) ;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Read, Step 1:
bool JsonTool::LoadFromStr( const std::string &str )
{
	try
	{
		std::istringstream is( str.c_str() ) ;
		boost::property_tree::json_parser::read_json( is, *m_pTree ) ;
		return true ;
	}
	catch( ... /*const std::exception& e*/ )
	{
		// 防止太长，所以只输出前一段
		const size_t limitLen =30 ;
		std::string errMsg( str, 0, str.size() > limitLen ? limitLen : str.size() ) ;
		ELOG( "JsonTool::LoadFromStr( %s ) error", errMsg.c_str() ) ;

		// 下面开启详细信息输出
		//std::cout << boost::diagnostic_information(e) << std::endl;

		return false ;
	}
}

void JsonTool::LoadFromFile( const std::string &fileName )
{
	try
	{
		boost::property_tree::json_parser::read_json( fileName, *m_pTree ) ;
	}
	catch( ... /*const std::exception& e*/ )
	{
		ELOG( "JsonTool::LoadFromFile( %s ) error", fileName.c_str() ) ;

		// 下面开启详细信息输出
		//std::cout << boost::diagnostic_information(e) << std::endl;
	}
}

// Read, Step 2:
int JsonTool::GetInt( const std::string &key ) const
{
	int retValue =0 ;
	Get( key, retValue ) ;
	return retValue ;
}

int64 JsonTool::GetInt64( const std::string &key ) const
{
	int64 retValue =0 ;
	Get( key, retValue ) ;
	return retValue ;
}


bool JsonTool::GetBool( const std::string &key ) const
{
	bool retValue =false ;
	Get( key, retValue ) ;
	return retValue ;

}

float JsonTool::GetFloat(  const std::string &key ) const
{
	float retValue =0.0f ;
	Get( key, retValue ) ;
	return retValue ;
}

void JsonTool::GetString( const std::string &key, std::string &value ) const
{
	Get( key, value ) ;
}

template< class KT, class RT >
void JsonTool::Get( const KT &key, RT &retVal ) const
{
	try
	{
		retVal =m_pTree->get<RT>( key ) ;
	}
	catch( /*const std::exception& e*/ ... )
	{
		ELOG( "JsonTool read error not the type" ) ;

		// 下面开启详细信息输出
		//std::cout << boost::diagnostic_information(e) << std::endl;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Write Step 1
void JsonTool::WriteUint64( const std::string& key, uint64 value )
{
	m_pTree->put<uint64>( key, value ) ;
}

void JsonTool::WriteInt( const std::string& key, int value )
{
	m_pTree->put<int>( key, value ) ;
}

void JsonTool::WriteBool( const std::string &key, bool value )
{
	m_pTree->put<bool>( key, value ) ;
}


void JsonTool::WriteFloat( const std::string& key, float value )
{
	m_pTree->put<float>( key, value ) ;
}

void JsonTool::WriteString( const std::string& key, const std::string &value )
{
	m_pTree->put<std::string>( key, value ) ;
}

// Write Step 1
void JsonTool::WriteToSting( std::string &str )
{
	std::ostringstream os ;
	boost::property_tree::json_parser::write_json( os, *m_pTree ) ;
	str =os.str() ;
}

void JsonTool::WriteToFile( const std::string &fileName )
{
	boost::property_tree::json_parser::write_json( fileName, *m_pTree ) ;
}



