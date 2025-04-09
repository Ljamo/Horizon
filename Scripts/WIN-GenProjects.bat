copy ReplaceSubmodules\GLFW\premake5.lua ..\Horizon\vendor\GLFW
copy ReplaceSubmodules\imgui\premake5.lua ..\Horizon\vendor\imgui
copy ReplaceSubmodules\imgui\imgui.cpp ..\Horizon\vendor\imgui
copy ReplaceSubmodules\assimp\premake5.lua ..\Horizon\vendor\assimp


@echo off
pushd ..\
call vendor\bin\premake\premake5.exe vs2022
popd
pause