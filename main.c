#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void handleDirectory(char* path);
int isDirectory(char* path);
int isFile(char* path);
char* strjoin(char* a, char* b);
int strends(char* target, char* test);

int main(int argc, char* argv[]) {
    handleDirectory(".");
    return 0;
}

void handleDirectory(char* path) {
	struct dirent *files;
	DIR* dir = opendir(path);

	if(dir == NULL)
		return;

	while((files = readdir(dir)) != NULL) {
		if(files->d_name[0] ==  '.')
			continue;	
	
		char* filePathPart = strjoin(path, "/");
		char* filePath = strjoin(filePathPart, files->d_name);
		free(filePathPart);
		
		if(isDirectory(filePath))
			handleDirectory(filePath);
		else if(strends(filePath, ".java")){
			printf("<<>> Compiling %s\n", filePath);
			
			char* command = strjoin("javac ", filePath);			
			int feedback = system(command);
			free(command);

			if(feedback != 0) {
				printf("<<>> Bad compiler feedback: %i\n", feedback);
				free(filePath);
				break;
			}
		}

		free(filePath);
	}

	closedir(dir);
}

int strends(char* target, char* test) {
	int targetLength = strlen(target);
	int testLength = strlen(test);

	if(targetLength < testLength)
		return 0;

	for(int index = 0; index < testLength; index++) {
		int shift = index + 1;
			
		if(test[testLength - shift] != target[targetLength - shift])
			return 0;
	}

	return 1;
}

char* strjoin(char* a, char* b) {
	int lengthA = strlen(a);
	int lengthB = strlen(b);
	int length = lengthA + lengthB + 1;
	char* joined = (char*) malloc(length * sizeof(char));
	
	for(int index = 0; index < lengthA; index++)
		joined[index] = a[index];

	for(int index = 0; index < lengthB; index++)
		joined[lengthA + index] = b[index];

	joined[length - 1] = '\0';
	return joined;
}

int isDirectory(char* path) {
	DIR* dir = opendir(path);
	
	if(dir == NULL)
		return 0;

	closedir(dir);
	return 1;
}

int isFile(char* path) {
	FILE *file = fopen(path, "r");
	
	if(file == NULL)
		return 0;

	fclose(file);
	return 1;
}
