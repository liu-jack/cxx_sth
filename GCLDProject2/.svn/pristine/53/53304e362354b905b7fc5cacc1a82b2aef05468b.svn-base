
#ifndef PROTOIMPORTER_H_
#define PROTOIMPORTER_H_

#include <string>

#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/compiler/importer.h>

class ProtoImporter
{

public:
	ProtoImporter();
	static ProtoImporter& GetSingleInstance( void ) ;

public:
	bool Import(const std::string& filename);
	google::protobuf::Message* createDynamicMessage(const std::string& typeName);

	void SetProtoDir( const char *dir ) ;

public:
	google::protobuf::compiler::Importer importer;
	google::protobuf::DynamicMessageFactory factory;
};

#define sProtoImporter ProtoImporter::GetSingleInstance()

#endif
