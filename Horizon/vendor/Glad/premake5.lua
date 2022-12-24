project "Glad"
    kind "StaticLib"
    language "C"
    cppdialect "C++17"
    staticruntime "On"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
    }

	includedirs
	{
		"include"
	}

    filter "system:windows"
       systemversion "latest"
        
    filter { "system:windows", "configurations:Release" }
    buildoptions "/MT"
    runtime "Release"
    symbols "on"

    filter {"system:windows", "configurations:Debug"}
        runtime "Debug"
        symbols "on"