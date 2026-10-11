-- tests/premake5.lua
project "UnitTest"
    location (path.join(BuildRoot, "tests"))
    kind "ConsoleApp"

    files {
        "**.h",
        "**.cpp"
    }

    includedirs { 
        "tests", 
        path.join(_MAIN_SCRIPT_DIR, "modules/EngineLib/include"), 
        path.join(_MAIN_SCRIPT_DIR, "third_party") 
    }
    links { "EngineLib", "ThirdParty" }

    pchheader "pch.h"
    pchsource "pch.cpp"

    nuget {
        "Microsoft.googletest.v140.windesktop.msvcstl.static.rt-dyn:1.8.1.8"
    }

    debugdir(_MAIN_SCRIPT_DIR)
    