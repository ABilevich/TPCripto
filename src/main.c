#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/images.h"
#include "include/main.h"

int main(int argc, char *argv[]) {
	
	arguments_struct * args = checkArguments(argc,argv);

	printf("func: %c, secret: %s, k: %d, directorio: %s\n", args->func, args->secretImage, args->k, args->directorio);

	if(args->func == 'd'){
		distribution(args);
	}else{
		recompilation(args);
	}

	free(args);
	return 0;
}

arguments_struct * checkArguments(int argc, char *argv[]){

	arguments_struct * args = malloc(sizeof(arguments_struct));
	
	if(argc != 5){
		fprintf(stderr, "4 arguments are required\n");
		exit(1);
	}
	
	if(strlen(argv[1]) != 1){
		fprintf(stderr, "first argument must be 'd' or 'r'\n");
		exit(1);
	}

	args->func = *argv[1];
	
	if (args->func != 'd' && args->func != 'r'){
		fprintf(stderr, "first argument must be 'd' or 'r'\n");
		exit(1);
	}

	args->secretImage = argv[2];

	args->k = atoi(argv[3]);
	if (args->k < 4 || args->k > 6){
		fprintf(stderr, "k must be 4, 5 or 6\n");
		exit(1);
	}

	args->directorio = argv[4];

	return args;	
}

void distribution(arguments_struct* args){
	printf("starting distribution\n");
	unsigned char * image_data = analizeImage(args->secretImage);
}

void recompilation(arguments_struct* args){
	printf("starting recompilation\n");
	analizeImage(args->secretImage);
}