#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

/* Forward declaration */
int get_size_dir(char *fname, size_t *blocks);

/* Gets in the blocks buffer the size of file fname using lstat. If fname is a
 * directory get_size_dir is called to add the size of its contents.
 */
int get_size(char *fname, size_t *blocks)
{
	struct stat st;
	if (lstat(fname, &st) == -1) {
		perror("lstat failed");
		return -1;
	}
	
	if((st.st_mode & S_IFMT) == S_IFDIR){
		printf("directorio\n");
		return get_size_dir(fname, blocks);
	}

	*blocks += st.st_blocks;
	return 0;
}


/* Gets the total number of blocks occupied by all the files in a directory. If
 * a contained file is a directory a recursive call to get_size_dir is
 * performed. Entries . and .. are conveniently ignored.
 */
int get_size_dir(char *dname, size_t *blocks)
{
	DIR* dir = opendir(dname);
	if(dir == NULL){
		perror("opendir failed");
		return -1;
	}

	struct dirent *dt;
	while((dt = readdir(dir)) != NULL){
		if(strcmp(dt->d_name, ".") == 0 || strcmp(dt->d_name, "..") == 0){
			printf("Punto\n");
			continue;
		}

		if(get_size(dt->d_name, blocks) == -1){
			perror("getsize failed");
			return -1;
		}
		// Está mal el tipo del bloque o algo porque escribe 15648858548897/4889+8 blocks
		printf("%lu blocks\n", blocks);	
	}

	return 0;
}

/* Processes all the files in the command line calling get_size on them to
 * obtain the number of 512 B blocks they occupy and prints the total size in
 * kilobytes on the standard output
 */
int main(int argc, char *argv[])
{
	size_t blocks = 0;
	int code = get_size(argv[1], &blocks);

	size_t size = (long unsigned int)blocks / 2;
	printf("%luK %s\n", size, argv[1]);
	return code;
}
