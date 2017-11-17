#pragma once

#include "def/TypeDef.h"
#include "memory_buffer/NetPack.h"

class ArrayData{
    static const uint8 DATA_SIZE = 16;
    int m_data[DATA_SIZE];
    short m_bits; // 标记有效值
public:
    ArrayData();
	int operator[](uint8 n) const{ return (n < DATA_SIZE) ? m_data[n] : 0;}
    bool Change(uint8 n, int v);
    bool Clear();
    friend class Pack_ArrayData;
};

class Pack_ArrayData{
	NetPack m_pack;
public:
    enum DataTyp {
        MapData,
        CampData,
    };
	Pack_ArrayData(uint16 opCode) : m_pack(opCode) {}
	inline operator NetPack&(){ return m_pack; }
    void FillMsg(DataTyp typ, const ArrayData& refArr);
    void FillMsg(DataTyp typ, const ArrayData& refArr, uint8 n);
    void FillArray(ArrayData& refArr); //Notice：在"rPack >> typ"之后调用此接口
};