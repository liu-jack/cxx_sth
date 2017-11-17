
#include "ProtoImporter.h"

#include <stdio.h>



class MyMultiFileErrorCollector : public google::protobuf::compiler::MultiFileErrorCollector
{
        virtual void AddError(
                const std::string & filename,
                int line,
                int column,
                const std::string & message)
        {
        	fprintf(stderr, "%s:%d:%d:%s\n", filename.c_str(), line, column, message.c_str());
       }
};

static MyMultiFileErrorCollector errorCollector;
static google::protobuf::compiler::DiskSourceTree sourceTree;

ProtoImporter& ProtoImporter::GetSingleInstance( void )
{
	static ProtoImporter single ;
	return single ;
}

ProtoImporter::ProtoImporter():
		importer(&sourceTree, &errorCollector)
{
	const char* protopath = getenv("PROTO_PATH");

	// Add CaiBingjie
	std::string selfDir ="./../../src/proto_file/proto_files/" ;
	if( protopath == NULL )
	{
		protopath = selfDir.c_str();
	}
	// End

	if (!protopath)
	{
		sourceTree.MapPath("", "");
	}
	else
	{
		sourceTree.MapPath("", protopath);
	}
	printf("[ProtoImporter] protopath:%s\n", protopath);
}

void ProtoImporter::SetProtoDir( const char *dir )
{
	if( dir != NULL )
	{
		sourceTree.MapPath("", dir);
		printf("[ProtoImporter] set protopath:%s\n", dir);
	}
}


bool ProtoImporter::Import(const std::string& filename)
{
	const  google::protobuf::FileDescriptor* filedescriptor = importer.Import(filename);
	 if (!filedescriptor)
	 {
		 fprintf(stderr, "import (%s) file descriptor error\n", filename.c_str());
		 return false;
	 }
	 return true;
}

google::protobuf::Message* ProtoImporter::createDynamicMessage(const std::string& typeName)
{
	  google::protobuf::Message* message = NULL;
	  const google::protobuf::Descriptor* descriptor = importer.pool()->FindMessageTypeByName(typeName);
	  if (descriptor)
	  {
	     const google::protobuf::Message* prototype = factory.GetPrototype(descriptor);
			if (prototype)
			{
				message = prototype->New();
			}
	  }
	  return message;
}
