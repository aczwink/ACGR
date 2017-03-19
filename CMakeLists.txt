cmake_minimum_required(VERSION 3.6)
project(AC3D)

set(CMAKE_CXX_STANDARD 11)

#debug or release
if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-D_DEBUG)

    set(CMAKE_DEBUG_POSTFIX "_d")
else()
    message(FATAL_ERROR "Unknown build type. Must be either 'Debug' or 'Release'")
endif()

#dependancies
find_library(ACSTDLIB_LIBRARY NAMES ACStdLib_d)
find_library(ACSTDLIBSTATIC_LIBRARY NAMES ACStdLibStatic_d)

#targets
add_subdirectory(src_demo)

#add_library(AC3D SHARED ${SOURCE_FILES})
add_executable(demo ${DEMO_SOURCE_FILES})
target_link_libraries(demo ${ACSTDLIB_LIBRARY} ${ACSTDLIBSTATIC_LIBRARY})