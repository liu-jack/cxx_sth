@echo off
set ProtoFilePath="./proto_files"
set GenerateOutCPPPath="./output_cpp"
base_define_port.py

%cd%/proto_files/common/ProtoEnumIndexCreate.exe %cd%/proto_files/common/Opcode.proto 1
%cd%/proto_files/common/ProtoEnumIndexCreate.exe %cd%/proto_files/common/ServerOpcode.proto 1
%cd%/proto_files/common/ProtoEnumIndexCreate.exe %cd%/proto_files/common/ENUM.proto 1


for  %%i in (%ProtoFilePath%/*.proto) do (
   @echo Generate finished:%%i
   protoc -I=%ProtoFilePath% --cpp_out=%GenerateOutCPPPath% %ProtoFilePath%/%%i
)

for /d %%i in (%ProtoFilePath%/*) do (
   call:ProtoSubDirFun %%i
)
exit
:ProtoSubDirFun
@echo Proto File Path: %1
set CurSubDirPath=%1
for  %%i in (%ProtoFilePath%/%CurSubDirPath%/*.proto) do (
   @echo Generate finished:%%i
   protoc -I=%ProtoFilePath%/%CurSubDirPath% --cpp_out=%GenerateOutCPPPath%/%CurSubDirPath% %ProtoFilePath%/%CurSubDirPath%/%%i
)
pause