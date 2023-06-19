reversi: server.o game.o client.o reversi.o
	gcc -o reversi server.o game.o client.o reversi.o

server.o: server.c server.h game.h
	gcc -c -o server.o server.c

game.o: game.c game.h
	gcc -c -o game.o game.c

client.o: client.c client.h game.h
	gcc -c -o client.o client.c

reversi.o: reversi.c server.h game.h client.h
	gcc -c -o reversi.o reversi.c

clean:
	rm -f reversi *.o