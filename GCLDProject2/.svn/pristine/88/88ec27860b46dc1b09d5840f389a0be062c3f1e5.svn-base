#pragma once

#include <map>

#ifdef WIN32
#include <Winsock2.h>
#include "mysql/mysql.h"
#include "mysql/errmsg.h"
#else
#include "mysql/mysql.h"
#include "mysql/errmsg.h"
#endif

#include "utility/TimeParser.h"

#include "def/TypeDef.h"

class Field
{
public:
    Field() : mValue( NULL){}
    ~Field(){}

	void SetValue(char* value) { mValue = value; }

	const char *GetString()const { 
		if(mValue==NULL)
			return "";
		return mValue; }
	float GetFloat()const	{ return mValue ? static_cast<float>(atof(mValue))	 : 0; }
	bool GetBool()const		{ return mValue ? atoi(mValue) > 0 : false; }
	uint8 GetUInt8()const	{ return mValue ? static_cast<uint8>(atoll(mValue)) : 0; }
	int8 GetInt8()const		{ return mValue ? static_cast<int8>(atoll(mValue)) : 0; }
	uint16 GetUInt16()const { return mValue ? static_cast<uint16>(atoll(mValue)) : 0; }
	int16 GetInt16()const	{ return mValue ? static_cast<int16>(atoll(mValue)) : 0; }
	uint32 GetUInt32()const { return mValue ? static_cast<uint32>(atoll(mValue)) : 0; }
	int32 GetInt32() const	{ return mValue ? static_cast<int32>(atoll(mValue)) : 0; }
	int64 GetInt64() const	{ return mValue ? static_cast<int64>(atoll(mValue)) : 0; }
	uint64 GetUInt64()const 
	{
		if(mValue)
		{
			uint64 value;
#ifndef WIN32	// Make GCC happy.
			sscanf(mValue,I64FMTD,(long long unsigned int*)&value);
#else
			sscanf_s(mValue, I64FMTD, &value);
#endif
			return value;
		}
		else
			return 0;
	}
	uint64 GetTimeT()const
	{
		time_t time1 = 0;
		String2TimeT(mValue,time1); 
		return time1;
	}

private:
	char *mValue;
};


class CMySqlRst
{
	friend class CMysqlRstPtr;
public:
	CMySqlRst(MYSQL_RES* res, uint32 FieldCount, uint32 RowCount);

	bool NextRow();

	Field* Fetch() { return m_pCurrentRow; }
	uint32 GetFieldCount() const { return m_nFieldCount; }
	uint32 GetRowCount() const { return m_nRowCount; }


	std::string GetFieldName(uint32 iCol);
	//int32		GetFieldIndex(const std::string pszName);

	//const Field& operator[](const std::string strFieldName) const;
	const Field& operator[](uint32 iCol) const;

protected:
	~CMySqlRst();
	void Delete() { delete this; }

protected:
	uint32 m_nFieldCount;
	uint32 m_nRowCount;
	Field * m_pCurrentRow;
	MYSQL_RES* m_pResult;

	//std::map< std::string, uint32> m_fieldName;
	MYSQL_FIELD* m_pFields;
};


class CMysqlRstPtr
{
public:
	CMysqlRstPtr(const CMysqlRstPtr& rst) : m_rst(NULL)
	{
		*this = rst;
	};
	CMysqlRstPtr(CMySqlRst* rst = NULL) : m_rst(rst) 
	{
	};
	~CMysqlRstPtr()
	{
		Delete();
	};
	//CMysqlRstPtr & operator = (CMySqlRst* rst)
	//{
	//	Delete();
	//	m_rst = rst;
	//	return *this;
	//};
	CMysqlRstPtr & operator = (const CMysqlRstPtr& arst)
	{
		Delete();
		m_rst = arst.m_rst;
		arst.m_rst = NULL;
		return *this;
	};
	CMySqlRst * operator -> ()
	{
		assert( m_rst != NULL );
		return m_rst;
	};
	const CMySqlRst * operator -> () const
	{
		assert( m_rst != NULL );
		return m_rst;
	};
	CMySqlRst & operator*() const
	{
		assert( m_rst != NULL );
		return *m_rst;
	}
	CMySqlRst * Get()
	{
		return m_rst;
	};
	operator bool () const
	{
		return m_rst != NULL;
	};
	bool operator! () const // never throws
	{
		return m_rst == NULL;
	};
public:
	void Delete()
	{
		if (m_rst)
		{
			m_rst->Delete();
			m_rst = NULL;
		}
	};
private:
	mutable CMySqlRst * m_rst;
};

