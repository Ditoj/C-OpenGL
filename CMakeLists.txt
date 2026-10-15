cmake_minimum_required (VERSION 3.13)

set(CMAKE_C_COMPILER "C:/msys64/ucrt64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/msys64/ucrt64/bin/g++.exe")

project(world LANGUAGES C)

find_package(OpenGL REQUIRED)

set(GLFW_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/libglfw3dll.a")

set(MY_SOURCE_FILES
	"${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/main.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/renderer/renderer.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/shader/shader.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/buffers/buffer_base.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/buffers/vertex_buffer.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/buffers/index_buffer.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/vertex_array/vertex_array.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/texture/texture.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/file/file.c"
)

add_executable(world_build ${MY_SOURCE_FILES})
add_executable(world_debug ${MY_SOURCE_FILES})

foreach(target world_build world_debug)
	target_include_directories(${target} PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}/include"
	)

	target_link_libraries(${target} PRIVATE
		OpenGL::GL
		"${GLFW_LIBRARY}"
	)
endforeach()

set_target_properties(world_build PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/build"
)

set_target_properties(world_debug PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/debug"
)