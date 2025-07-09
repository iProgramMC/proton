cmake_minimum_required(VERSION 3.10)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/../bin)

if(NOT DEFINED ENV{DEVKITPPC})
	message(FATAL_ERROR "Please define the DEVKITPPC environment variable.")
endif()

#append the path to the tools
set(ENV{PATH} "$ENV{DEVKITPPC}/bin:$ENV{PATH}")

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER powerpc-eabi-gcc)
set(CMAKE_CXX_COMPILER powerpc-eabi-g++)

include_directories("$ENV{DEVKITPRO}/libogc/include")
link_directories("$ENV{DEVKITPRO}/libogc/lib/wii")

# if(NOT CMAKE_BUILD_TYPE)
#   set(CMAKE_BUILD_TYPE "Release")
# endif()
set(CMAKE_BUILD_TYPE "Debug")

ENABLE_LANGUAGE(C)

# suppress some crappy warnings
add_definitions(-DBOOST_BIND_GLOBAL_PLACEHOLDERS -DC_GL_MODE)

set(BASE_FLAGS "-DGEKKO -mrvl -mcpu=750 -meabi -mhard-float")

set(CMAKE_C_FLAGS "${BASE_FLAGS} -Wno-old-style-definition")
set(CMAKE_CXX_FLAGS "${BASE_FLAGS}")

add_definitions(-DRT_WII)

project(RTSimpleApp)
include(../../shared/wii/Proton.cmake)

proton_wii_opengx()
proton_use_wii_audio()
proton_use_internal_zlib()

proton_include_all_components()
proton_use_jpeg_support()
proton_use_linearparticles()

set(APP "../source")
include_directories("${APP}")
proton_set_sources("${APP}/App.cpp" "${APP}/Component/ParticleTestComponent.cpp" "${APP}/GUI/DebugMenu.cpp" "${APP}/GUI/EnterNameMenu.cpp"
    "${APP}/GUI/MainMenu.cpp" "${APP}/GUI/ParticleTestMenu.cpp" "${APP}/GUI/AboutMenu.cpp" 
    "${APP}/GUI/TouchTestMenu.cpp" "${APP}/Component/TouchTestComponent.cpp" 
    "${APP}/GUI/MainMenu.cpp" "${APP}/GUI/UTF16TestMenu.cpp")

target_link_libraries(RTSimpleApp)

proton_convert_dol()
