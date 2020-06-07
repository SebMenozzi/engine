.PHONY : clean
.SUFFIXES :

NAME = engine
SRC =	src/main.cpp src/scene/scene.cpp src/shader/shader.cpp \
			src/clock/clock.cpp src/input/input.cpp src/camera/camera.cpp \
			src/utils/fileUtils.cpp src/utils/imageUtils.cpp \
			src/models/trash/lightCube.cpp \
			src/texture/texture.cpp src/cubemap/cubemap.cpp \
			src/world/chunk.cpp src/world/heightmap.cpp \
			src/models/mesh.cpp src/models/texturedMesh.cpp \
			src/models/cube/cube.cpp src/models/plane/plane.cpp \
			src/models/cube/box.cpp src/models/plane/floor.cpp \
			src/models/sphere/uvSphere.cpp \
			src/models/skybox/skybox.cpp

CC ?= g++

OBJ = $(SRC:.cpp=.o)

OS = $(shell uname)

ifeq ($(OS), Darwin) # if the OS is MacOS
	LFLAGS = -framework OpenGL -F/Library/Frameworks -framework SDL2 -framework SDL2_image -framework SDL2_TTF
  IFLAGS = -I/Library/Frameworks/SDL2.framework/Headers/ -I/Library/Frameworks/SDL2_image.framework/Headers/ -I/Library/Frameworks/SDL2_TTF.framework/Headers/
else
  LFLAGS = -lGL -lGLU -lSDL2 -lSDL2_image -lSDL2_ttf
endif

CFLAGS = -w -std=c++11 -lc++

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(LFLAGS) -o $@

%.o : %.cpp
	$(CC) -c $^ $(CFLAGS) $(IFLAGS) -o $@

clean :
	rm -f $(OBJ) $(NAME)
