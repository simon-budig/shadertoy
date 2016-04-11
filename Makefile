OS := $(shell uname -s)
ifeq ($(OS),Darwin)
  GL_LIBS=-framework OpenGL -framework GLUT
else
  GL_LIBS=-lglut -lGL
endif

CFLAGS = -Wall -Wextra -g

shadertoy: shadertoy.c
	$(CC) $(CFLAGS) -o $@ $< `pkg-config --libs --cflags gdk-pixbuf-2.0 glew` $(GL_LIBS) -lm

clean:
	-rm -rf shadertoy shadertoy.dSYM
