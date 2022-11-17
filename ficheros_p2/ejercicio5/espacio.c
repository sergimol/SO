#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

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
	
	if(S_ISDIR(st.st_mode)){
		printf("directorio");
		return get_size_dir(fname, blocks);
	}

	blocks += st.st_blocks;
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
		if(get_size(dt->d_name ,blocks) == -1){
			perror("getsize failed");
			return -1;
		}	
	}
}

/* Processes all the files in the command line calling get_size on them to
 * obtain the number of 512 B blocks they occupy and prints the total size in
 * kilobytes on the standard output
 */
int main(int argc, char *argv[])
{
	size_t *blocks = 0;
	int code = get_size(argv[1], blocks);

	size_t size = blocks * 512 / 2;
	return code;
}
