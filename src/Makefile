CC = gcc
CFLAGS =

PROG1 = crear
PROG2 = combine


SRC1 = crear.c
SRC2 = combine.c


OBJ1 = $(SRC1:.c=.o)
OBJ2 = $(SRC2:.c=.o)

FOLDER = build
ZIP_NAME = ../test/ssoo_p1_100522110_100522240_100522257.zip

all: $(PROG1) $(PROG2) bundlezip

$(PROG1): $(OBJ1)
	$(CC) $(CFLAGS) -o ./$(FOLDER)/$@ ./$(FOLDER)/$^

$(PROG2): $(OBJ2)
	$(CC) $(CFLAGS) -o ./$(FOLDER)/$@ ./$(FOLDER)/$^


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o ./$(FOLDER)/$@

clean:
	rm -f ./$(FOLDER)/$(PROG1) ./$(FOLDER)/$(PROG2) ./$(FOLDER)/$(OBJ1) ./$(FOLDER)/$(OBJ2)

distclean: clean
	rm -rf ./$(FOLDER)/*
	rm $(ZIP_NAME)

bundlezip:
	mv Makefile ./build
	mv makefilecp Makefile
	zip $(ZIP_NAME) combine.c crear.c Makefile authors.txt
	mv Makefile makefilecp
	mv ./build/Makefile ./Makefile