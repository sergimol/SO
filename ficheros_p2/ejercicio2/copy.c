#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

size_t SIZE = 512;

void copy(int fdo, int fdd)
{
	char *buf = malloc(SIZE);
	char end = '\0';
	size_t r;
	size_t w = 0;
	while((r = read(fdo, buf, SIZE)) > 0 && w >= 0){
		printf("%d\n", r);
		w = write(fdd, buf, r);
	}
	if(r < 0)
		perror("read failed");
	if(w < 0)
		perror("write failed");
	free(buf);
}

int main(int argc, char *argv[])
{
	int fdo = open(argv[1], O_RDONLY);
	int fdd = open(argv[2], O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	if(fdo == -1 || fdd == -1){
		perror("open failed");
		return 1;
	}
	copy(fdo, fdd);
	if(close(fdo) != -1 && close(fdd) != -1)
		return 0;
	else{
		perror("close failed");
		return 1;
	}
}
