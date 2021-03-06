cmake_minimum_required(VERSION 3.6)
project(ACGR)
find_package(XP-CMake REQUIRED) #we need advanced cmake modules

set(CMAKE_CXX_STANDARD 17)

XPC_AutoBuildType()
XPC_OptForHost()
XPC_SetCompileDefinitions()


#dependencies
find_package(StdPlusPlus REQUIRED)

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
target_link_libraries(ACGR Std++)
target_link_libraries(demo Std++ Std++Static ACGR)

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