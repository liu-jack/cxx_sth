/***********************************************************************
* @ ʵʱ���а�
* @ brief
    1���Ӵ�С���򣬡�1��ʼ��
    2�����黺��Top N���䶯ʱ�����ƶ�
    3����������뺬��int rank��int GetRankVal()

* @ author zhoumf
* @ date 2016-12-26
************************************************************************/
#pragma once
#include "def/MmoAssert.h"

template <class T>
class Rank {
    const int       _amount; //�Ŷ�����
    int             _last;
    std::vector<T*> _arr;

public:
    Rank(int amount) : _amount(amount), _last(0)
    {
        _arr.resize(_amount + 1, NULL);
    }
    bool OnValueChange(T& obj)
    {
		const int oldRank = obj.rank;

        int newIdx = SearchInsertIdx(obj.GetRankVal());
        if (obj.rank > 0) //���ϰ�
        {
            MoveToIndex(newIdx, obj.rank);
        }
        else if (newIdx > 0) //֮ǰδ�ϰ�
        {
            InsertToIndex(newIdx, obj);
        }
		return oldRank == obj.rank;
    }
    void Clear()
    {
        memset(&_arr[0], 0, _arr.size() * sizeof(T*));
    }
    T*  GetRanker(int rank)
    {
        if (rank > 0 && rank <= _amount)
        {
            return _arr[rank];
        }
        return NULL;
    }
    int GetLastRank() { return _last; }

private:
    int SearchInsertIdx(int dstVal)
    {
        //FIXME�����ֲ����µ���λ
        for (int i = 1; i <= _amount; ++i)
        {
            if (_arr[i] == NULL || dstVal >= _arr[i]->GetRankVal())
            {
                return i;
            }
        }
        return 0;
    }
    void MoveToIndex(int dst, int src)
    {
        T* tmp = _arr[src]; ASSERT(dst > 0 && src > 0);
        if (src > dst) {
            memmove(&_arr[dst + 1], &_arr[dst], (src - dst) * sizeof(T*)); //dst����һ��
            _arr[dst] = tmp;

            for (int i = dst; i <= src; ++i)
            {
                if (_arr[i]) _arr[i]->rank = i;
            }
        } else if (src < dst) {
            memmove(&_arr[src], &_arr[src + 1], (dst - src) * sizeof(T*)); //src+1ǰ��һ��
            _arr[dst] = tmp;

            for (int i = src; i <= dst; ++i)
            {
                if (_arr[i]) _arr[i]->rank = i;
            }
        }
    }
    void InsertToIndex(int idx, T& obj)
    {
        ASSERT(idx > 0);

        if (idx > _last) _last = idx;

        if (_arr[_amount]) _arr[_amount]->rank = 0; //β��������

        memmove(&_arr[idx + 1], &_arr[idx], (_amount - idx) * sizeof(T*));
        _arr[idx] = &obj;

        for (int i = idx; i <= _amount; ++i)
        {
            if (_arr[i]) _arr[i]->rank = i;
        }
    }
};
