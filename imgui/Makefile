##################################################
#			GLOBAL VARIABLES DEFINITION			 #
##################################################

TARGET  = libimgui.a

CFLAGS	= -Wall -Wextra -Werror --std=c++11
CC		= g++

INC 	= -I ./include/imgui

SRC		= $(wildcard *.cpp)

OBJDIR	= ./objs/
OBJS	= ${addprefix ${OBJDIR}, ${SRC:.cpp=.o}}

##################################################
#				COMPILATION RULES				 #
##################################################

${OBJDIR}%.o: %.cpp
	${CC} ${CFLAGS} -c $< ${INC} -o $@

${TARGET}: ${OBJDIR} ${OBJS}
	ar -rcs $@ ${OBJS}

${OBJDIR}:
	mkdir -p ${OBJDIR}

##################################################
#  				   USUAL RULES					 #./
##################################################

all: ${TARGET}

clean:
	rm -rf ${OBJDIR}

fclean: clean
	rm -rf ${TARGET}

re: fclean all

.PHONY:	all clean fclean re
