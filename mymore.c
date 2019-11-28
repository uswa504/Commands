#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//#include <windows.h>
#include <sys/wait.h>
#include <sys/types.h>
#define PAGELEN 20
#define LINELEN 512
void do_more(FILE *);
int percentage;
int res;
int get_input(FILE *);
long int find_filesize(char* );
int main(int argc, char *argv[]){
	/*CONSOLE_SCREEN_BUFFEE_INFO csbi;
	int rows;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top +1;
	printf(rows);*/
	system("/bin/stty -echo <&2");
	system("/bin/stty -icanon <&2");
	int i=0;
	char* size = getenv("SIZE");
	printf("size = %s", size);
	if(argc == 1)
		do_more(stdin);
	FILE * fp;
	while(++i < argc){
		fp = fopen(argv[i] , "r");
		res = find_filesize(argv[i]);
		if(fp == NULL){
			perror("Can't open file");
			exit(1);
   		}
   		do_more(fp);
   		fclose(fp);
	}
  	system("/bin/stty echo <&2");
	system("/bin/stty icanon <&2");
	return 0;
}
void do_more(FILE *fp){
	int num_of_lines = 0;
	int rv, lines=0;
	char buffer[LINELEN];
	FILE* fp_tty = fopen("/dev//tty", "r");
	while(fgets(buffer, LINELEN, fp)){
		fputs(buffer, stdout);
		lines++;
		num_of_lines++;
		percentage = (lines*100)/res;
		if(num_of_lines == PAGELEN){
			printf("\n");
			rv = get_input(fp_tty);
        	 	if (rv == 0){
				printf("\033[1B \033[2K \033[1G");
         			break;
			}
         		else if (rv == 1){
				printf("\033[1A \033[2K \033[1G");
         			num_of_lines -= PAGELEN;
			}
         		else if (rv == 2){
				printf("\033[1A \033[2K \033[1G");
	 			num_of_lines -= 1;
			}
			else if (rv == 3){
				int pid = fork();
				if(pid == -1)
					perror("failed");
				else if(pid == 0)
					execlp("vim", "vim", 0);
				else
					wait(NULL);
			}
         		else if (rv == 4){
				printf("\033[1B \033[2K \033[1G");
         			break;
			}
      		}
  	}
}
long int find_filesize(char* filename){
	FILE* fp = fopen(filename, "r");
	char c;
	int count = 0;
	for(c=getc(fp); c!=EOF; c=getc(fp)){
		if(c == '\n');
			count=count+1;
	}
	fclose(fp);
	return count;
}
int get_input(FILE* cmdstream){
	int c;
	printf("\033[7m --more--(%d%) \033[m", percentage);
	c = getc(cmdstream);
	if(c == 'q')
		return 0;
        if ( c == ' ')
	 	return 1;
        if ( c == '\n')
	 	return 2;
	if ( c == 'v')
		return 3;
	return 4;
   return 0;
}

