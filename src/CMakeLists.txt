set(SOURCE_FILES
	${SOURCE_FILES}

	#Cameras
	${CMAKE_CURRENT_SOURCE_DIR}/Cameras/FreeFlyCamera.cpp

	#Device
	${CMAKE_CURRENT_SOURCE_DIR}/Device/Passes/ShadowPass.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Device/Passes/ShadowPass.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Device/DeviceRenderer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Device/DeviceRenderer.hpp

	#Internal
	${CMAKE_CURRENT_SOURCE_DIR}/Internal/ShaderCompiler.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Internal/ShaderCompiler.hpp

	#Meshes
	${CMAKE_CURRENT_SOURCE_DIR}/Meshes/Icosahedron.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Meshes/IcoSphere.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Meshes/Plane.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Meshes/Triangle.cpp

	#RayTracer
	${CMAKE_CURRENT_SOURCE_DIR}/RayTracer/RayTraceRenderer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RayTracer/RayTraceRenderer.hpp

	#.
	${CMAKE_CURRENT_SOURCE_DIR}/Camera.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Entity.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Mesh.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Renderer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SceneManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SceneNode.cpp

	PARENT_SCOPE)