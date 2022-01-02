# SDL2

set(SDL2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL)
add_subdirectory(${SDL2_DIR})

# SDL2 Image

set(SDL2_IMAGE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL_image)
add_subdirectory(${SDL2_IMAGE_DIR})

# Json

set(JSON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/json)
add_subdirectory(${JSON_DIR})

# TinyGLTF

set(TINYGLTF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tinygltf)
add_subdirectory(${TINYGLTF_DIR})

set(TINYGLTF_HEADER_ONLY ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL OFF CACHE INTERNAL "" FORCE)

# Assimp

set(ASSIMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assimp)
add_subdirectory(${ASSIMP_DIR})

# IMGUI

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl.cpp
)

# GLM

set(GLM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glm)

# STATIC LIBRARIES

add_library(STATIC_LIBRARIES STATIC)

target_sources(STATIC_LIBRARIES PRIVATE ${IMGUI_SOURCES})
target_include_directories(STATIC_LIBRARIES PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${SDL2_DIR}/include
    ${SDL2_IMAGE_DIR}
    ${JSON_DIR}/include
    ${TINYGLTF_DIR}
    ${ASSIMP_DIR}/include
    ${GLM_DIR}/glm
    ${GLM_DIR}/glm/gtx
    ${GLM_DIR}/glm/gtc
    ${GLM_DIR}/glm/core
)

find_package(OpenGL REQUIRED)
target_link_libraries(STATIC_LIBRARIES PUBLIC ${OPENGL_LIBRARIES})
target_link_libraries(STATIC_LIBRARIES PUBLIC SDL2main SDL2 SDL2_image nlohmann_json tinygltf assimp glm ${CMAKE_DL_LIBS})