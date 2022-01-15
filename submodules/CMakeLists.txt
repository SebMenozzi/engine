find_package(OpenGL REQUIRED)

# GLFW

set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glfw)
add_subdirectory(${GLFW_DIR})

add_library(GLFW_LIBRARY INTERFACE)
target_include_directories(GLFW_LIBRARY INTERFACE ${GLFW_DIR}/include)
target_link_libraries(GLFW_LIBRARY INTERFACE ${OPENGL_LIBRARIES} glfw)

# STB

set(STB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/stb)

add_library(STB_LIBRARY INTERFACE)
target_include_directories(STB_LIBRARY INTERFACE ${STB_DIR})

# Json

set(JSON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/json)
add_subdirectory(${JSON_DIR})

add_library(JSON_LIBRARY INTERFACE)
target_include_directories(JSON_LIBRARY INTERFACE ${JSON_DIR}/include)
target_link_libraries(JSON_LIBRARY INTERFACE nlohmann_json::nlohmann_json)

# TinyGLTF

set(TINYGLTF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tinygltf)
add_subdirectory(${TINYGLTF_DIR})

set(TINYGLTF_HEADER_ONLY ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL OFF CACHE INTERNAL "" FORCE)

add_library(TINYGLTF_LIBRARY INTERFACE)
target_include_directories(TINYGLTF_LIBRARY INTERFACE ${TINYGLTF_DIR})
target_link_libraries(TINYGLTF_LIBRARY INTERFACE tinygltf)

# Assimp

set(ASSIMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assimp)
set(ASSIMP_NO_EXPORT TRUE CACHE BOOL "x" FORCE)
set(ASSIMP_BUILD_TESTS FALSE CACHE BOOL "x" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT FALSE CACHE BOOL "x" FORCE)
set(ASSIMP_INSTALL_PDB FALSE CACHE BOOL "x" FORCE)
set(ASSIMP_BUILD_ZLIB FALSE CACHE BOOL "x" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS FALSE CACHE BOOL "x" FORCE)
set(ASSIMP_BUILD_COLLADA_IMPORTER TRUE CACHE BOOL "x" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER TRUE CACHE BOOL "x" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER TRUE CACHE BOOL "x" FORCE)
set(ASSIMP_BUILD_BLEND_IMPORTER TRUE CACHE BOOL "x" FORCE)
add_subdirectory(${ASSIMP_DIR})

add_library(ASSIMP_LIBRARY INTERFACE)
target_include_directories(ASSIMP_LIBRARY INTERFACE 
  ${ASSIMP_DIR}/include 
  ${CMAKE_BINARY_DIR}/submodules/assimp/include
)
target_link_libraries(ASSIMP_LIBRARY INTERFACE assimp)

# IMGUI

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set(IMGUI_SOURCES
  ${IMGUI_DIR}/imgui_demo.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
)

add_library(IMGUI_LIBRARY STATIC)
target_sources(IMGUI_LIBRARY PRIVATE ${IMGUI_SOURCES})
target_include_directories(IMGUI_LIBRARY PUBLIC
  ${IMGUI_DIR}
  ${IMGUI_DIR}/backends
)
target_link_libraries(IMGUI_LIBRARY PRIVATE GLFW_LIBRARY)

# GLM

set(GLM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glm)

add_library(GLM_LIBRARY INTERFACE)
target_include_directories(GLM_LIBRARY INTERFACE ${GLM_DIR}/glm)