TARGET	= rubik.out
INC 	= ./include/
SRCDIR	= ./src/
SRC		= main.cpp ACube.cpp FaceletCube.cpp cubie.cpp \
			CubieCube.cpp MoveTables.cpp Solver.cpp math_utils.cpp \
			PruningTables.cpp BaseGenerator.cpp
OBJDIR	= ./objs/
OBJS	= ${addprefix ${OBJDIR}, ${SRC:.cpp=.o}}
CFLAGS	= -Wall -Wextra -Werror --std=c++14 -g
LFLAGS	= -Ldeps -lglfw3 -limgui -lGLEW -lGLU -lGL -lpthread -ldl
CC		= g++

${OBJDIR}%.o:	${SRCDIR}%.cpp
	${CC} ${CFLAGS} -c $< -o $@

${TARGET}:		${OBJDIR} ${OBJS} ${INC}
	${CC} ${CFLAGS} ${OBJS} -I ${INC} -o $@ ${LFLAGS}

${OBJDIR}:
	mkdir -p ${OBJDIR}

all:			${TARGET}

clean:
	rm -rf ${OBJDIR}

fclean:			clean
	rm -rf ${TARGET}
	rm -rf ./tables

re:				fclean all

.PHONY:			all clean fclean re