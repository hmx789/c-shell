#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>

void sigint_handler(int sig){
  char msg[] = "SIGINT handled\n";
  write(1, msg, sizeof(msg));
  
}

void sigstp_handler(int sig) {
	char msg[] = "SIGTSTP handled\n";
	write(1,msg,sizeof(msg)); 

}

int main(){
	signal(SIGINT, sigint_handler);
	signal(SIGTSTP, sigstp_handler);
	char *file = (char*)0;
	char *redirect = (char*)0;
	while(1) {
 		//create some space for our strings
		char input[500];
  	char line[500];
		char *args[20];
		printf("361>");
  	fgets(line, 500, stdin);

  	//break the string up into words
  	char *word = strtok(line," \n\0");
		if (strcmp(line,"exit\0")==0) {
			exit(0);
		}
  	int i = 0;
  	while (word) {
    	//copy a word to the arg array
			if(strcmp(word,">")==0 || strcmp(word,">>")==0 || strcmp(word,"<")==0) {
				//printf("Going inside the redirect\n");
				redirect = word;
				//printf("Put the redirect symbol inside redirect");
				word = strtok(NULL, " \n\0");
				file = word;
				break; 
			}
			else {
				//strcpy(args[i], word);
				args[i] = word;
				word = strtok(NULL, " \n\0");
				i = i + 1;
			}
  	}
		args[i] = (char*)0;
		
		int pid = fork();
		
		if (pid == 0) {
			if(redirect == (char*)0) {
				execv(args[0],args);
				exit(0);
			}
			else if(strcmp(redirect,">")==0) {
				int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IROTH);
				dup2(fd,1);		//gets the process from output and puts into file
				execv(args[0],args);
				close(fd);		//closing the file`
			}
			else if(strcmp(redirect,">>")==0) {
				int fd = open(file, O_APPEND | O_WRONLY);
				dup2(fd,1);
				execv(args[0],args);
				close(fd);
				
			}
			else if(strcmp(redirect,"<")==0) {
				int fd = open(file,O_RDONLY);
				dup2(fd,0);
				execv(args[0],args);
				close(fd);
			}
 		} else {
    	printf("child PID: %d.\n", pid);
    	int status;
    	wait(&status);	//waiting for the child
  		}
			redirect = (char*)0;
      file = (char*)0; 
	}	
}
