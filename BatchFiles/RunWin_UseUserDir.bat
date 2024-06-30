@echo off
setlocal

REM Path to the Unreal Editor executable
set UE5_EDITOR_PATH="F:\UnrealEngine\UE_5_Source\Engine\Binaries\Win64\UnrealEditor.exe"

REM Relative path to your Unreal project
set REL_PROJECT_PATH="..\DogFight.uproject"

REM Convert relative path to absolute path
pushd %~dp0
for %%i in (%REL_PROJECT_PATH%) do set ABS_PROJECT_PATH=%%~dpfxi
popd

REM Additional command line arguments (if any)
set CMD_ARGS=-game -windowed -ResX=1920 -ResY=1080 -SaveToUserDir

REM Debugging: Print paths to verify
echo UE5_EDITOR_PATH=%UE5_EDITOR_PATH%
echo PROJECT_PATH=%ABS_PROJECT_PATH%

REM Run the Unreal Editor in standalone mode
%UE5_EDITOR_PATH% %ABS_PROJECT_PATH% %CMD_ARGS%

endlocal
