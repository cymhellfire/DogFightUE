@echo off

rem Get protoc path
set PROTOC_PATH=..\Source\ThirdParty\libprotobuf\bin\protoc.exe

rem Set Output path
set OUTPUT_PB_PATH=.\Config\Config.pb
set OUTPUT_CPP_PATH=..\Source\ProtobufIntegration\Public\GenProto\

rem Set Input Path
set GAME_PROTO_PATH=.\Proto\
set LIB_PATH=..\Source\ThirdParty\libprotobuf\include\google\protobuf\

rem Convert config proto
echo Convert config
%PROTOC_PATH% -o %OUTPUT_PB_PATH% --cpp_out %OUTPUT_CPP_PATH% -I=%GAME_PROTO_PATH% -I=%LIB_PATH% %GAME_PROTO_PATH%Config.proto %GAME_PROTO_PATH%xresloader.proto %GAME_PROTO_PATH%xresloader_ue.proto

rem Convert tools
echo Convert tools

REM %PROTOC_PATH% --cpp_out %OUTPUT_CPP_PATH% -I=%GAME_PROTO_PATH% %GAME_PROTO_PATH%xresloader.proto
REM %PROTOC_PATH% --cpp_out %OUTPUT_CPP_PATH% -I=%GAME_PROTO_PATH% %GAME_PROTO_PATH%pb_header_v3.proto

echo Finish
pause