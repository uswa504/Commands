#include <inttypes.h>
#include <pwd.h>
#include <time.h>
#include <grp.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include<sys/stat.h>
#include <stdlib.h>
#include <errno.h>
extern int errno;
void do_ls(char*);
void file_permissions(char*);
char* file_type(char*);
void get_username(char*);
void get_groupname(char*);
void show_size(char*);
void show_mtime(char*);
void show_ctime(char*);
void show_linkcount(char*);
int main(int argc, char* argv[]){
	if (argc == 1)
      		do_ls(".");
   	else{
      		int i = 0;
      		while(++i < argc)
	 		do_ls(argv[i]);
      	}
   return 0;
}
void file_permissions(char* filename){
	struct stat buf;
   	if (lstat(filename, &buf)<0){
      		perror("Error in stat");
      		exit(1);
   	}
   	int mode = buf.st_mode;
   	char str[10];
   	strcpy(str, "---------");
//owner  permissions
   	if((mode & S_IRUSR) == S_IRUSR) str[0] = 'r';
   	if((mode & S_IWUSR) == S_IWUSR) str[1] = 'w';
   	if((mode & S_IXUSR) == S_IXUSR) str[2] = 'x';
//group permissions
   	if((mode & S_IRGRP) == S_IRGRP) str[3] = 'r';
   	if((mode & S_IWGRP) == S_IWGRP) str[4] = 'w';
   	if((mode & S_IXGRP) == S_IXGRP) str[5] = 'x';
//others  permissions
   	if((mode & S_IROTH) == S_IROTH) str[6] = 'r';
   	if((mode & S_IWOTH) == S_IWOTH) str[7] = 'w';
   	if((mode & S_IXOTH) == S_IXOTH) str[8] = 'x';
//special  permissions
   	if((mode & 0004000) == 0004000) str[2] = 's';
   	if((mode & 0002000) == 0002000) str[5] = 's';
   	if((mode & 0001000) == 0001000) str[8] = 't';
   	printf("%s ", str);
}
void get_username(char* filename){
	int uid = atoi(filename);
   	struct passwd *pwd = getpwuid(uid);
   	if (pwd == NULL)
      		perror("getpwuid failed OR record not found");
   	else printf("%s ", pwd->pw_name);
}
void get_groupname(char* filename){
	int gid = atoi(filename);
   	struct group * grp = getgrgid(gid);
   	if (grp == NULL)
		 perror("getgrgid failed OR record not found");
   	else printf("%s ", grp->gr_name);
}
void show_size(char* filename){
	struct stat info;
   	int rv = lstat(filename, &info);
   	if (rv == -1){
      		perror("stat failed");
      		exit(1);
   	}
   	printf("%ld ", info.st_size);
}
void show_linkcount(char* filename){
	struct stat info;
   	int rv = lstat(filename, &info);
   	if (rv == -1){
     		perror("stat failed");
     		exit(1);
   	}
   	printf("%ld ", info.st_nlink);
}
void show_mtime(char* filename){
	struct stat info;
	int rv = lstat(filename, &info);
        if (rv == -1){
                perror("stat failed");
                exit(1);
        }
        printf("%s ",ctime(& info.st_mtime));
}
char* file_type(char* filename){
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
void do_ls(char * dir){
	struct dirent * entry;
	DIR * dp = opendir(dir);
	if (dp == NULL){
		fprintf(stderr, "Cannot open directory:%s\n", dir);
      		return;
   	}
	char *type;
  	errno = 0;
	int inode_num;
   	while((entry = readdir(dp)) != NULL){
		if(entry == NULL && errno != 0){
			perror("readdir failed");
			exit(1);
  		}
		else if(entry->d_name[0] == '.')
			continue;
		printf("%" PRIuMAX,(uintmax_t)(entry->d_ino));
		printf(" ");
		type = file_type(entry->d_name);
		printf("%s", type);
		file_permissions(entry->d_name);
		show_linkcount(entry->d_name);
		get_username(entry->d_name);
		get_groupname(entry->d_name);
		show_size(entry->d_name);
		show_mtime(entry->d_name);
		printf("%s\n", entry->d_name);
	}
   	closedir(dp);
}
