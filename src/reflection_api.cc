#include "functions.h"
#include "flatbuffers/reflection.h"
#include "flatbuffers/util.h"
#include <iostream>

int main(){
    // uint8_t *buffer_pointer = create_property("my_proptery", 1.23, "my_type");
    // std::cout << "buffer size: " << buffer_size << std::endl;

    std::string bfbsfile;
    flatbuffers::LoadFile("property.bfbs", true, &bfbsfile);
    const reflection::Schema &schema = *reflection::GetSchema(bfbsfile.c_str());

    auto types = schema.objects();
    for (size_t i = 0; i < types->Length(); i++){
        const reflection::Object *type = types->Get(i);
        std::cout << type->name()->c_str();
    }
    
    return 0;
}