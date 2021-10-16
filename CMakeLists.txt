cmake_minimum_required (VERSION 3.0)
project (GuiCode2)

set (CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

add_subdirectory(libs)

option(GUICODE2_BUILD_EXAMPLE "Build Example" OFF)

set(SRCGUI "${GuiCode2_SOURCE_DIR}/")

file(GLOB_RECURSE SOURCE
  "${SRCGUI}source/*.cpp"
  "${SRCGUI}include/*.hpp"
  "${SRCGUI}shaders/*.shader"
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_library(GLAD STATIC
  libs/glad/glad.c
)

include_directories(
  libs/
  include/
)

add_library(GuiCode2
  ${SOURCE}
)

source_group(TREE ${SRCGUI} FILES ${SOURCE})

target_precompile_headers(GuiCode2 PUBLIC
  "${SRCGUI}include/GuiCode2/pch.hpp"
)

target_link_libraries(GuiCode2
  ${OPENGL_LIBRARY}
  ${GLFW_LIBRARIES}
  glfw
  GLAD
  freetype
  comctl32
  propsys
  dwmapi
  gdiplus
  user32
)

if (GUICODE2_BUILD_EXAMPLE)
file(GLOB_RECURSE EXAMPLE_SOURCE
  "${SRCGUI}example/*.cpp"
)

add_executable(GuiCode2Example
  ${EXAMPLE_SOURCE}
)

target_link_libraries(GuiCode2Example
  GuiCode2
)
endif()