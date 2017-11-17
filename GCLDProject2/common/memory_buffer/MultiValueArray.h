#pragma once
#include "TValueArray.h"

namespace pb
{
	class Int32List ;
	class Int32ListUpdate ;

	class Uint64List;
	class Uint64ListUpdate;

	class FloatList ;
	class FloatListUpdate ;

	class StringList;
	class StringListUpdate;

	class ObjectFullValue ;
	class ObjectValueUpdate ;


}


class MultiValueArray
{
public:
	MultiValueArray(uint32 int32Size, uint32 uint64Size, uint32 floatSize, uint32 stringSize);
	MultiValueArray(const MultiValueArray& other);
	virtual ~MultiValueArray(void);

	bool operator==(const MultiValueArray& other) const;
	bool operator!=(const MultiValueArray& other) const;

	int Int32ValuesSize() const;
	int Uint64ValuesSize() const;
	int FloatValuesSize() const;
	int StringValuesSize() const;

	bool IsModified() const;
	bool IsInt32ValuesModified() const;
	bool IsUint64ValuesModified() const;
	bool IsFloatValuesModified() const;
	bool IsStringValuesModified() const;

	void SetUnmodified();

	bool SetInt32(uint32 idx, int32 val);
	int32 GetInt32(uint32 idx) const;

	bool SetUint64(uint32 idx, uint64 val);
	uint64 GetUint64(uint32 idx) const;

	bool SetFloat(uint32 idx, float val);
	float GetFloat(uint32 idx) const;

	bool SetString(uint32 idx, const string &val);
	const std::string& GetString(uint32 idx) const;

	void BuildUpdateMsg(pb::ObjectValueUpdate& msg) const;
	void BuildFullUpdateMsg(pb::ObjectFullValue& msg) const;

    void ApplyUpdateMsg( const pb::ObjectValueUpdate& msg);
    void ApplyFullUpdateMsg( const pb::ObjectFullValue& msg);

private:
	TValueArray<int32,  pb::Int32List,  pb::Int32ListUpdate>  m_in32Value;
	TValueArray<uint64, pb::Uint64List, pb::Uint64ListUpdate> m_uin64Value;
	TValueArray<float,  pb::FloatList,  pb::FloatListUpdate>  m_floatValue;
	TValueArray<string, pb::StringList, pb::StringListUpdate> m_stringValue;


	// Tool
public:
	static int32 ModifyInt( int32 orignValue, int32 modifyValue )
	{
		orignValue += modifyValue ;
		if( orignValue < 0 )
		{
			orignValue = modifyValue < 0 ? 0 : 0x7FFFFFF ;
		}

		return orignValue ;
	}

};
