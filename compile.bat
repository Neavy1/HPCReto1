@echo off
echo Compilando programas para el calculo de pi...

REM Compilar version serial
echo Compilando version serial...
cl /Fe:dart_serial.exe Dart.c /link
if %errorlevel% neq 0 (
    echo Error al compilar version serial
    pause
    exit /b 1
)

REM Compilar version con hilos
echo Compilando version con hilos...
cl /Fe:dart_threads.exe DartBoardH.c /link
if %errorlevel% neq 0 (
    echo Error al compilar version con hilos
    pause
    exit /b 1
)

REM Compilar version con procesos
echo Compilando version con procesos...
cl /Fe:dart_processes.exe DartBoardF.c /link
if %errorlevel% neq 0 (
    echo Error al compilar version con procesos
    pause
    exit /b 1
)

echo.
echo Compilacion exitosa!
echo.
echo Para ejecutar las pruebas:
echo dart_serial.exe 1000000
echo dart_threads.exe 1000000 4
echo dart_processes.exe 1000000 4
echo.
pause
