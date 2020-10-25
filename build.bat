@echo off

IF NOT EXIST .\build_win32 mkdir .\build_win32
pushd .\build_win32
    cl -nologo -Z7 -Incremental:no ..\clike.c
popd