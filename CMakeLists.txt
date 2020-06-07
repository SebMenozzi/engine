# OPTIMISATION FLAGS :
set(OPT_FLAGS, "-Ofast")
# END OPTIMIZATION FLAGS

# SOURCES :
# Put all your sources here EXCEPT YOUR MAIN.CPP
set(SRC_ENGINE
  src/camera/camera.cpp
  src/clock/clock.cpp
  src/cubemap/cubemap.cpp
  src/input/input.cpp
  src/models/cube/cube.cpp
  src/models/cube/box.cpp
  src/models/plane/plane.cpp
  src/models/plane/floor.cpp
  src/models/skybox/skybox.cpp
  src/models/sphere/uvSphere.cpp
  src/models/trash/lightCube.cpp
  src/models/mesh.cpp
  src/models/texturedMesh.cpp
  src/scene/scene.cpp
  src/shader/shader.cpp
  src/texture/texture.cpp
  src/utils/fileUtils.cpp
  src/utils/imageUtils.cpp
  src/world/chunk.cpp
  src/world/heightmap.cpp
)

# Directories containing headers must be specified in the include_directories()
include_directories(src)
# END SOURCES

# SETUP
cmake_minimum_required(VERSION 3.12)
project(Engine)

# BUILD OPTIONS
set(CMAKE_CXX_STANDARD 17)

# -Wall -Werror -Wextra -pedantic
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OPT_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3 -fsanitize=address")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# https://github.com/tcbrindle/sdl2-cmake-scripts
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIR}
                    ${SDL2_IMAGE_INCLUDE_DIR}
                    ${SDL2_TTF_INCLUDE_DIR})

#if(NOT WIN32)
find_package(OpenGL REQUIRED)
#endif()

find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

# ENGINE
add_executable(engine)
target_sources(engine PRIVATE "src/main.cpp" ${SRC_ENGINE})
target_link_libraries(engine PRIVATE ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${OPENGL_LIBRARIES} ${GLM_LIBRARIES})
