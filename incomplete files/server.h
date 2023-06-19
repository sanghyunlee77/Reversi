#ifndef SERVER_H
#define SERVER_H

// 함수 선언
int main_server(int port_num);
void game(int conn_fd);
void sendBoardData(int clientSocket, const char* boardData);
int listen_at_port(int portnum);

#endif // SERVER_H
