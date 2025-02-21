CC = gcc
CFLAGS = -Wall -Werror -Wextra -O2

PROG1 = crear
PROG2 = combine


SRC1 = crear.c
SRC2 = combine.c


OBJ1 = $(SRC1:.c=.o)
OBJ2 = $(SRC2:.c=.o)

FOLDER = build

all: $(PROG1) $(PROG2) 

$(PROG1): $(OBJ1)
	$(CC) $(CFLAGS) -o ./$(FOLDER)/$@ ./$(FOLDER)/$^

$(PROG2): $(OBJ2)
	$(CC) $(CFLAGS) -o ./$(FOLDER)/$@ ./$(FOLDER)/$^


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o ./$(FOLDER)/$@

clean:
	rm -f ./$(FOLDER)/$(PROG1) ./$(FOLDER)/$(PROG2) ./$(FOLDER)/$(OBJ1) ./$(FOLDER)/$(OBJ2)

distclean: clean
	rm -f ./$(FOLDER)/*

bundlezip:
	zip ./$(FOLDER)/soo_p1_100522110_100522240_100522257.zip\
 		combine.c crear.c Makefile authors.txt