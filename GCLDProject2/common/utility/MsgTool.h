#ifndef MsgTool_h__
#define MsgTool_h__


#include "GameServer.pb.h"
#include "common/Enum.pb.h"
#include "def/TypeDef.h"
#include "boost/dynamic_bitset.hpp"
#include "boost/function.hpp"
#include <algorithm>

namespace MsgTool
{
    template< typename T , class Msg>
    inline Msg* SaveToRepeatField( T* table, ::google::protobuf::RepeatedPtrField< Msg >* field)
    {
        if ( table != NULL)
        {
            Msg* msg = field->Add();
            if ( msg )
            {
                table->SaveTo( *msg);
            }
            return msg;
        }
        return NULL;
    }

    template< typename T , class MSG>
    void SaveVecToMsg( const std::vector<T>& vec, MSG& list)
    {
        std::copy(vec.begin(), vec.end(),
            RepeatedFieldBackInserter(list.mutable_values()));
    }

    template< typename T , class MSG>
    void SaveVecToMsg( const T* arr, int arr_size, MSG& list)
    {
        std::copy(arr, arr + arr_size,
            RepeatedFieldBackInserter(list.mutable_values()));
    }

    template< typename T , class MSG>
    void SaveMsgToVec(const MSG& list, std::vector<T>& vec)
    {
        vec.reserve(list.values_size());
        std::copy(
            list.values().begin(),
            list.values().end(),
            std::inserter(vec, vec.end())
            );
    }

    template< typename T , class MSG>
    void SaveMsgToVec(const MSG& list, T* arr, int arr_size)
    {
        for ( int i = 0; i < arr_size && i < list.values_size(); ++i )
        {
            arr[i] = list.values(i);
        }
    }


    template< class UpdateMSG, typename TValue>
    void SaveUpdateMsgTo(const UpdateMSG& msg , std::vector<TValue>& values)
    {
        boost::dynamic_bitset<uint32> modbit( msg.bit_count());
        boost::from_block_range( msg.bit_masks().begin(), msg.bit_masks().end(), modbit);

        for ( size_t i = modbit.find_first(), index = 0;

            i != boost::dynamic_bitset<uint32>::npos 
            && index < (size_t)msg.values_size() 
            && i < values.size();

        i = modbit.find_next(i), ++index)
        {
            values[i] = msg.values(index);
        }

    }

    template< class UpdateMSG, typename TValue>
    void SaveUpdateMsgTo(const UpdateMSG& msg , TValue* values, size_t values_size, size_t offset = 0, bool* remainInfo = NULL)
    {
        boost::dynamic_bitset<uint32> modbit( msg.bit_count());
        boost::from_block_range( msg.bit_masks().begin(), msg.bit_masks().end(), modbit);

        for ( size_t i = modbit.find_first(), index = 0;

            i != boost::dynamic_bitset<uint32>::npos 
            && index < (size_t)msg.values_size() 
            /*&& i < values_size*/;

        i = modbit.find_next(i), ++index)
        {
            if ( i >= offset && i - offset < values_size)
            {
                values[i - offset] = msg.values(index);
            }
            else
            {
                if ( remainInfo != NULL)
                {
                    *remainInfo = true;
                }                
            }
        }
    }  

	template< class TValue, class UpdateMSG>
	void SaveUpdateMsgTo(const UpdateMSG& msg , const boost::function< TValue*(uint32)>& ValueIndexGetter, size_t values_size)
	{
		boost::dynamic_bitset<uint32> modbit( msg.bit_count());
		boost::from_block_range( msg.bit_masks().begin(), msg.bit_masks().end(), modbit);

		for ( size_t i = modbit.find_first(), index = 0;

				i != boost::dynamic_bitset<uint32>::npos 
				&& index < (size_t)msg.values_size() 
				&& i < values_size;

				i = modbit.find_next(i), ++index)
		{
			if ( TValue* value = ValueIndexGetter(i))
			{
				*value = msg.values(index);
			}
		}
	}  

	template< class TValue, class UpdateMSG>
	bool SaveAndCheckUpdateMsg(const UpdateMSG& msg , const boost::function< TValue*(uint32)>& ValueIndexGetter,const boost::function< bool(uint32)>& ValueIndexCheck, size_t values_size)
	{
		bool needSyncSave = false;
		boost::dynamic_bitset<uint32> modbit( msg.bit_count());
		boost::from_block_range( msg.bit_masks().begin(), msg.bit_masks().end(), modbit);

		for ( size_t i = modbit.find_first(), index = 0;

			i != boost::dynamic_bitset<uint32>::npos 
			&& index < (size_t)msg.values_size() 
			&& i < values_size;

		i = modbit.find_next(i), ++index)
		{
			if ( TValue* value = ValueIndexGetter(i))
			{
				*value = msg.values(index);

				if(ValueIndexCheck(i))
				{
					needSyncSave = true;
				}
			}
		}
		return needSyncSave;
	}  
}


#endif // MsgTool_h__
