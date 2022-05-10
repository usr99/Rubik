TARGET	= Rubik

CC		= g++
CFLAGS	= -Wall -Wextra -Werror --std=c++14
LINK	= -lglfw3 -limgui -lGLEW -lGLU -lGL -lpthread -ldl

INC 	= `find ./include -type d | sed s/^/-I/`
SRCDIR	= ./src/
SRC		= main.cpp ACube.cpp FaceletCube.cpp cubie.cpp \
			CubieCube.cpp MoveTables.cpp Solver.cpp math_utils.cpp \
			PruningTables.cpp BaseGenerator.cpp \
			\
			Renderer.cpp Camera.cpp Shader.cpp \
			VertexArray.cpp VertexBuffer.cpp VertexBufferLayout.cpp IndexBuffer.cpp

OBJDIR	= ./objs/
OBJS	= ${addprefix ${OBJDIR}, ${SRC:.cpp=.o}}

${OBJDIR}%.o: ${SRCDIR}/%.cpp
	${CC} ${CFLAGS} ${INC} -c $< -o $@

${OBJDIR}%.o: ${SRCDIR}/Renderer/%.cpp
	${CC} ${CFLAGS} ${INC} -c $< -o $@

${TARGET}: ${OBJDIR} ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o $@ -Ldeps ${LINK}

${OBJDIR}:
	mkdir -p ${OBJDIR}

all: ${TARGET}

clean:
	rm -rf ${OBJDIR}

fclean: clean
	rm -rf ${TARGET}

re: fclean all

.PHONY: all clean fclean re
