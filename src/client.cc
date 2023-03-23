#include "functions.h"
#include <iostream>

int main(){
    uint8_t *buffer_pointer = create_property("my_proptery", 1.23, "my_type");
    std::cout << "buffer size: " << buffer_size << std::endl;
    start_client(buffer_pointer, buffer_size);
    return 0;
}