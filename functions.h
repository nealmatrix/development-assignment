#ifndef FUNCTION_H
#define FUNCTION_H

#include "property_generated.h"
#include <string>

extern int buffer_size;

uint8_t * create_property(std::string name, float value, std::string type);
void read_property(uint8_t *buffer_pointer);
void update_property(uint8_t *buffer_pointer, float new_value);
void stop_server(int p);
void start_server();
void start_client(uint8_t * buffer_pointer, int buffer_size);

#endif