default: client server client_display

client: client.o networking.o
	gcc -o client client.o networking.o

server: server.o networking.o
	gcc -o server server.o networking.o

client.o: client.c
	gcc -c client.c

server.o: server.c
	gcc -c server.c

networking.o: networking.c networking.h
	gcc -c networking.c

client_display: client_display.o
	gcc -o client_display client_display.o

client_display.o:
	gcc -c client_display.c

clean:
	rm *.o
	rm *~
