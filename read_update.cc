// #include "../open_sources/flatbuffers/include/flatbuffers/"
#include "property_generated.h"
#include <string>
#include <vector>
#include <iostream>

uint8_t *buffer_pointer;

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

int main(){
    uint8_t *buffer_pointer = create_property("my_proptery", 1.23, "my_type");
    read_property(buffer_pointer);
    std::cout << "after change:" << std::endl;
    update_property(buffer_pointer, 10);
    read_property(buffer_pointer);
}