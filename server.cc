#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <iostream>

#define IP_ADDRESS "127.0.0.1"
#define DEFAULT_PORT 1234
#define MAXLINK 2048
#define BUFFSIZE 2048

int server_sock;

void stop_server(int p){
    close(server_sock);
    std::cout << "close server" << std::endl;
    exit(0);
}

int main(){
    int server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    serv_addr.sin_port = htons(DEFAULT_PORT);
    bind(server_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    listen(server_sock, MAXLINK);

    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    char buffer[BUFFSIZE];

    while(true){
        signal(SIGINT, stop_server);
        int client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size);

        bzero(buffer, BUFFSIZE);
        read(client_sock, buffer, BUFFSIZE - 1);
        std::cout << buffer << std::endl;

        close(client_sock);
    }

    return 0;    
}