copy ReplaceSubmodules\GLFW\premake5.lua ..\Horizon\vendor\GLFW
copy ReplaceSubmodules\imgui\premake5.lua ..\Horizon\vendor\imgui
@echo off
pushd ..\
call vendor\bin\premake\premake5.exe vs2022
popd
pause