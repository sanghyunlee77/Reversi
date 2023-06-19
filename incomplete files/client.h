#ifndef CLIENT_H
#define CLIENT_H

// 함수 선언
int main_client(const char *ip, int port);
void receiveBoardData(int clientSocket, char* boardData);
void game(int conn_fd);
int connect_ipaddr_port(const char *ip, int port);

#endif // CLIENT_H