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

void copy_link(char* ori, char* dest, struct stat st){
	char *buf = malloc(st.st_size + 1);
	readlink(ori, buf, st.st_size + 1);
	buf += '\0';
	symlink(buf, dest);
}

int main(int argc, char *argv[])
{
	int fdo = open(argv[1], O_RDONLY);
	struct stat st;
	if (lstat(argv[1], &st) == -1) {
		perror("lstat failed");
		return 1;
	}
	if(S_ISLNK(st.st_mode)){
		printf("simbolico");
		copy_link(argv[1], argv[2], st);
	}
	else if(S_ISREG(st.st_mode)){
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
	else{
		perror("is not a regular file");
		return 1;
	}
}
