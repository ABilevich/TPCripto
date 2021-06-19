#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/images.h"
#include "include/main.h"
#include "include/galois_8.h"
#include "include/interpolation.h"
#include <dirent.h> 
#include <stdint.h>

int main(int argc, char *argv[]) {
	
	arguments_struct * args = checkArguments(argc,argv);

	//Para ver si agarra bien los files
	// for(int i = 0 ; i < args->n ; i++){
	// 	printf("File %d: %s \n",i,args->images[i]);
	// }

	//printf("func: %c, secret: %s, k: %d, directorio: %s\n", args->func, args->secretImage, args->k, args->directorio);

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
	if(args->n < args->k){
		fprintf(stderr, "There must be at least k images.\n");
		exit(1);
	}

	return args;
}

void distribution(arguments_struct* args){
	//printf("starting distribution\n");

	int k = args->k;

	//llamamos a anlaize image para la imagens que uqeremos ocultar
	IMAGEDATA * image_data = analizeImage(args->secretImage);
	// for( int i = 0; i < (int)image_data->biSize; i+=1){
	// 	printf("%d\n", image_data->bitmapImage[i]);
	// }


	
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
		//printf("------ iteration %d ------\n",i);
		uint8_t segment[k];

		//printf("segment %d: ", i);
		for(int m=0; m<k; m++){
			segment[m] = image_data->bitmapImage[i+m];
			//printf("%d ",segment[m]);
		}
		//printf("\n");

		//iteramos las host images y actualizamos el valor del cuadrado correspondiente.
		uint8_t valuesOfX[host_image_count];
		for(int h=0; h<host_image_count; h++){
			valuesOfX[h] = -1;
		}

		int g = i/k;
		// printf("g is: %d", g);
		for(int j=0; j < host_image_count; j++){
			int imageWidth = host_images[j]->biWidth;
			int blocksPerLine = imageWidth/2;
			int pointer = (g%blocksPerLine)*2+(2*imageWidth)*(g/blocksPerLine);

			uint8_t x = host_images[j]->bitmapImage[pointer];
			while(valueIsPresent(valuesOfX, j, x)){
				if(x<255){
					x++;
				}else{
					x=245;
				}
			}
			valuesOfX[j] = x;
			uint8_t w = host_images[j]->bitmapImage[pointer+1];
			uint8_t v = host_images[j]->bitmapImage[pointer+imageWidth];
			uint8_t u = host_images[j]->bitmapImage[pointer+imageWidth+1];

			//printf("square %d values are: [%d,%d,%d,%d]\n", g, x, w, u, v);

			uint8_t y = g_evaluatePolinomial(x, segment, k);
			
			uint8_t * newValues = calculateOfuscatedValues(w,v,u,y);
			host_images[j]->bitmapImage[pointer] = x;
			host_images[j]->bitmapImage[pointer+1] = newValues[0];
			host_images[j]->bitmapImage[pointer+imageWidth] = newValues[1];
			host_images[j]->bitmapImage[pointer+imageWidth+1] = newValues[2];

			uint8_t new_y = reconstructY(newValues[0],newValues[1],newValues[2]);
			// if(i+j<20){
			// 	printf("value number %d: (%d,%d): {%d} [w: %d / v: %d / u:%d]\n", i+j, x, y, new_y, newValues[0], newValues[1], newValues[2]);
			// }
			

			free(newValues);


		}

		// if(i<k*3){
		// 	printf("segment is: [");
		// 	for(int j=0; j<k ; j++){
		// 		printf("%d,",segment[j]);	
		// 	}
		// 	printf("]\n");
		// }
	}
	// updateamos todas las imagenes con la data cambiada.
	for(int i = 0; i < host_image_count; i++){
		updateImageData(host_image_paths[i], host_images[i]);
	}
}

void recompilation(arguments_struct* args){
	//printf("starting recompilation\n");

	int k = args->k;

	//conseguimos en base al nombre del directorio un array con todos los nombres de las imagenes portadoras
	char ** host_image_paths = args->images;
	int host_image_count = args->n;
	//printf("---------------------host Image Count: %d\n", host_image_count);
	
	if(host_image_count < k){
		fprintf(stderr,"Not enough Images to reconstruct original image\n");
		exit(1);
	}
	
	//alocar espacio para el array la data de imagenes portadoras del secreto
	IMAGEDATA ** host_images = malloc(sizeof(IMAGEDATA) * host_image_count);

	// llamamos analize image para todas las imagenes donde se va a ocultar el mensaje
	for(int n=0; n<host_image_count; n+=1){
		//printf("----------------------host Image %d Info:\n", n);
		host_images[n] = analizeImage(host_image_paths[n]);
	}

	int image_size = (int)host_images[0]->biSize;
	uint8_t * bitmapImage = malloc(sizeof(uint8_t) * image_size);
	
	//iteramos  la data de las imagenes portadoras en intervalos de tamaño k
	for(int i=0; i<image_size; i+=k){
		// printf("------ iteration %d ------\n",i);
		uint8_t * segment = malloc(sizeof(uint8_t) * k);
		
		int g = i/k;

		uint8_t point_x[host_image_count];
		uint8_t point_y[host_image_count];

		for(int j=0; j < host_image_count; j++){
			//printf("HostImage %d\n", j);
			int imageWidth = host_images[j]->biWidth;
			int blocksPerLine = imageWidth/2;
			int pointer = (g%blocksPerLine)*2+(2*imageWidth)*(g/blocksPerLine);

			uint8_t x = host_images[j]->bitmapImage[pointer];
			uint8_t w = host_images[j]->bitmapImage[pointer+1];
			uint8_t v = host_images[j]->bitmapImage[pointer+imageWidth];
			uint8_t u = host_images[j]->bitmapImage[pointer+imageWidth+1];

			uint8_t y = reconstructY(w,v,u);
			
			point_x[j] = x;
			point_y[j] = y;

			// if(i+j<20){
			// 	printf("value number %d: (%d,%d) [w: %d / v: %d / u:%d]\n", i+j, x, y, w, v, u);
			// }
		}
		//printf("------ la lagrangiananashex ------\n");
		lagrange_interpolation(point_x, point_y, host_image_count, segment);
		//printf("finiquitada\n");

		for(int h=0;h<k;h++){
			bitmapImage[i+h] = segment[h];
		}

		// if(i<k*3){
		// 	printf("segment is: [");
		// 	for(int j=0; j<k ; j++){
		// 		printf("%d,",segment[j]);	
		// 	}
		// 	printf("]\n");
		// 	printf("bitmapImage is: [");
		// 	for(int j=0; j<k ; j++){
		// 		printf("%d,",bitmapImage[i+j]);	
		// 	}
		// 	printf("]\n");
		// }
			
		free(segment);
		//printf("------ end iteration %d ------\n",i);
	}
	
	// for( int i = 0; i < image_size; i+=1){
	// 		printf("%d\n", bitmapImage[i]);
	// }

	createImage(args->images[0], args->secretImage, bitmapImage);
	free(bitmapImage);
	
}

int valueIsPresent(uint8_t * valuesOfX, int valuesOfXSize, uint8_t x){
	for(int i = 0 ; i < valuesOfXSize ; i++){
		if(x == valuesOfX[i]){
			return 1;
		}
	}
	return 0;
}