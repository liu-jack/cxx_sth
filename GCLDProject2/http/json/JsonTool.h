#ifndef JSON_TOOL_H__
#define JSON_TOOL_H__

#include <string>

#include <boost/smart_ptr.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "def/TypeDef.h"

class JsonTool
{
public:
	typedef boost::property_tree::ptree Tree ;
	typedef Tree*                       TreePtr ;
	typedef Tree::iterator              TreeItr ;

public:
	JsonTool( void ) ;
	JsonTool( TreePtr pTree ) ;
	~JsonTool( void ) ;

	static bool SimpleCheckIsJsonData( const std::string &str ) ;

	bool     GetIsOk( void ) {   return m_pTree != NULL ;   }
	TreePtr  GetTreePtr(void) { return  m_pTree;}


	JsonTool GetChild( const std::string &str ) ;
	
	void PutChild(const std::string& key,JsonTool& jsont);
	void PushBack(JsonTool& jsont);
	bool GetIsHadElement( void ) ;
	JsonTool GetElement( void ) ;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Read, Step 1:
	bool LoadFromStr( const std::string &str ) ;
	void LoadFromFile( const std::string &fileName ) ;

	// Read, Step 2:
	int GetInt( const std::string &key ) const ;
	int64 GetInt64( const std::string &key ) const ;
	bool GetBool( const std::string &key ) const ;
	float GetFloat(  const std::string &key ) const ;
	void GetString( const std::string &key, std::string &value ) const ;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Write Step 1
	void WriteInt( const std::string& key, int value ) ;
	void WriteUint64( const std::string& key, uint64 value );
	void WriteBool( const std::string &key, bool value ) ;
	void WriteFloat( const std::string& key, float value ) ;
	void WriteString( const std::string& key, const std::string &value ) ;

	// Write Step 1
	void WriteToSting( std::string &str ) ;
	void WriteToFile( const std::string &fileName ) ;

private:
	template< class KT, class RT >
	void Get( const KT &key, RT &retVal ) const ;

private:
	bool    m_needDelete ;
	TreePtr m_pTree;
	TreeItr m_itr ;
} ;



#endif

