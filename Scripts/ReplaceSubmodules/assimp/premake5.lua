-- How to use:
-- 0. Get assimp from https://github.com/assimp/assimp
-- 1. Place this file next to the cloned assimp repository like this:
--
--       assimp.lua
--       assimp/
--
-- 2. Set up configuration headers
--
--     2.1 Create a folder for configuration headers, I usually name it _config_headers/
--
--          _config_headers/assimp/     <- config.h will be included as "assimp/config.h" so the assimp/ part is important
--          assimp.lua
--          assimp
--
--     2.2 Generate config.h and revision.h from assimp/code/config.h.in and assimp/revision.h.in and put them into _config_headers/assimp. (You can also just edit and rename them, that's what I usually end up doing.)
--
--          _config_headers/
--            assimp/
--              config.h
--              revision.h
--          assimp.lua
--          assimp
--
-- 3. Edit the 'files { ... }' and 'defines { ... }' depending on which importers/exporters you need. By default I enabled COLLADA, OBJ, FBX and ASSBIN (assimp binary, good for caching)

project '*'
	includedirs {
		'_config_headers/',
		'_config_headers/assimp/', -- Location of assimp's config.h, for a template see include/assimp/config.h.in
		'assimp/include/',
	}
project 'assimp'
	kind 'StaticLib'
	warnings 'Off'
	-- optimize 'Speed'
	cppdialect "C++17"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	includedirs {
		'_config_headers/',
		'_config_headers/assimp/',
		'assimp/',
		'assimp/contrib/',
		'assimp/contrib/irrXML/',
		'assimp/contrib/unzip/',
		'assimp/contrib/rapidjson/include/',
		'assimp/contrib/pugixml/src/',
		'assimp/contrib/zlib/',
		'assimp/contrib/utf8cpp/source',
		'assimp/code',
		'assimp/include',
	}
	files {
		-- Dependencies
		'assimp/contrib/unzip/**',
		'assimp/contrib/irrXML/**',
		'assimp/contrib/zlib/*',
		-- Common
		'assimp/code/Common/**',
		'assimp/code/PostProcessing/**',
		'assimp/code/Material/**',
		'assimp/code/CApi/**',
		'assimp/code/Geometry/**',
		-- Importers
		'assimp/code/AssetLib/IQM/**',
		'assimp/code/AssetLib/Assbin/**',

		'assimp/code/AssetLib/Collada/**',
		'assimp/code/AssetLib/Obj/**',
		-- 'assimp/code/AssetLib/Blender/**', 'assimp/contrib/poly2tri/poly2tri/**',
		'assimp/code/AssetLib/FBX/**',
		'assimp/code/AssetLib/glTF2/**',
		'assimp/code/AssetLib/glTF/**',
		'assimp/code/AssetLib/glTFCommon/**',
		'assimp/code/Assbin/**' -- Very fast format to parse/write, useful for caching
	}
	-- Importers
	defines {
        "ASSIMP_BUILD_NO_OWN_ZLIB",


		'ASSIMP_BUILD_NO_3D_IMPORTER',
		'ASSIMP_BUILD_NO_3DS_IMPORTER',
		'ASSIMP_BUILD_NO_3MF_IMPORTER',
		'ASSIMP_BUILD_NO_AC_IMPORTER',
		'ASSIMP_BUILD_NO_AMF_IMPORTER',
		'ASSIMP_BUILD_NO_ASE_IMPORTER',
		'ASSIMP_BUILD_NO_ASSBIN_IMPORTER',
		'ASSIMP_BUILD_NO_B3D_IMPORTER',
		'ASSIMP_BUILD_NO_BLEND_IMPORTER',
		'ASSIMP_BUILD_NO_BVH_IMPORTER',
		'ASSIMP_BUILD_NO_C4D_IMPORTER',
		'ASSIMP_BUILD_NO_COB_IMPORTER',
		'ASSIMP_BUILD_NO_COLLADA_IMPORTER',
		'ASSIMP_BUILD_NO_CSM_IMPORTER',
		'ASSIMP_BUILD_NO_DXF_IMPORTER',
		-- 'ASSIMP_BUILD_NO_FBX_IMPORTER',
		-- 'ASSIMP_BUILD_NO_GLTF_IMPORTER',
        "ASSIMP_BUILD_NO_USD_IMPORTER",
		'ASSIMP_BUILD_NO_HMP_IMPORTER',
		'ASSIMP_BUILD_NO_IFC_IMPORTER',
		'ASSIMP_BUILD_NO_IRR_IMPORTER',
		'ASSIMP_BUILD_NO_IRRMESH_IMPORTER',
		'ASSIMP_BUILD_NO_LWO_IMPORTER',
		'ASSIMP_BUILD_NO_LWS_IMPORTER',
		'ASSIMP_BUILD_NO_M3D_IMPORTER',
		'ASSIMP_BUILD_NO_MD2_IMPORTER',
		'ASSIMP_BUILD_NO_MD3_IMPORTER',
		'ASSIMP_BUILD_NO_MD5_IMPORTER',
		'ASSIMP_BUILD_NO_MDC_IMPORTER',
		'ASSIMP_BUILD_NO_MDL_IMPORTER',
		'ASSIMP_BUILD_NO_MMD_IMPORTER',
		'ASSIMP_BUILD_NO_MS3D_IMPORTER',
		'ASSIMP_BUILD_NO_NDO_IMPORTER',
		'ASSIMP_BUILD_NO_NFF_IMPORTER',
		'ASSIMP_BUILD_NO_OBJ_IMPORTER',
		'ASSIMP_BUILD_NO_OFF_IMPORTER',
		'ASSIMP_BUILD_NO_OGRE_IMPORTER',
		'ASSIMP_BUILD_NO_OPENGEX_IMPORTER',
		'ASSIMP_BUILD_NO_PLY_IMPORTER',
		'ASSIMP_BUILD_NO_Q3BSP_IMPORTER',
		'ASSIMP_BUILD_NO_Q3D_IMPORTER',
		'ASSIMP_BUILD_NO_RAW_IMPORTER',
		'ASSIMP_BUILD_NO_SIB_IMPORTER',
		'ASSIMP_BUILD_NO_SMD_IMPORTER',
		'ASSIMP_BUILD_NO_STEP_IMPORTER',
		'ASSIMP_BUILD_NO_STL_IMPORTER',
		'ASSIMP_BUILD_NO_TERRAGEN_IMPORTER',
		'ASSIMP_BUILD_NO_X_IMPORTER',
		'ASSIMP_BUILD_NO_X3D_IMPORTER',
		'ASSIMP_BUILD_NO_XGL_IMPORTER'
	}
	-- Exporters
	defines {
		'ASSIMP_BUILD_NO_COLLADA_EXPORTER',
		'ASSIMP_BUILD_NO_X_EXPORTER',
		'ASSIMP_BUILD_NO_STEP_EXPORTER',
		'ASSIMP_BUILD_NO_OBJ_EXPORTER',
		'ASSIMP_BUILD_NO_STL_EXPORTER',
		'ASSIMP_BUILD_NO_PLY_EXPORTER',
		'ASSIMP_BUILD_NO_3DS_EXPORTER',
		'ASSIMP_BUILD_NO_GLTF_EXPORTER',
		-- 'ASSIMP_BUILD_NO_ASSBIN_EXPORTER',
		'ASSIMP_BUILD_NO_ASSXML_EXPORTER',
		'ASSIMP_BUILD_NO_X3D_EXPORTER',
		'ASSIMP_BUILD_NO_FBX_EXPORTER',
		'ASSIMP_BUILD_NO_M3D_EXPORTER',
		'ASSIMP_BUILD_NO_3MF_EXPORTER',
		'ASSIMP_BUILD_NO_ASSJSON_EXPORTER',
        "ASSIMP_BUILD_NO_PBRT_EXPORTER",
        'RAPIDJSON_HAS_STDSTRING=1'

	}

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"