set(SOURCE_FILES
	${SOURCE_FILES}

	#Device
	${CMAKE_CURRENT_SOURCE_DIR}/Device/Passes/ShadowPass.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Device/Passes/ShadowPass.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Device/DeviceRenderer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Device/DeviceRenderer.hpp

	#Internal
	${CMAKE_CURRENT_SOURCE_DIR}/Internal/Parse.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Internal/ShaderCompiler.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Internal/ShaderCompiler.hpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/Camera.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Renderer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SceneManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SceneNode.cpp

	PARENT_SCOPE)