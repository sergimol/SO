#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fdo = open(argv[1], O_RDONLY);
	if(fdo == -1){
			perror("open failed");
			return -1;
	}

	if(getopt(argc, argv, "n:") != -1){
		int N = 0;
		N = atoi(optarg);	
		if(getopt(argc, argv, "e") != -1){
			lseek(fdo, -N, SEEK_END);	
		}
		else{
			lseek(fdo, N, SEEK_SET);
		}		
	}

	char *buf = malloc(1);
	while(read(fdo, buf, 1) > 0){
		printf(buf);
	}
	return 0;
}
