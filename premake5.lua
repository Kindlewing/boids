workspace("boids")
configurations({ "debug", "release" })
platforms({ "x64" })
location("build")

language("C")
cdialect("C99")

filter("configurations:debug")
defines({ "DEBUG" })
symbols("On")

filter("configurations:release")
defines({ "NDEBUG" })
optimize("On")

filter({})

-- ========================
-- Base library
-- ========================
include("lib/base")

-- ========================
-- Spark library
-- ========================
include("lib/spark")

-- ========================
-- Boids executable
-- ========================
project("boids")
kind("ConsoleApp")

targetdir("build/bin/%{cfg.buildcfg}")
objdir("build/obj/%{prj.name}/%{cfg.buildcfg}")

files({
	"src/**.c",
	"include/**.h",
})

includedirs({
	"include",
	"lib/base/include",
	"lib/spark/include",
})

links({
	"GL",
	"X11",
	"base",
	"spark",
})

filter("toolset:gcc or toolset:clang")
buildoptions({ "-Wall", "-Wextra" })

filter({})
