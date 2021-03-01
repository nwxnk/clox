workspace "clox"
    configurations { "debug", "release" }

project "clox"
    kind "ConsoleApp"
    language "C"
    location "build"
    targetdir "build/%{cfg.buildcfg}"

    files { "**.h", "**.c" }
    includedirs { "src/include" }

    filter "configurations:debug"
        defines { "DEBUG_PRINT_CODE", "DEBUG_TRACE_EXECUTION" }
        symbols "On"

    filter "configurations:release"
        optimize "On"

    newaction {
        trigger     = "clean",
        description = "clean the project",
        execute     = function ()
            os.rmdir("./build")
            os.remove("Makefile")
        end
    }