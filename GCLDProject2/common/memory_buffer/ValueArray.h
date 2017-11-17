#pragma once

#include "def/TypeDef.h"
#include "boost/throw_exception.hpp"
#include "boost/dynamic_bitset.hpp"

namespace pb
{
	class Int32List ;
	class Int32ListUpdate ;
}

class ValueArray
{
public:
	ValueArray(uint32 size);
	ValueArray(const ValueArray& other);
	~ValueArray(void);

	bool operator==(const ValueArray& other) const;
	bool operator!=(const ValueArray& other) const;

	uint32 Size() const;

	bool IsModified() const;
	void SetUnmodified();

	bool IsFloat( uint32 idx) const;

	void SetInt(uint32 idx, int32 val);
	int32 GetInt(uint32 idx) const;
	void ModifyInt( uint32 idx, int32 val ) ;

	void SetFloat(uint32 idx, F32 val);
	F32 GetFloat(uint32 idx) const;
	F32 GetIntOrFloat(uint32 idx) const; // for lua script
	void CopyFrom(const ValueArray& other, uint32 idx);

	void SetFlag(uint32 idx, int32 flag );
	void SetFlag(uint32 idx, int32 flag, bool isSet );
	bool HasAllFlag(uint32 idx, int32 flag ) const;
	bool HasAnyFlag(uint32 idx, int32 flag ) const;
	void RemoveFlag(uint32 idx, int32 flag );

	void BuildUpdateMsg( pb::Int32ListUpdate& msg) const;
	void BuildFullUpdateMsg( pb::Int32List& msg) const; 

	void BuildFullUpdateMsg( pb::Int32ListUpdate& msg) const;

    void ApplyUpdateMsg(const pb::Int32ListUpdate& msg) ;
    void ApplyFullUpdateMsg(const pb::Int32List& msg) ;
public:




protected:
	F32 InnerGetFloat(uint32 idx) const;

protected:
	std::vector<int32>				m_values;
	boost::dynamic_bitset<uint32>	m_isFloat;
	boost::dynamic_bitset<uint32>	m_modifyBits;
};
