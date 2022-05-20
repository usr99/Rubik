TARGET	= Rubik.out

CC		= g++
CFLAGS	= -Wall -Wextra -Werror --std=c++14 -g
LINK	= -lglfw3 -limgui -lGLEW -lGLU -lGL -lpthread -ldl

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

GLFW_B	= deps/GLFW/build
GLFW	= deps/GLFW/build/src/libglfw3.a

# Compile sources
${OBJDIR}%.o: ${SRCDIR}/%.cpp
	${CC} ${CFLAGS} ${INC} -c $< -o $@

# Link project
${TARGET}: ${OBJDIR} ${OBJS} ${GLFW}
	${CC} ${CFLAGS} ${OBJS} -o $@ -Ldeps ${LINK}

# Create object directories
${OBJDIR}:
	mkdir -p ${OBJDIR}

# Compile GLFW from sources
${GLFW}: ${GLFW_B}
	${MAKE} -C ${GLFW_B}
${GLFW_B}:
	mkdir deps/GLFW/build
	cmake -B deps/GLFW/build -S deps/GLFW

# Usual rules
all: ${TARGET}

clean:
	rm -rf ${OBJDIR}
	${MAKE} -C clean ${GLFW_B}

fclean: clean
	rm -rf ${TARGET}

re: fclean all

.PHONY: all clean fclean re
