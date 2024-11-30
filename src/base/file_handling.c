#include "file_handling.h"

bool file_loadstr(String* result, char* path) {
	char* buffer = 0;
	u32 length;

	FILE* file = fopen(path, "rb");
	if(!file) {
        return false;
    }

	fseek(file, 0, SEEK_END);
	length = ftell(file);
	fseek(file, 0, SEEK_SET);

	buffer = malloc(sizeof(char) * length + 1);
	if(!buffer) {
        return false;
    }

	fread(buffer, sizeof(char), length, file);
	
	fclose(file);
	
	buffer[length] = '\0';
    
    result->length = length;
    result->chars = buffer;

	return true;
}
