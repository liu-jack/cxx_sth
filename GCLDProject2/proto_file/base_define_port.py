import os
import sys



        
workDir = os.getcwd()
mysqlCmd = workDir +"\\..\\..\\database\\mysql_bin\\mysql.exe"
dbhost = "192.168.2.142"
dbName = "slg_db"
userName = "root"
password = "123456"

print (mysqlCmd + " -h" + dbhost + " -u" + userName + " -p" + password + " -N -B -e \"SELECT `name`,`value` from `" +dbName +"`.`base_define`\"")


ProtoFileName = workDir + "\\proto_files\\common\\BaseDefine.proto";
CHeaderFileName = workDir +"\\..\\common\\def\\DBBaseDefine.h"
print (ProtoFileName)

result = os.popen(mysqlCmd + " -h" + dbhost + " -u" + userName + " -p" + password + " -N -B -e \"SET NAMES UTF8;SELECT `name`,`id`,`value` from `" +dbName +"`.`base_define`\"",'r')

# cHeaderFileHandle = open( CHeaderFileName, "w")
# cHeaderFileHandle.write( "#pragma once\n\n\n")

pbFileHandle = open( ProtoFileName, "w")
pbFileHandle.write( "package pb;\n")
pbFileHandle.write( "enum BASE_DEFINE\n")
pbFileHandle.write( "{\n")

for line in result:
    line = line.rstrip().split("\t", 3)
    pbFileHandle.write("\tBD_%(name)-60s= %(id)s;\n" % { "name" : line[0] , "id" : line[1] } )
    if line[2].find('.') != -1 :
        line[2] += 'f'
    # cHeaderFileHandle.write("#define BD_%(name)-60s %(value)s\n"% { "name" : line[0] , "value" : line[2] })
pbFileHandle.write( "}\n");

pbFileHandle.close()
# cHeaderFileHandle.close();
