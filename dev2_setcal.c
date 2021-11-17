#include <stdio.h>
#include <stdbool.h>

#define MAX_ARG 2

/**
* Funkce pro kontrolu parametru
* Pokud je zadan nevhodny argument, program vypise na vystupu chybovou hlasku a ukonci se
**/

int arg_check (int argc){
    if (argc != MAX_ARG){
        printf("Invalid count of arguments!\n");
        return 0;
    }
    return 1;
}


int main (int argc, char* argv[]){
    FILE *soubor;
    char s;

    if (arg_check (argc)){
        //printf("ARG jo\n");
        soubor = fopen(argv[1], "r");
        if (soubor != NULL){
            while ((s = fgetc(soubor)) != EOF){
                printf("%c", s);
            }
        }
        else {
            printf("File not found!\n");
            return 1;
        }    
            fclose(soubor);
    }



    return 0;
}