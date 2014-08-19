shadertoy: shadertoy.c
	gcc -Wall -g `pkg-config --libs --cflags gdk-pixbuf-2.0` -lglut -lGLEW -lGL -lm -o $@ $<
