@echo off
set CURRENT_DIR=%CD%
set ProtoFilePath=%CURRENT_DIR%\proto_files
set GenerateOutCSPath=%CURRENT_DIR%\..\..\..\..\client\GameClient_GamePlay\Assets\Scripts\ProtoFile
set GEN=%CURRENT_DIR%\ProtoGen\protogen

%cd%/proto_files/common/ProtoEnumIndexCreate.exe %cd%/proto_files/common/ServerOpcode.proto 1
%cd%/proto_files/common/ProtoEnumIndexCreate.exe %cd%/proto_files/common/ENUM.proto 1


for  %%i in (%ProtoFilePath%\*.proto) do (
    @echo Generate finished:%%~ni
    pushd %%~pi
    %GEN% -i:%%~ni.proto -o:%GenerateOutCSPath%\%%~ni.cs
    popd
)

for  %%i in (%ProtoFilePath%\common\*.proto) do (
    @echo Generate finished:%%~ni
    pushd %%~pi
    %GEN% -i:%%~ni.proto -o:%GenerateOutCSPath%\%%~ni.cs
    popd
)
