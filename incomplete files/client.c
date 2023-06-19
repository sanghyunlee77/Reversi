#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include "game.h"
#include "client.h"

#define BOARD_SIZE 8
#define MAX_BUFFER_SIZE (BOARD_SIZE * BOARD_SIZE + 1)


int connect_ipaddr_port(const char *ip, int port)
{
        int sock_fd ;
        sock_fd = socket(AF_INET, SOCK_STREAM, 0) ;
        if (sock_fd <= 0) {
                perror("socket failed: ");
                exit(EXIT_FAILURE);
        }
        int opt = 2 ;
        if (setsockopt(sock_fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt)) != 0) {
                fprintf(stderr, "fail at setsockopt\n") ;
                exit(EXIT_FAILURE) ;
        }

        struct sockaddr_in address ;
        memset(&address, 0, sizeof(address)) ;
        address.sin_family = AF_INET ;
        address.sin_port = htons(port) ;
        if (inet_pton(AF_INET, ip, &(address.sin_addr)) <= 0) {
                perror("inet_pton failed: ") ;
                exit(EXIT_FAILURE) ;
        }

        if (connect(sock_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
                perror("connect failed: ") ;
                exit(EXIT_FAILURE) ;
        }
        return sock_fd ;
}

void game(int conn_fd)
{
        char buf[256] ;

        do {
                fgets(buf, 256, stdin) ;
                buf[strlen(buf) - 1] = '\0' ;
                if (strcmp(buf, "quit()") == 0)
                        break ;

                send(conn_fd, buf, strlen(buf), 0) ;

                int s ;
                while ((s = recv(conn_fd, buf, 1024, 0)) == 0) ;
                if (s == -1)
                        break ;
                buf[s] = '\0' ;

                printf(">%s\n", buf) ;
        } while (strcmp(buf, "quit()") != 0) ;
}

void receiveBoardData(int clientSocket, char* boardData) {
  int dataSize = recv(clientSocket, boardData, MAX_BUFFER_SIZE - 1, 0);
  if (dataSize == -1) {
    perror("Failed to receive board data");
    // Handle error
  } else if (dataSize == 0) {
    // Connection closed by client
  } else {
    boardData[dataSize] = '\0'; // Null-terminate the received data
  }
}

int main_client(const char *ip, int port)
{
        int s, len ;
        char buffer[1024] = {0};
        char *data ;
/*
        if (argc != 3) {
                fprintf(stderr, "Wrong number of argument\n") ;
                fprintf(stderr, "Usage: ./chat-sendfirst [IP addr] [Port num]\n") ;
                exit(EXIT_FAILURE) ;
        }
*/
        int conn_fd = connect_ipaddr_port(ip, port) ;

        char boardData[MAX_BUFFER_SIZE];

        receiveBoardData(conn_fd, boardData);

        game(conn_fd) ;

        shutdown(conn_fd, SHUT_RDWR) ;

        return EXIT_SUCCESS ;
}