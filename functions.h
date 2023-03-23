#include "property_generated.h"
#include <string>

uint8_t * create_property(std::string name, float value, std::string type);
void read_property(uint8_t *buffer_pointer);
void update_property(uint8_t *buffer_pointer, float new_value);