CC=g++
FLAG=-Wall -Werror
OBJ=hotel.o main.o tools.o
BIN=HMS

all:$(OBJ)
	$(CC) $(OBJ) -o $(BIN) && ./$(BIN) room_info.txt

main.o:main.cpp hotel.cpp hotel.h tools.cpp tools.h
	$(CC) -c $(FLAG) main.cpp -o main.o

hotel.o:hotel.cpp hotel.h tools.cpp tools.h room.h guest.h
	$(CC) -c $(FLAG) hotel.cpp -o hotel.o

tools.o:tools.cpp tools.h
	$(CC) -c $(FLAG) tools.cpp -o tools.o

clean:
	rm -rf *~ *.h.gch
	rm -rf $(OBJ) $(BIN)
	


