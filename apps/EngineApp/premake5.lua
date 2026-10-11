-- apps/EngineApp/premake5.lua

project "EngineApp"
    location (path.join(BuildRoot, "EngineApp"))
    kind "WindowedApp"
    rtti "On"

    files {
        "include/**.h",
        "src/**.cpp"
    }

    includedirs { 
        "include",
        path.join(_MAIN_SCRIPT_DIR, "modules/EngineLib/include"),
        path.join(_MAIN_SCRIPT_DIR, "third_party")
    }
    links { "EngineLib", "ThirdParty" }

    -- Assets/, Shaders/ 상대 경로의 기준
    debugdir(_MAIN_SCRIPT_DIR)