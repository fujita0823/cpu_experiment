PRO = main
OBJ = subfiles/print.o subfiles/fpu.o subfiles/new_exec.o subfiles/init.o subfiles/read.o
CC = gcc
CFLAGS = -Wall -Ofast #-lm -mcmodel=large 

$(PRO): $(PRO).o $(OBJ)
	$(CC) -o $(PRO) $(PRO).o $(OBJ) -lm

$(OBJ) $(PRO).o: header.h

clean:
	rm -f *.o subfiles/*.o
