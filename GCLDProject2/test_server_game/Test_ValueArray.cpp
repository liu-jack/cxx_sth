#include "gtest/gtest.h"

#include "memory_buffer/ValueArray.h"
#include "CommomDef.pb.h"


TEST( ValueArray, ValueApply)
{
    ValueArray arr(3);

    arr.SetInt( 0 , 0);
    arr.SetInt( 1 , 1);
    arr.SetInt( 2 , 2);

    arr.SetUnmodified();

    arr.SetInt( 2 , 3);

    pb::Int32ListUpdate msg;
    arr.BuildUpdateMsg( msg);

    arr.SetInt( 2 , 4);

    arr.ApplyUpdateMsg( msg);

    EXPECT_EQ( arr.GetInt(2), 3);
}


