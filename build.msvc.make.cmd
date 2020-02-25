@echo off
rem Public domain
rem http://unlicense.org/
rem Created by Grigore Stefan <g_stefan@yahoo.com>

set ACTION=%1
if "%1" == "" set ACTION=make

echo -^> %ACTION% quantum-script-extension-datetime

goto StepX
:cmdX
%*
if errorlevel 1 goto cmdXError
goto :eof
:cmdXError
echo "Error: %ACTION%"
exit 1
:StepX

call :cmdX xyo-cc --mode=%ACTION% @util/quantum-script-extension-datetime.static.compile.info
call :cmdX xyo-cc --mode=%ACTION% @util/quantum-script-extension-datetime.dynamic.compile.info

