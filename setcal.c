#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LENGTH 30

char command1[7] = {"empty\0"};
char command2[6] = {"card\0"};
char command3[12] = {"complement\0"};
char command4[7] = {"union\0"};
char command5[11] = {"intersect\0"};
char command6[7] = {"minus\0"};
char command7[10] = {"subseteq\0"};
char command8[8] = {"subset\0"};
char command9[8] = {"equals\0"};
char command10[11] = {"reflexive\0"};
char command11[11] = {"symmetric\0"};
char command12[15] = {"antisymmetric\0"};
char command13[12] = {"transitive\0"};
char command14[10] = {"function\0"};
char command15[8] = {"domain\0"};
char command16[10] = {"codomain\0"};
char command17[11] = {"injective\0"};
char command18[12] = {"surjective\0"};
char command19[11] = {"bijective\0"};
char forbidden_word1[6] = {"true\0"};
char forbidden_word2[7] = {"false\0"};
char *list_of_forbidden_elements[21] = {command1, command2, command3, command4, command5, command6, command7, command8, command9, command10, command11, command12, command13, command14, command15, command16, command17, command18, command19, forbidden_word1, forbidden_word2};

typedef struct string_t{
    char *str;
    int length;
} str;

typedef struct storage_t{
    char **array;
    char type;
    int length;
} stg;

typedef struct command_t{
    str action;
    int *operants;
} comm;

void str_ctor(str *p_word){
    p_word->str = NULL;
    p_word->length = 0;
}

void str_dtor(str *p_word){
    if (p_word->str != NULL){
        free(p_word->str);
    }
}

void word_dtor(char *p_word){
    if (p_word != NULL){
        free(p_word);
    }
}

void stg_ctor(stg *p_stg, char type){
    p_stg->array = NULL;
    p_stg->type = type;
    p_stg->length = 0;
}

void stg_dtor(stg *p_stg){
    if (p_stg->array != NULL){
        free(p_stg->array);
    }
}

int read_word(str *p_line, str *p_word, int start){
    int i = 0;
    while (p_line->str[start + i] != 32 && p_line->str[start + i] != '\n' && p_line->str[start + i] != EOF){
        if (i == MAX_LENGTH){
            fprintf(stderr, "Too long string on input.\nGiven string:\n%s", p_line->str);
            return -1;
        }
        p_word->str = realloc(p_word->str, sizeof(p_word->str) + (sizeof(char) * (p_word->length + 1)));
        if (p_word->str == NULL){
            fprintf(stderr, "Memory issues, the program ran out of memory.");
            return -1;
        }
        p_word->str[i] = p_line->str[i + start];
        p_word->str[i + 1] = '\0';
        p_word->length++;
        i++;
    }
    return 1;
}

char * is_in_uni(str *p_word, stg *comparison){
    int i = 0;
    while (i < comparison->length){
        if (strcmp(p_word->str, comparison->array[i]) == 0){
            return comparison->array[i];
        }
        i++;
    }
    return NULL;
}

int read_line(FILE *p_file, str *p_line){
    p_line->str = malloc(sizeof(char));
    do {
        p_line->str = realloc(p_line->str, sizeof(p_line->str) + (sizeof(char) * (p_line->length + 1)));
        if (p_line->str == NULL){
            fprintf(stderr, "Memory issues, the program ran out of memory.");
            return -1;
        }
        p_line->str[p_line->length] = fgetc(p_file);
        p_line->str[p_line->length + 1] = '\0';
        p_line->length++;
    } while (p_line->str[p_line->length - 1] != '\n' && p_line->str[p_line->length - 1] != EOF);
    return 1;
}

int seperate_word_in_line(str *p_line, str *word, int *progres){
    str_ctor(word);
    if (read_word(p_line, word, *progres) == -1){
        return -1;
    }
    if (word->str == NULL){
        fprintf(stderr, "Too many spaces between words, the next word couldnt be read properly.");
        return -1;
    }
    *progres += word->length + 1;
    return 1;
}

int make_uni(str *p_line, stg *p_uni){
    int progres = 2;
    int i = 0;
    int j = 0;
    str word;
    while (progres < p_line->length){
        j = 0;
        if (seperate_word_in_line(p_line, &word, &progres) == -1){
            return -1;
        }
        while (j < 21){
            if ((strcmp(word.str, list_of_forbidden_elements[j]) == 0)){
                fprintf(stderr, "Invalid input, you entered a forbidden word");
            }
            j++;
        }
        p_uni->array = realloc(p_uni->array, sizeof(p_uni->array) * (p_uni->length + 1) );
        if (p_uni->array == NULL){
            fprintf(stderr, "Memory issues, the program ran out of memory.");
            return -1;
        }
        p_uni->array[i] = word.str;
        p_uni->length++;
        i++;
    }
    return 1;
}

int unique_element(stg *p_set){
    for (int i = 0; i < p_set->length - 1; i++){
        for (int j = i + 1; j < p_set->length; j++){
            if (p_set->array[i] == p_set->array[j]){
                fprintf(stderr, "Element %s is not unique.\n", p_set->array[i]);
                return -1;
            }
        }
    }
    return 1;
}

