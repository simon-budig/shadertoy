shadertoy: shadertoy.c
	gcc -Wall -Wextra -lglut -lGLEW -lGL -lm -o $@ $<
