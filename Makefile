TARGET	= Rubik.out

CC		= g++
CFLAGS	= -Wall -Wextra -Werror --std=c++14

INC 	= -I ./include -I ./deps
SRCDIR	= ./src
SRC		= main.cpp ACube.cpp FaceletCube.cpp cubie.cpp \
			CubieCube.cpp MoveTables.cpp Solver.cpp math_utils.cpp \
			PruningTables.cpp BaseGenerator.cpp \
			\
			Renderer.cpp Shader.cpp \
			VertexArray.cpp VertexBuffer.cpp VertexBufferLayout.cpp IndexBuffer.cpp Texture.cpp \
			CubeModel.cpp cubemodel_utils.cpp Menu.cpp

OBJDIR	= ./objs/
OBJS	= ${addprefix ${OBJDIR}, ${SRC:.cpp=.o}}

IMGUI_B	= deps/imgui/
IMGUI	= ${IMGUI_B}/libimgui.a

DEPS	= -L${IMGUI_B} -limgui -lglfw -lGLEW -lGL -lpthread -ldl -lX11

# Compile sources
${OBJDIR}%.o: ${SRCDIR}/%.cpp
	${CC} ${CFLAGS} ${INC} -c $< -o $@

# Link project
${TARGET}: ${OBJDIR} ${OBJS} ${GLFW} ${GLEW} ${IMGUI}
	${CC} ${CFLAGS} ${OBJS} -o $@ ${DEPS}

# Create object directories
${OBJDIR}:
	mkdir -p ${OBJDIR}

# Compile Dear ImGui from sources
${IMGUI}:
	${MAKE} -C ${IMGUI_B}

# Usual rules
all: ${TARGET}

clean:
	rm -rf ${OBJDIR}
	${MAKE} clean -C ${IMGUI_B}

fclean: clean
	rm -rf ${TARGET}
	${MAKE} fclean -C ${IMGUI_B}

re: fclean all

.PHONY: all clean fclean re
