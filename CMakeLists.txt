cmake_minimum_required(VERSION 3.13)
project(opengl_project LANGUAGES C)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SRC_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

set(MY_SOURCE_FILES
  "${SRC_FILES}"
)

add_executable(world_build ${MY_SOURCE_FILES})

foreach(target world_build)
  target_include_directories(world_build PRIVATE "include")

  target_link_libraries(world_build
    glfw
    OpenGL::GL
    $<$<PLATFORM_ID:Linux>:m>
  )
endforeach()
