@echo off

IF NOT EXIST .\build mkdir .\build
pushd .\build
cl /O2 /MD ..\steamkiller.c ..\steamkiller.res user32.lib shell32.lib
del steamkiller.obj
popd
