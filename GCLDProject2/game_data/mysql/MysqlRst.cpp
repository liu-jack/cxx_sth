#include "MysqlRst.h"

#ifdef MYSQL_SUPPORT

#include <string.h>
static const char* INDEX_ERROR = "Index Error!";


CMysqlRst::CMysqlRst(MYSQL_RES* res, uint32 FieldCount, uint32 RowCount) : m_nFieldCount(FieldCount), m_nRowCount(RowCount), m_pResult( res )
{
	if( res == NULL || FieldCount == 0 )
	{
		m_pCurrentRow	= NULL;
		m_pFields		= NULL;
		m_nRowCount		= 0;
		//m_fieldName.clear();
	}else{
		m_pCurrentRow	= new Field[FieldCount];
		m_pFields		= mysql_fetch_fields(res);
		/*for ( uint32 iField = 0; iField < m_nFieldCount; iField ++ )
		{
			m_fieldName[ m_pFields[iField].name] = iField;
		}*/
	}
}

CMysqlRst::~CMysqlRst()
{
	//m_fieldName.clear();
	if(m_pResult) mysql_free_result(m_pResult);
	delete [] m_pCurrentRow;
}

bool CMysqlRst::NextRow()
{
	if( m_nRowCount <= 0 )
		return false;

	MYSQL_ROW row = mysql_fetch_row(m_pResult);
	if(row == NULL)
		return false;

	for(uint32 i = 0; i < m_nFieldCount; ++i)
		m_pCurrentRow[i].SetValue(row[i]);

	return true;
}


std::string  CMysqlRst::GetFieldName(uint32 iCol)
{
	if ( iCol < 0 || iCol >= m_nFieldCount)
	{
		return INDEX_ERROR;
	}
	return m_pFields[iCol].name;
}
/*
int32 CMysqlRst::GetFieldIndex(const std::string strFieldName)
{
	std::map< std::string, uint32>::iterator iter = m_fieldName.find(strFieldName);
	if ( iter != m_fieldName.end() )
		return iter->second;
	return -1;
}

const Field& CMysqlRst::operator[](const std::string strFieldName) const
{
	assert(m_pCurrentRow);	
	std::map< std::string, uint32>::const_iterator iter = m_fieldName.find( strFieldName);
	assert( iter != m_fieldName.end() );
	return m_pCurrentRow[iter->second];
}*/
const Field& CMysqlRst::operator[](uint32 iCol) const
{
	assert(m_pCurrentRow&& iCol<= m_nFieldCount);
	return m_pCurrentRow[iCol];
}

#endif