int make_set(str *p_line, stg *p_set, stg*p_uni){
    int progres = 2;
    int i = 0;
    str word;
    while (progres < p_line->length){
        if (seperate_word_in_line(p_line, &word, &progres) == -1){
            return -1;
        }
        p_set->array = realloc(p_set->array, sizeof(p_uni->array) * (p_uni->length + 1));
        if (p_set->array == NULL){
            fprintf(stderr, "Memory issues, the program ran out of memory.");
            return -1;
        }
        p_set->array[i] = is_in_uni(&word, p_uni);
        if (p_set->array[i] == NULL){
            fprintf(stderr, "The word '%s' isn`t located in the univerzum.", word.str);
            return -1;
        }
        str_dtor(&word);
        p_set->length++;
        i++;
    }
    
    if (unique_element(p_set) == -1){
        return -1;
    }
    return 1;
}

int unique_pair(stg *rel){
    const int pair = 2;
    for (int i = 0; i < rel->length; i += pair){
        for (int j = i + pair; j < rel->length; j += pair){
            if(rel->array[i] == rel->array[j] && rel->array[i+1] == rel->array[j+1]){
                fprintf(stderr, "Invalid relation (one pair more then once).");
                return -1;
            }
        }
    }
    return 1;
}

int make_rel(str *p_line, stg *p_rel, stg *p_uni){
    int progres = 2;
    int i = 0;
    str word1;
    str word2;
    while (progres < p_line->length){
        if (p_line->str[progres] == '('){
            progres++;
            if ((seperate_word_in_line(p_line, &word1, &progres) == -1) || (seperate_word_in_line(p_line, &word2, &progres) == -1)){
                return -1;
            }
            if (word2.str[word2.length - 1] == ')'){
                word2.str[word2.length - 1] = '\0';
                word2.length--;
            }
            else {
                fprintf(stderr, "Invalid formulation of relation, accepted format is (domain range)!!");
                return -1;
            }
            p_rel->array = realloc(p_rel->array, sizeof(p_uni->array) * (p_uni->length + 2));
            if (p_rel->array == NULL){
                fprintf(stderr, "Memory issues, the program ran out of memory.");
                return -1;
            }
            p_rel->array[i] = is_in_uni(&word1, p_uni);
            p_rel->array[i + 1] = is_in_uni(&word2, p_uni);
            if ((p_rel->array[i] == NULL) || (p_rel->array[i + 1] == NULL)){
                fprintf(stderr, "The relation ('%s' '%s') isn`t located in the univerzum.", word1.str, word2.str);
                return -1;
            }
            str_dtor(&word1);
            str_dtor(&word2);
            p_rel->length += 2;
            i += 2;
        }
        else {
            fprintf(stderr, "Invalid formulation of relation, accepted format is (domain range)!!");
            return -1;
        }
    }
    if (unique_pair(p_rel) == -1){
        return -1;
    }
    return 1;
}

int store_line(FILE *p_file, stg *p_uni, stg *p_stg, int *lines_count){
    str line;
    str_ctor(&line);
    int j = 0;
    if (read_line(p_file, &line) == -1){
        return -1;
    }
    (*lines_count)++;
    if (line.str[0] == 'U' && *lines_count == 1){
        if(line.str[1] == ' '){ 
            stg_ctor(p_uni, 'U');
            if (make_uni(&line, p_uni) == -1){
                return -1;
            }
            while (j < p_uni->length){
                printf("%s ", p_uni->array[j]);
                j++;
            }
            printf("\n");
            str_dtor(&line);
            return 2;
        }
    }
    else if (line.str[0] == 'S' && *lines_count != 1){
        if(line.str[1] == 32){
            stg_ctor(p_stg, 'S');
            if (make_set(&line, p_stg, p_uni) == -1){
                return -1;
            }
            while (j < p_stg->length){
                printf("%s ", p_stg->array[j]);
                j++;
            }
            printf("\n");
            str_dtor(&line);
            return 1;
        }
    }
    else if (line.str[0] == 'R' && *lines_count != 1){
        if(line.str[1] == 32){
            stg_ctor(p_stg, 'R');
            if (make_rel(&line, p_stg, p_uni) == -1){
                return -1;
            }
            while (j < p_stg->length){
                printf("(%s %s)", p_stg->array[j], p_stg->array[j + 1]);
                j += 2;
            }
            printf("\n");
            str_dtor(&line);
            return 0;
        }
    }
    fprintf(stderr, "Invalid format, after declaration of type of the line must come space.");
    return -1;
}

int main(){
    FILE *p_f;
    p_f = fopen("text.txt", "r");
    if (p_f == NULL){
        return 0;
    }
    stg uni;
    stg stg;
    int lines = 0; 
    int i = 0;
    int pocet_zadanych_radku = 3;
    while (i < pocet_zadanych_radku){
        if (store_line(p_f, &uni, &stg, &lines) == -1){
            return -1;
        }
        i++;
    }
    i = 0;
    while (i < uni.length){
        word_dtor(uni.array[i]);
        i++;
    }
    stg_dtor(&stg);
    stg_dtor(&uni);
    fclose(p_f);
    return 0;
}