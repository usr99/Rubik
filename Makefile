TARGET	= rubik

CC		= g++
CFLAGS	= -Wall -Wextra -Werror --std=c++14

IMGUIDIR= imgui
IMGUI   = ${IMGUIDIR}/libimgui.a

INC 	= -I ./include -I ./${IMGUIDIR}/include
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

LIBS	= -lglfw -lGLEW -lGL -L ${IMGUIDIR} -limgui

# Compile sources
${OBJDIR}%.o: ${SRCDIR}/%.cpp
	${CC} ${CFLAGS} ${INC} -c $< -o $@

# Link project
${TARGET}: ${OBJDIR} ${OBJS} ${IMGUI}
	${CC} ${CFLAGS} ${OBJS} -o $@ ${LIBS}

# Create object directories
${OBJDIR}:
	mkdir -p ${OBJDIR}

# Compile Dear ImGui from sources
${IMGUI}:
	${MAKE} -C ${IMGUIDIR}

# Usual rules
all: ${TARGET}

clean:
	rm -rf ${OBJDIR}

fclean: clean
	rm -rf ${TARGET}
	${MAKE} fclean -C ${IMGUIDIR}

re: fclean all

.PHONY: all clean fclean re
