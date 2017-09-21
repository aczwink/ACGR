cmake_minimum_required(VERSION 3.6)
project(ACGR)

set(CMAKE_CXX_STANDARD 14) #for lambdas
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1") #currently a requirement for fast ray tracer

#debug or release
if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-D_DEBUG)

    set(CMAKE_DEBUG_POSTFIX "_d")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
	#ok
else()
    message(FATAL_ERROR "Unknown build type. Must be either 'Debug' or 'Release'")
endif()

#dependancies
find_library(ACSTDLIB_LIBRARY NAMES ACStdLib_d)
find_library(ACSTDLIBSTATIC_LIBRARY NAMES ACStdLibStatic_d)

#collect files
add_subdirectory(include)
add_subdirectory(shaders)
add_subdirectory(src)
add_subdirectory(src_demo)

#targets
add_library(ACGR SHARED ${SOURCE_FILES})
target_include_directories(ACGR PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>)

add_executable(demo ${DEMO_SOURCE_FILES})
target_include_directories(demo PUBLIC "include")
target_link_libraries(demo ${ACSTDLIB_LIBRARY} ${ACSTDLIBSTATIC_LIBRARY} ACGR)

#---INSTALLATION---

#install target
install(TARGETS ACGR EXPORT ACGRConfig
	LIBRARY DESTINATION lib
	)
install(EXPORT ACGRConfig DESTINATION share/ACGR/cmake)

#install public headers
function(installHeaderDir sourceDir targetDir)
	file(GLOB LOCAL_FILES_AND_DIRS "${sourceDir}/*")
	foreach(ITEM ${LOCAL_FILES_AND_DIRS})
		IF(IS_DIRECTORY "${ITEM}")
			LIST(APPEND LOCAL_DIRS "${ITEM}")
		ELSE()
			LIST(APPEND LOCAL_FILES "${ITEM}")
		ENDIF()
	endforeach()

	INSTALL(DIRECTORY ${LOCAL_DIRS} DESTINATION "include/${targetDir}")
	INSTALL(FILES ${LOCAL_FILES} DESTINATION include/${targetDir})
endfunction()

installHeaderDir("${PROJECT_SOURCE_DIR}/include" "ACGR")