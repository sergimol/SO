#include <stdio.h>
#include <stdlib.h>

int N = 0;

int main(int argc, char *argv[])
{
	if(getopt(argc, argv, "n:") != -1){
		N = optarg - '0';
		if(getopt(argc, argv, "e") != -1){
			
		}
		else{

		}
	}
	
	return 0;
}
