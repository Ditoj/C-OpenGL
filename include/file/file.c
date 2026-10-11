#include "file/file.h"

#include <stdio.h>
#include <stdlib.h>

#define EXIT_FILE_OPEN_FAIL  1
#define EXIT_FILE_READ_FAIL  2
#define EXIT_ALLOCATION_FAIL 3

char* readFile(const char* path) {
	FILE* file = fopen(path, "rb");
	if(file == NULL) {
		fprintf(stderr, "%s", "Failed to open file \"%s\".", path);
		exit(EXIT_FILE_OPEN_FAIL);
	}

	fseek(file, 0L, SEEK_END);
	size_t fileSize = ftell(file);
	rewind(file);

	char* dest = malloc(fileSize + 1);
	if(dest == NULL) {
		fprintf(stderr, "%s", "Not enough memory to read file \"%s\".", path);
		exit(EXIT_ALLOCATION_FAIL);
	}

	size_t bytesRead = fread(dest, sizeof(char), fileSize, file);
	if(bytesRead != fileSize) {
		fprintf(stderr, "%s", "Failed to read file \"%s\".", path);
		exit(EXIT_FILE_READ_FAIL);
	}

	dest[bytesRead] = '\0';

	fclose(file);

	return dest;
}

void getFileExtension(const char* path, char* dest) {
	int length = 0;
	for(int i = 0; path[i] != '\0'; ++ i) {
		if(path[i] == '.') {
			for(int j = i + 1; path[j] != '\0'; ++ j) {
				if(length > 254) break;
				dest[length ++] = path[j];
			}

			break;
		}
	}

	dest[length] = '\0';
}