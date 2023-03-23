// #include <stdio.h>
#include <string.h>
// #include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define IP_ADDRESS "127.0.0.1"
#define DEFAULT_PORT 1234
#define MAXLINK 2048

int main(){
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    serv_addr.sin_port = htons(DEFAULT_PORT);
    bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    listen(serv_sock, MAXLINK);

    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_sock = accept(serv_sock, (struct sockaddr *)&client_addr, &client_addr_size);

    char str[] = "test socket";
    write(client_sock, str, sizeof(str));

    close(client_sock);
    close(serv_sock);

    return 0;    
}