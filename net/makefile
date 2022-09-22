.PHONY : server client clean

server : 
	gcc server.c message.c msg_parser.c tcp_server.c tcp_client.c -o server.o

client :
	gcc client.c message.c msg_parser.c tcp_server.c tcp_client.c -o client.o

clean : 
	rm *.o