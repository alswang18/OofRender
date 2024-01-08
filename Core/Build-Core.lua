project "Core"
   print("Building Core")
   kind "StaticLib"
   language "C++"
   cppdialect "C++17"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp", "Vendor/**.c", "Vendor/glm/**.hpp" }

   includedirs
   {
      "Vendor",
      "Vendor/glm",
      "../Vendor/Libraries/GLFW/Include",
      "../Vendor/Libraries/glad/Include",
      "Source"
   }

    libdirs
    {
        "../Vendor/Libraries/GLFW/Lib"
    }
    
    links
    {
        "glfw3.lib",
    }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")
   
   -- prevents linking to MSVCRT
   linkoptions { "/NODEFAULTLIB:MSVCRT" }

   filter "system:windows"
       systemversion "latest"
       defines { }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"

    print("Finished Building Core")