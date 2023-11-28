#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#define DIR_MAX 20
#define FILE_MAX 100

int line_cnt = -1;

void recurvDirectory(const char *name){
	DIR *dir;
	struct dirent *directory;

	char temp[DIR_MAX][FILE_MAX];
	char *directory_path[DIR_MAX];
	int idx = 0, p_idx = 0;

	for(int i = 0; i < DIR_MAX; i++){
		directory_path[i] = NULL;
		for(int j = 0; j < FILE_MAX; j++)
			temp[i][j] = NULL;
	}

	if(!(dir = opendir(name))) return;

	printf("%s : \n", name);

	while((directory = readdir(dir)) != NULL) {
		if (directory->d_type == DT_DIR) {
			char path[1024];
			if(strcmp(directory->d_name, ".") == 0 || strcmp(directory->d_name, "..") == 0 || strcmp(directory->d_name, ".git") == 0) continue;

			snprintf(path, sizeof(path), "%s/%s", name, directory->d_name);
			for(int k = 0; k < strlen(path); k++)
				temp[idx][k] = path[k];

			directory_path[idx] = temp[idx];
			idx++;

			printf("%s\t", directory_path[idx-1]);
		} else {
			printf("%s\t", directory->d_name);
		}

		line_cnt++;

		if(line_cnt > 5){
			printf("\n");
			line_cnt = 1;
		}
	}

	printf("\n\n");

	while(directory_path[p_idx] != NULL){
		if(directory_path[p_idx] == NULL) break;
		recurvDirectory(directory_path[p_idx]);
		p_idx++;
	}

	closedir(dir);
}

int main(int argc, char *argv[]){
	char dir[1024];

	if(argc == 1) strcpy(dir, ".");
	else strcpy(dir, argv[1]);

	recurvDirectory(dir);
	return 0;
}


