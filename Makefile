TARGET	= rubik.out
INC 	= ./include/
SRCDIR	= ./src/
SRC		= main.cpp
OBJDIR	= ./objs/
OBJS	= ${addprefix ${OBJDIR}, ${SRC:.cpp=.o}}
CFLAGS	= -Wall -Wextra -Werror --std=c++98
CC		= g++

${OBJDIR}%.o:	${SRCDIR}%.cpp
	${CC} ${CFLAGS} -c $< -o $@

${TARGET}:		${OBJDIR} ${OBJS} ${INC}
	${CC} ${CFLAGS} ${OBJS} -I ${INC} -o $@

${OBJDIR}:
	mkdir -p ${OBJDIR}

all:			${TARGET}

clean:
	rm -rf ${OBJDIR}

fclean:			clean
	rm -rf ${TARGET}

re:				fclean all

.PHONY:			all clean fclean re