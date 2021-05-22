#ifndef MAIN_H
#define MAIN_H

typedef struct arguments_struct{
    char func;
    char * secretImage;
    int k;
    char * directorio;
} arguments_struct;

arguments_struct * checkArguments(int argc, char *argv[]);
void distribution(arguments_struct* args);
void recompilation(arguments_struct* args);

#endif