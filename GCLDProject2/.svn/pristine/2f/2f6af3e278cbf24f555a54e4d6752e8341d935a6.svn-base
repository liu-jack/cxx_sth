#include "MapData.h"

ArrayData::ArrayData()
{
	//STATIC_ASSERT(((int)sizeof(m_bits) * 8 >= (int)DATA_SIZE), ArrayDataError);
	m_bits = 0;
	memset(m_data, 0, sizeof(m_data));
}
bool ArrayData::Clear()
{
    if (m_bits == 0) return false;
    m_bits = 0;
    memset(m_data, 0, sizeof(m_data));
    return true;
}
bool ArrayData::Change(uint8 n, int v)
{
    if (n < DATA_SIZE)
    {
        if (m_data[n] == v) return false;

		m_data[n] = v;

        if (v == 0)
			m_bits &= ~(1 << n);  // 将n位置0
        else
			m_bits |= (1 << n);   // 将n为置1
		return true;
    }
    return false;
}


void Pack_ArrayData::FillMsg(DataTyp typ, const ArrayData& refArr)
{
    m_pack << typ << true << refArr.m_bits;

    for (int i = 0; i < (int)ArrayData::DATA_SIZE; ++i){
        if (refArr.m_data[i]){
            m_pack << refArr.m_data[i];
        }
    }
}
void Pack_ArrayData::FillMsg(DataTyp typ, const ArrayData& refArr, uint8 n)
{
    if (n < ArrayData::DATA_SIZE)
    {
		m_pack << typ << false << short(1 << n) << refArr.m_data[n];
    }
}
void Pack_ArrayData::FillArray(ArrayData& refArr) //Notice：在"rrPack >> typ"之后调用此接口
{
	bool isReset; short bits;
	m_pack >> isReset >> bits;

    if (isReset){
		refArr.m_bits = bits;
        memset(refArr.m_data, 0, sizeof(refArr.m_data));
    }else{
        refArr.m_bits |= bits;
    }
    for (int i = 0; i < (int)ArrayData::DATA_SIZE; ++i){
        if (bits & (1 << i)){
            m_pack >> refArr.m_data[i];
        }
    }
}