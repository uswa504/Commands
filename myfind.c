#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
extern int errno;
void do_find_by_name(char*, char*);
void do_find_by_type(char*, char*);
char* file_type(char*);
int main(int argc, char* argv[]){
	if (argc < 4)
		printf("Invalid number of arguments\n");
   	else if (strcmp(argv[2], "-name") == 0)
		do_find_by_name(argv[1], argv[3]);
	else if(strcmp(argv[2], "-type") == 0)
		do_find_by_type(argv[1], argv[3]);
}
char* file_type(char *filename){
	struct stat buf;
   	if (lstat(filename, &buf)<0){
      		perror("Error in stat");
     	 	exit(1);
   	}
	if ((buf.st_mode &  S_IFMT) == S_IFIFO)
        	return "p";
        else if ((buf.st_mode &  S_IFMT) == S_IFCHR)
	        return "c";
        else if ((buf.st_mode &  S_IFMT) == S_IFDIR)
                return "d";
        else if ((buf.st_mode &  S_IFMT) == S_IFBLK)
                return "b";
        else if ((buf.st_mode &  S_IFMT) == S_IFREG)
                return "-";
        else if ((buf.st_mode &  S_IFMT) == S_IFLNK)
                return "l";
        else if ((buf.st_mode &  S_IFMT) == S_IFSOCK)
                return "s";
        else return "u";

}
void do_find_by_type(char* path, char *type){
	int count = 0;
	struct dirent *entry;
        DIR * dp = opendir(path);
        if (dp == NULL){
                fprintf(stderr, "Directory not found \n");
                return;
        }
        errno = 0;
        while((entry = readdir(dp)) != NULL){
                if(entry == NULL && errno != 0){
                        perror("readdir failed");
                        exit(1);
                }
                else{
			if(strcmp(file_type(entry->d_name), type) == 0){
				count++;
			}
                }
	}
	printf("%d files of this type found in specified path\n", count);
}
void do_find_by_name(char* path, char* filename){
	struct dirent * entry;
	DIR * dp = opendir(path);
	if (dp == NULL){
                fprintf(stderr, "Directory not found \n");
                return;
        }
        errno = 0;
	while((entry = readdir(dp)) != NULL){
                if(entry == NULL && errno != 0){
                        perror("readdir failed");
                        exit(1);
                }
                else{
			if(strcmp(entry->d_name, filename) == 0){
				printf("File Found\n");
				exit(0);
			}
		}
	}
	printf("File not found in specified path\n");
        closedir(dp);
}
