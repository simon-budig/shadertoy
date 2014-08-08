shadertoy: shadertoy.c
	gcc -Wall -lglut -lGLEW -lGL -lm -o $@ $<
