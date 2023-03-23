#include "functions.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

#define IP_ADDRESS "127.0.0.1"
#define DEFAULT_PORT 1234
#define MAXLINK 2048
#define BUFFSIZE 2048

int server_sock;
int buffer_size;

uint8_t * create_property(std::string name, float value, std::string type){
    flatbuffers::FlatBufferBuilder builder(1024);

    std::vector<flatbuffers::Offset<MySample::Property>> sub_properties_vector;
    auto sub_properties = builder.CreateVector(sub_properties_vector);
    auto property_name = builder.CreateString(name);
    auto property_type = builder.CreateString(type);

    MySample::PropertyBuilder property_builder(builder);
    property_builder.add_name(property_name);
    property_builder.add_value(value);
    property_builder.add_type(property_type);
    property_builder.add_sub_properties(sub_properties);

    auto property = property_builder.Finish();
    builder.Finish(property);
    buffer_size = builder.GetSize();
    return builder.GetBufferPointer();
}

void read_property(uint8_t *buffer_pointer){
    auto property = MySample::GetProperty(buffer_pointer);

    std::string name = property->name()->str();
    float value = property->value();
    std::string type = property->type()->str();

    std::cout << "property_name: " << name << std::endl;
    std::cout << "property_value: " << value << std::endl;
    std::cout << "property_type: " << type << std::endl;

    for (auto sub_property: *(property->sub_properties())){
        std::string sub_property_name = sub_property->name()->str();
        float sub_property_value = sub_property->value();
        std::string sub_property_type = sub_property->type()->str();

        std::cout << "sub_property_name: " << sub_property_name << std::endl;
        std::cout << "sub_property_value: " << sub_property_value << std::endl;
        std::cout << "sub_property_type: " << sub_property_type << std::endl;
    }
}

void update_property(uint8_t *buffer_pointer, float new_value){
    auto property = MySample::GetMutableProperty(buffer_pointer);
    property->mutate_value(new_value);
}

void stop_server(int p){
    close(server_sock);
    std::cout << "close server" << std::endl;
    exit(0);
}

void start_server(){
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
    uint8_t buffer[BUFFSIZE];

    while(true){
        signal(SIGINT, stop_server);
        int client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size);

        bzero(buffer, BUFFSIZE);
        read(client_sock, buffer, BUFFSIZE - 1);
        // std::cout << buffer << std::endl;
        read_property(buffer);

        close(client_sock);
    }
}

void start_client(uint8_t * buffer_pointer, int buffer_size){
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    serv_addr.sin_port = htons(DEFAULT_PORT);
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // char buffer[BUFFSIZE];
    // char str[] = "test socket";
    // write(sock, str, sizeof(str));
    write(sock, buffer_pointer, buffer_size);

    close(sock);
}