@echo off
REM run_utf8.bat - switch console to UTF-8 then run built exe in Debug/Release
chcp 65001 >nul

REM Try Debug foldernif exist "Debug\*.exe" (
    pushd Debug
    for %%f in (*.exe) do (
        echo Running %%f
        "%%~f"
        popd
        goto :eof
    )
)

REM Try Release folder
if exist "Release\*.exe" (
    pushd Release
    for %%f in (*.exe) do (
        echo Running %%f
        "%%~f"
        popd
        goto :eof
    )
)

REM Search recursively for any exe and run the first found
for /r %%f in (*.exe) do (
    echo Running %%~f
    "%%~f"
    goto :eof
)

echo No executable found. Please build the project first.
pause
