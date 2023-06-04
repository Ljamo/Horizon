copy Scripts\ReplaceSubmodules\GLFW\premake5.lua Horizon\vendor\GLFW
copy Scripts\ReplaceSubmodules\imgui\premake5.lua Horizon\vendor\imgui
call vendor\bin\premake\premake5.exe vs2022
pause