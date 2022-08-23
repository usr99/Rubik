TARGET	= Rubik.out

CC		= g++
CFLAGS	= -Wall -Wextra -Werror --std=c++14

BREW_PATH = /Users/${USER}/.brew/opt/
DEPENDENCIES = glfw glew
DEPS_FULL_PATH = ${addprefix ${BREW_PATH}, ${DEPENDENCIES}}

INC = ${addsuffix /include, ${addprefix -I, ${DEPS_FULL_PATH}}} -I include
LIBS = ${addsuffix /lib, ${addprefix -L, ${DEPS_FULL_PATH}}} -lGLEW -lglfw -Llibs -limgui -framework OpenGL

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

# Compile sources
${OBJDIR}%.o: ${SRCDIR}/%.cpp
	${CC} ${CFLAGS} ${INC} -c $< -o $@

# Link project
${TARGET}: ${OBJDIR} ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o $@ ${LIBS}

# Create object directories
${OBJDIR}:
	mkdir -p ${OBJDIR}

# Usual rules
all: ${TARGET}

clean:
	rm -rf ${OBJDIR}

fclean: clean
	rm -rf ${TARGET}

re: fclean all

.PHONY: all clean fclean re
