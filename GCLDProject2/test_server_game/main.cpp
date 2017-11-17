#include "gtest/gtest.h"
#include <google/protobuf/stubs/common.h>
#include "DatabaseMgr.h"
#include "gtest/memCheck.h"
#include <vld.h>

extern void LoadDBTables();

int main(int argc, char* argv[])
{
    ENABLE_MEMORY_LEAK_CHECK;

    _CrtSetBreakAlloc(0);
    //LoadDBTables();

    testing::InitGoogleTest(&argc, argv);
    int nRet = RUN_ALL_TESTS();
    //DatabaseMgr::deleteInstance();

    if (argc <=1)
    {
        getchar();
    }

    google::protobuf::ShutdownProtobufLibrary();

    return nRet;
}


void OnCrash(bool Terminate)
{

}