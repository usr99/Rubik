TARGET	= libimgui.a

CC		= g++
CFLAGS	= -Wall -Wextra -Werror --std=c++14

OBJS	= imgui_demo.o imgui_draw.o imgui_tables.o imgui_widgets.o \
			imgui.o imgui_impl_glfw.o imgui_impl_opengl3.o

%.o: %.cpp
	${CC} ${CFLAGS} -I ../GLFW/include -c $< -o $@

${TARGET}: ${OBJS}
	ar -rcs ${TARGET} ${OBJS}

all: ${TARGET}

clean:
	rm -rf ${OBJS}

fclean: clean
	rm -rf ${TARGET}

.PHONY: all clean
