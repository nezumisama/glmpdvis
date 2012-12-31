all: glmpdvis

glmpdvis: src/glmpdvis.c
	gcc src/glmpdvis.c -o src/glmpdvis -O3 -march=native -mtune=native --std=c99 -pedantic -Wall -lGL -lGLU -lglfw -lGLEW -lm -lfftw3
