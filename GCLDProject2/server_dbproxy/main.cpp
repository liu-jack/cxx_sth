
#ifdef WIN32
#include <vld.h>
#endif

#include "def/MmoAssert.h"
#include "google/protobuf/stubs/common.h"

extern int run_server(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	// GOOGLE_PROTOBUF_VERIFY_VERSION, Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	int ret = 0 ;
	ret = run_server(argc, argv);

	// Optional:  Delete all global objects allocated by libprotobuf.
	google::protobuf::ShutdownProtobufLibrary() ;

	return ret ;
}

