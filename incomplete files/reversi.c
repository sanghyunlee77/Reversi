#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "server.h"
#include "client.h"
#include "game.h"


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <mode> [arguments]\n", argv[0]);
        printf("Available modes:\n");
        printf("  -server <port>    Start the reversi server\n");
        printf("  -client <ip> <port>  Connect to a reversi server\n");
        return 1;
    }

    if (strcmp(argv[1], "-server") == 0) {
        if (argc != 3) {
            printf("Invalid number of arguments for server mode.\n");
            return 1;
        }
        int port = atoi(argv[2]);
        main_server(port);
    } else if (strcmp(argv[1], "-client") == 0) {
        if (argc != 4) {
            printf("Invalid number of arguments for client mode.\n");
            return 1;
        }
        const char* ipAddress = argv[2];
        int port = atoi(argv[3]);
         main_client(ipAddress, port);
    } else {
        printf("Invalid mode.\n");
        return 1;
    }


    return 0;
}