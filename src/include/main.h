#ifndef MAIN_H
#define MAIN_H

#define MAX_IMAGES 16
typedef struct arguments_struct{
    char func;
    char * secretImage;
    int k;
    char * directorio;
    int n;
    char * images[MAX_IMAGES];
} arguments_struct;

arguments_struct * checkArguments(int argc, char *argv[]);
void distribution(arguments_struct* args);
void recompilation(arguments_struct* args);

#endif