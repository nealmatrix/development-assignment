#include "functions.h"
#include <iostream>

int main(){
    uint8_t *buffer_pointer = create_property("my_proptery", 1.23, "my_type");
    std::cout << "buffer size: " << buffer_size << std::endl;
    read_property(buffer_pointer);
    std::cout << "after change:" << std::endl;
    update_property(buffer_pointer, 10);
    read_property(buffer_pointer);
}