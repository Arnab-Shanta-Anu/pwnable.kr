#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(){
   int i;

    /* Stage 1 is looking for 100 arguments (we need one extra space at the end)*/
    char *args[101] = {};

    for (i=0; i<101; i++) {
        args[i] = "A"; // fill up 100 arguments with a filler char
    }

    args['A'] = "\x00";
    args['B'] = "\x20\x0a\x0d";
    args['C']="55555";
	args[100] = NULL;
    //execve("/home/blaze/pwnable.kr/toddler/input/input", args, NULL);

	setenv("\xde\xad\xbe\xef","\xca\xfe\xba\xbe",1);
	extern char** environ;
	
	//file
	
	FILE* fp=fopen("\x0a", "w");
	fwrite("\x00\x00\x00\x00",4,1,fp);
	fclose(fp);

    //stdio
	
	pid_t childpid;
	int pipe_stdin[2];
	int pipe_stderr[2];

	if(pipe(pipe_stdin) < 0 || pipe(pipe_stderr) < 0 ) {
		perror("oh no\n");
		exit(1);
	}

	if((childpid = fork()) < 0){
		perror("fork failed\n");
		exit(1);
	}
	if(childpid == 0){
		close(pipe_stdin[0]);
		close(pipe_stderr[0]);

		write(pipe_stdin[1], "\x00\x0a\x00\xff", 4);
		write(pipe_stderr[1],"\x00\x0a\x02\xff", 4);
		
		sleep(5);
		int sd, cd;
		struct sockaddr_in saddr, caddr;
		sd = socket(AF_INET, SOCK_STREAM, 0);
		if(sd == -1){
			printf("socket error, tell admin\n");
			return 0;
		}
		saddr.sin_family = AF_INET;
		saddr.sin_addr.s_addr = INADDR_ANY;
		saddr.sin_port = htons( atoi(args['C']) );

		if(connect(sd, (struct sockaddr *)&saddr, sizeof(saddr))<0)
		{
			printf("\n Error : Connect Failed \n");
			return 1;
		}

		write(sd, "\xde\xad\xbe\xef", 4);
		close(sd); 	

		return 0;
	}
	
	else{
		close(pipe_stdin[1]);
		close(pipe_stderr[1]);

		dup2(pipe_stdin[0],0);
		dup2(pipe_stderr[0],2);

		close(pipe_stdin[0]);
		close(pipe_stderr[0]);

	execve("/home/blaze/pwnable.kr/toddler/input/input",args,environ);

	}

	//env



	// network

	
	//finally executing the binary
	//done

	return 0;
}
