@echo off
REM Build and run the test suite (assumes tests built to tests\test_suite.exe)
if exist "tests\test_suite.exe" (
    echo Running tests\test_suite.exe
    tests\test_suite.exe
) else (
    echo Test executable not found. Build tests first.
)
pause
