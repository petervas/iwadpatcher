@echo off
setlocal

echo Running IWAD patcher tests on Windows...
echo.

rem Check if main IWAD files are present
for %%i in (DOOM.WAD DOOM2.WAD PLUTONIA.WAD TNT.WAD HERETIC.WAD HEXEN.WAD HEXDD.WAD STRIFE1.WAD) do (
    if not exist "%%i" (
        echo Error: Required file %%i not found.
        exit /b 1
    )
)
echo All required files are present.
echo.

rem Variable to track if any test failed
set all_tests_passed=true

rem Run all tests
call :run_test DOOM_11.WAD DOOM_11 DOOM.WAD
call :run_test DOOM_11_KEX.WAD DOOM_KEX DOOM_11.WAD
call :run_test DOOM_KEX_11.WAD DOOM_11 DOOM_11_KEX.WAD
call :run_test DOOM2_1666G.WAD DOOM2_1666G DOOM2.WAD
call :run_test DOOM2_1666G_KEX.WAD DOOM2_KEX DOOM2_1666G.WAD
call :run_test DOOM2_KEX_1666G.WAD DOOM2_1666G DOOM2_1666G_KEX.WAD
call :run_test PLUTONIA_19.WAD PLUTONIA_19 PLUTONIA.WAD
call :run_test PLUTONIA_19_KEX.WAD PLUTONIA_KEX PLUTONIA_19.WAD
call :run_test PLUTONIA_KEX_19.WAD PLUTONIA_19 PLUTONIA_19_KEX.WAD
call :run_test TNT_19.WAD TNT_19 TNT.WAD
call :run_test TNT_19_KEX.WAD TNT_KEX TNT_19.WAD
call :run_test TNT_KEX_19.WAD TNT_19 TNT_19_KEX.WAD
call :run_test HERETIC_10.WAD HERETIC_10 HERETIC.WAD
call :run_test HERETIC_10_13.WAD HERETIC_13 HERETIC_10.WAD
call :run_test HERETIC_13_10.WAD HERETIC_10 HERETIC_10_13.WAD
call :run_test HEXEN_10.WAD HEXEN_10 HEXEN.WAD
call :run_test HEXEN_10_11.WAD HEXEN_11 HEXEN_10.WAD
call :run_test HEXEN_11_10.WAD HEXEN_10 HEXEN_10_11.WAD
call :run_test HEXDD_10.WAD HEXDD_10 HEXDD.WAD
call :run_test HEXDD_10_11.WAD HEXDD_11 HEXDD_10.WAD
call :run_test HEXDD_11_10.WAD HEXDD_10 HEXDD_10_11.WAD
call :run_test STRIFE1_10.WAD STRIFE1_10 STRIFE1.WAD
call :run_test STRIFE1_10_131.WAD STRIFE1_131 STRIFE1_10.WAD
call :run_test STRIFE1_131_10.WAD STRIFE1_10 STRIFE1_10_131.WAD

rem Cleanup WAD files
del /Q "DOOM_11.WAD" "DOOM_11_KEX.WAD" "DOOM_KEX_11.WAD" "DOOM2_1666G.WAD" "DOOM2_1666G_KEX.WAD" "DOOM2_KEX_1666G.WAD" "PLUTONIA_19.WAD" "PLUTONIA_19_KEX.WAD" "PLUTONIA_KEX_19.WAD" "TNT_19.WAD" "TNT_19_KEX.WAD" "TNT_KEX_19.WAD" "HERETIC_10.WAD" "HERETIC_10_13.WAD" "HERETIC_13_10.WAD" "HEXEN_10.WAD" "HEXEN_10_11.WAD" "HEXEN_11_10.WAD" "HEXDD_10.WAD" "HEXDD_10_11.WAD" "HEXDD_11_10.WAD" "STRIFE1_10.WAD" "STRIFE1_10_131.WAD" "STRIFE1_131_10.WAD"

rem Print result
echo.
if "%all_tests_passed%"=="true" (
    echo All tests passed successfully.
    exit /b 0
) else (
    echo One or more tests failed.
    exit /b 1
)

rem Subroutine to run a test and check for errors
:run_test
    iwadpatcher -O%1 -V%2 %3
    if ERRORLEVEL 1 (
        echo Error running test for %1
        set all_tests_passed=false
    )
    goto :eof

endlocal
