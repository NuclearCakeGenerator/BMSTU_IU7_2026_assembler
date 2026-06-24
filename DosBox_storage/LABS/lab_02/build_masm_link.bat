@echo off
rem Build a target with MASM and LINK under DOS.
rem Usage: build_masm_link [target]
rem Example: build_masm_link cycle

if "%1"=="" goto usedefault
set TARGET=%1
goto checksource

:usedefault
set TARGET=program

:checksource
if not exist %TARGET%.asm goto nosource

echo Assembling %TARGET%.asm...
masm %TARGET%.asm,%TARGET%.obj,%TARGET%.lst,;
if errorlevel 1 goto asmfail

echo Linking %TARGET%.obj...
link %TARGET%.obj,%TARGET%.exe,%TARGET%.map,;
if errorlevel 1 goto linkfail

echo Build successful: %TARGET%.exe
goto end

:nosource
echo Source file %TARGET%.asm was not found.
goto end

:asmfail
echo MASM reported an error.
goto end

:linkfail
echo LINK reported an error.

:end