TARGET	= Rubik.out

CC		= g++
CFLAGS	= -Wall -Wextra -Werror --std=c++14

INC 	= -I ./include				\
			-I ./deps				\
			-I ./deps/GLFW/include	\
			-I ./deps/GLEW/include
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

GLFW_B	= deps/GLFW/build
GLFW	= ${GLFW_B}/src/libglfw3.a

GLEW_B	= deps/GLEW
GLEW	= ${GLEW_B}/lib/libGLEW.a

IMGUI_B	= deps/imgui/
IMGUI	= ${IMGUI_B}/libimgui.a

DEPS	= -L${GLFW_B}/src -lglfw3 -L${IMGUI_B} -limgui -L${GLEW_B}/lib -lGLEW -lGL -lpthread -ldl -lX11

# Compile sources
${OBJDIR}%.o: ${SRCDIR}/%.cpp
	${CC} ${CFLAGS} ${INC} -c $< -o $@

# Link project
${TARGET}: ${OBJDIR} ${OBJS} ${GLFW} ${GLEW} ${IMGUI}
	${CC} ${CFLAGS} ${OBJS} -o $@ ${DEPS}

# Create object directories
${OBJDIR}:
	mkdir -p ${OBJDIR}

# Compile GLFW from sources
${GLFW}: ${GLFW_B}
	${MAKE} -C ${GLFW_B}

${GLFW_B}:
	mkdir ${GLFW_B}
	cd ${GLFW_B} && cmake ..

# Compile GLEW from sources
# Then delete dynamic library to avoid conflict
${GLEW}:
	${MAKE} glew.lib -C ${GLEW_B}
	rm -rf ${GLEW_B}/lib/libGLEW.so

# Compile Dear ImGui from sources
${IMGUI}:
	${MAKE} -C ${IMGUI_B}

# Usual rules
all: ${TARGET}

clean:
	rm -rf ${OBJDIR}
	${MAKE} clean -C ${GLFW_B}
	${MAKE} clean -C ${GLEW_B}
	${MAKE} clean -C ${IMGUI_B}

fclean: clean
	rm -rf ${TARGET}
	rm -rf ${GLFW_B}
	${MAKE} fclean -C ${IMGUI_B}

re: fclean all

.PHONY: all clean fclean re
