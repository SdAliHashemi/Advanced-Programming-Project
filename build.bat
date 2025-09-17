@echo off
title Reservation System [Build Tool]
color 1F
cls

echo Loading...
for /l %%i in (1,1,1) do (
    color 1F
    timeout /t 1 >nul

    color 0F
    timeout /t 1 >nul

)

cls
powershell -Command "Write-Host '=============================================' -ForegroundColor Yellow"
powershell -Command "Write-Host '  Project: Reservation System' -ForegroundColor Yellow"
powershell -Command "Write-Host '  Compile: g++ -std=c++17 FINAL.cpp -o Reservation_System' -ForegroundColor Yellow"
powershell -Command "Write-Host '---------------------------------------------' -ForegroundColor Yellow"
powershell -Command "Write-Host '  DEMO USER (IF THERE IS NO CSV FILE):' -ForegroundColor Cyan"
powershell -Command "Write-Host '   Student -> Email: ali@example.com | Password: 1234' -ForegroundColor Cyan"
powershell -Command "Write-Host '   Admin   -> Email: admin@example.com | Password: admin' -ForegroundColor Cyan"
powershell -Command "Write-Host '=============================================' -ForegroundColor Yellow"
echo.

REM --- Compile ---
g++ -std=c++17 FINAL.cpp -o Reservation_System

if %ERRORLEVEL% NEQ 0 (
    color 4F
    timeout /t 1 >nul
    color 0F
    powershell -Command "Write-Host 'Compilation failed!' -ForegroundColor Red"
    pause
    exit /b %ERRORLEVEL%
)

color 2F



powershell -Command "Write-Host 'Compilation successful!' -ForegroundColor Green"
timeout /t 1 >nul
color 0F
echo Run with: Reservation_System.exe
pause
