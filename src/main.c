#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/images.h"
#include "include/main.h"
#include <dirent.h> 

int main(int argc, char *argv[]) {
	
	arguments_struct * args = checkArguments(argc,argv);

	//Para ver si agarra bien los files
	for(int i = 0 ; i < args->n ; i++){
		printf("File %d: %s \n",i,args->images[i]);
	}

	printf("func: %c, secret: %s, k: %d, directorio: %s\n", args->func, args->secretImage, args->k, args->directorio);

	if(args->func == 'd'){
		distribution(args);
	}else{
		//recompilation(args);
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
	DIR * d = opendir(args->directorio);
	if(!d){
		fprintf(stderr, "Directory must exists\n");
		exit(1);
	}
	struct dirent * dir;
	int fileCounter = 0;
	while ((dir = readdir(d)) != NULL){
		if(dir->d_type == DT_REG) { /* only deal with regular file */
			const char *ext = strrchr(dir->d_name,'.');
			if((!ext) || (ext == dir->d_name)){
				
			}
			else {
				if(strcmp(ext, ".bmp") == 0){
					args->images[fileCounter] = malloc(strlen(args->directorio) + strlen(dir->d_name) + 1);
					strcpy(args->images[fileCounter], args->directorio);
					strcat(args->images[fileCounter],dir->d_name);
					args->images[fileCounter][strlen(args->directorio) + strlen(dir->d_name) + 1] = '\0';
					fileCounter++;
				}
			}
		}
	}
	closedir(d);
	args->n = fileCounter;
	
	return args;	
}

void distribution(arguments_struct* args){
	printf("starting distribution\n");

	int k = args->k;

	//llamamos a anlaize image para la imagens que uqeremos ocultar
	printf("--------------------------secretImage\n");
	IMAGEDATA * image_data = analizeImage(args->secretImage);

	//conseguimos en base al nombre del directorio un array con todos los nombres de las imagenes portadoras
	char ** host_image_paths = args->images;
	int host_image_count = args->n;
	//printf("---------------------host Image Count: %d\n", host_image_count);
	
	//alocar espacio para el array la data de imagenes portadoras del secreto
	IMAGEDATA ** host_images = malloc(sizeof(IMAGEDATA) * host_image_count);

	// llamamos analize image para todas las imagenes donde se va a ocultar el mensaje
	for(int n=0; n<host_image_count; n+=1){
		//printf("----------------------host Image %d Info:\n", n);
		host_images[n] = analizeImage(host_image_paths[n]);
	}

	//iteramos  la data de image_data en intervalos de tamaño k
	for(int i=0; i<(int)image_data->biSize; i+=k){
		unsigned int * segment = malloc(k);

		//printf("segment %d: ", i);
		for(int m=0; m<k; m++){
			segment[m] = image_data->bitmapImage[i+m];
			//printf("%d ",segment[m]);
		}
		//printf("\n");

		//iteramos cada imagen a ocultar y actualizamos el valor del cuadrado correspondiente.
		int * valuesOfX = malloc(host_image_count * sizeof(int));

		for(int h=0; h<host_image_count; h++){
			valuesOfX[h] = -1;
		}

		int g = i/k;
		// printf("g is: %d", g);
		for(int j=0; j < host_image_count; j++){
			int imageWidth = host_images[j]->biWidth;
			int blocksPerLine = imageWidth/2;
			int pointer = (g%blocksPerLine)*2+(2*imageWidth)*(g/blocksPerLine);

			unsigned int x = host_images[j]->bitmapImage[pointer];
			// while(valueIsPrecent(valuesOfX, x)){
			// 	if(x<255){
			// 		x++;
			// 	}else{
			// 		x=0;
			// 	}
			// }
			unsigned int w = host_images[j]->bitmapImage[pointer+1];
			unsigned int u = host_images[j]->bitmapImage[pointer+imageWidth];
			unsigned int v = host_images[j]->bitmapImage[pointer+imageWidth+1];

			//printf("square %d values are: [%d,%d,%d,%d]\n", g, x, w, u, v);

			// tarea scott
			// unsigned int y = evaluatePolinomial(x, segment);
			// unsigned int * newValues = malloc(3*sizeof(unsigned int));
			// newValues = calculateOfuscatedValues(w,u,v,y);
			// host_images[j]->bitmapImage[pointer+1] = newValues[0];
			// host_images[j]->bitmapImage[pointer+imageWidth] = newValues[1];
			// host_images[j]->bitmapImage[pointeri+imageWidth+1] = newValues[2];
			// free(newValues);

		}
		free(valuesOfX);
		free(segment);
	}
	//updateamos todas las imagenes con la data cambiada.

}

void recompilation(arguments_struct* args){
	printf("starting recompilation\n");
	analizeImage(args->secretImage);
}