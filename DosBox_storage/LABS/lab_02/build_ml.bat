@echo off
rem Build a target with ML under DOS.
rem Usage: build_ml [target]
rem Example: build_ml cycle

if "%1"=="" goto usedefault
set TARGET=%1
goto checksource

:usedefault
set TARGET=program

:checksource
if not exist %TARGET%.asm goto nosource

echo Building %TARGET%.asm with ML...
ml %TARGET%.asm /Fe%TARGET%.exe /Fl%TARGET%.lst /Fm%TARGET%.map /Fo%TARGET%.obj
if errorlevel 1 goto mlfail

echo Build successful: %TARGET%.exe
goto end

:nosource
echo Source file %TARGET%.asm was not found.
goto end

:mlfail
echo ML reported an error.

:end