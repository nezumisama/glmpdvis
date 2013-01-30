CC      =   gcc
CFLAGS  =   -O2 -march=native -mtune=native --std=c99 -pedantic -Wall
LDFLAGS =
LIBS    =   -lGL -lGLU -lglfw -lGLEW -lm -lfftw3
SRC     =   glmpdvis.c config_parser.c cmd_arg.c 
OBJ     =   $(SRC:.c=.o)

all: glmpdvis

glmpdvis: $(OBJ)
	@echo "CC -o $@"
	@$(CC) $(LDFLAGS) $(OBJ) $(LIBS) -o $@ 

.c.o:
	@echo "CC -o $@"
	@$(CC) $(CFLAGS) -c $< -o $@

parser_test: config_parser.o cmd_arg.o parser_test.o
	@echo "CC -o $@"
	@$(CC) $(CFLAGS) config_parser.o cmd_arg.o $@.o -o $@
	
cmd_arg_test: cmd_arg_test.o cmd_arg.o
	@echo "CC -o $@"
	@$(CC) $(CFLAGS) cmd_arg.o $@.o -o $@

clean:
	rm -f $(OBJ) glmpdvis parser_test cmd_arg_test parser_test.o cmd_arg_test.o
