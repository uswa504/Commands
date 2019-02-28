#include<stdio.h>
#include<string.h>
#include <stdlib.h>
void mygrep(FILE*, char*);
void myreplace(FILE *fp,char *find, char * replace);
int  main(int argc,char *argv[]){
	int behaviour;
	FILE *fp;
	char *filename=argv[1];
	char *find=argv[2];
	char *replace;
	if(argc != 3){
		printf("usage\t./mygrep filename <string-to-search>\n");
		exit(1);
	}
	behaviour = GREP;
	else if(argc != 4){
		printf("\t./myreplace filename  <string-to-search> <string-to-replace>");
		exit(1);
	}
	behaviour = REPLACE;
	replace = argv[3];
	fp=fopen(filename,"rt");
	if(behaviour == GREP){
		mygrep(fp,find);
	}
	else if ( behaviour == REPLACE ){
		myreplace(fp,find,replace);
	}
	fclose(fp);
	return 0;
}
void mygrep(FILE *fp,char *find){
	char c1[500];
	while(fscanf(fp , "%[^\n]\n" , c1) != EOF){
		if(strstr(c1 , find) != NULL){
    	printf("%s\n" , line);
    }
    else continue;
	}
}
void myreplace(FILE *fp, char *find, char *replace){
	char c1[500];
	int i, count = 0;
	int flen = strlen(find);
	while(fscanf(fp , "%[^\n]\n" , c1) != EOF){
		if(strstr(c1 , find) != NULL){
			strcpy(c1, replace)
		}
	}
}
