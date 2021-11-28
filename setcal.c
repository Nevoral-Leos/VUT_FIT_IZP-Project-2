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
char *single_arg_comm[10] = {command1, command2, command3, command10, command11, command12, command13, command14, command15, command16};
char *double_arg_comm[6] = {command4, command5, command6, command7, command8, command9};
char *triple_arg_comm[3] = {command17, command18, command19};
char *list_of_forbidden_elements[21] = {command1, command2, command3, command4, command5, command6, command7, command8, command9, command10, command11, command12, command13, command14, command15, command16, command17, command18, command19, forbidden_word1, forbidden_word2};

/*
 * This structure is used to store each words and lines before they get seperated in a single words.
 * The words are stored as strings and we also know it`s length for the purpose of efficiency in some functions.
 */

typedef struct string_t{
    char *str;
    int length;
} str;

/*
 * This structure stores, what kind of line we store, if its Univerzum, Set or Relation.
 * In the array are stored all the words in Univerzum Set or Relation.
 * The words are stored one after another. In relation we use two positions in array to represent a single relation.
 */

typedef struct storage_t{
    char **array;
    char type;
    int length;
} stg;

/**
 * This structure stores commands and paramaters required for the command. 
 */

typedef struct command_t{
    char *str;
    int operands[3];
} comm;

/**
 * @brief This function initialiazes a string_t storage.
 * 
 * @param p_word pointer to the memory, at which the structure is stored
 */

void str_ctor(str *p_word){
    p_word->str = NULL;
    p_word->length = 0;
}

/**
 * @brief This function clears all the memory dynamically allocated in the area, that is pointed by the given pointer.
 * 
 * @param p_word pointer to the dynamically allocated arrea
 */

void word_dtor(char *p_word){
    if (p_word != NULL){
        free(p_word);
    }
}

/**
 * @brief This function initialiazes a storage_t structure and defines, what kind of data it stores.
 * 
 * @param p_stg pointer to the memory, where the sctructure is stored
 * @param type declares the kind of data, that is supposed to be stored in the storage
 */

void stg_ctor(stg *p_stg, char type){
    p_stg->array = NULL;
    p_stg->type = type;
    p_stg->length = 0;
}

/**
 * @brief This function clears all the memory dynamically allocated in the area, that is pointed by the given pointer.
 * 
 * @param array pointer to the dynamically allocated arrea of pointers to each word
 */

void stg_dtor(stg *p_stg){
    if (p_stg->array != NULL){
        free(p_stg->array);
    }
    if (p_stg != NULL && p_stg->type != 'U'){
        free(p_stg);
    }
}

/**
 * @brief This function initialiazes a command_t.
 * 
 * @param p_comm pointer to the storage of command
 */

void comm_ctor(comm *p_comm){
    p_comm->str = NULL;
}

/**
 * @brief This function clears all the memory dynamically allocated in the area designated for command, that is pointed by the given pointer.
 *
 * @param p_comm pointer to the dynamically allocated area
 */

void comm_dtor(comm *p_comm){
    if (p_comm->str != NULL){
        free(p_comm->str);
    }
    if (p_comm != NULL){
        free(p_comm);
    }
}

/**
 * @brief This function reads a single word from given line and stores it at given memory.
 * 
 * @param p_line - pointer to the line, that we want to read the word from
 * @param p_word - pointer to the memory, where we want the word to be stored
 * @param start - tells us, how many characters to skip in line
 * @return int - if no error occures, the program returns 1, value -1 means error
 */

int read_word(str *p_line, str *p_word, int start){
    int i = 0;
    char word[MAX_LENGTH + 1];
    while (p_line->str[start + i] != 32 && p_line->str[start + i] != '\0' && p_line->str[start + i] != ')' && p_line->str[start + i] != '\n' && p_line->str[start + i] != EOF){
        if (i == MAX_LENGTH){
            fprintf(stderr, "The string is too long.\nGiven string:\n%s", p_line->str);
            return -1;
        }
        else if (!((p_line->str[start + i] >= 'A' && p_line->str[start + i] <= 'Z') || (p_line->str[start + i] >= 'a' && p_line->str[start + i] <= 'z'))){
            fprintf(stderr, "Some characters in the string aren`t allowed.\nGiven string:\n%s", p_line->str);
            return -1;
        }
        
        word[i] = p_line->str[i + start];
        i++;
    }

    p_word->str = malloc(sizeof(p_word->str) + (sizeof(char) * i));
    if (p_word->str == NULL){
        fprintf(stderr, "Memory issues, the program ran out of memory.");
        return -1;
    }
    word[i] = '\0';
    strcpy(p_word->str, word);
    p_word->length = i;
    return 1;
}

/**
 * @brief This function reads a number from line
 * 
 * @param p_line pointer to the line, that we want to read from
 * @param ans pointer to where we want to store our read number
 * @param start count of how many characters, do we need to skip before reaching the arguments
 * @return int returns -1, if any error occures and 1, if everything goes right
 */

int read_num(str *p_line, int *ans, int start){
    int i = 0;
    while (p_line->str[start + i] >= '0' && p_line->str[start + i] <= '9'){
        if (i >= 4){
            fprintf(stderr, "The operand is too high limit is 999.\n");
            return -1;  
        }
        *ans = ((10 * *ans) + (p_line->str[start + i] - '0'));
        i++;
    }
    if (p_line->str[start + i] == 32 || p_line->str[start + i] == '\n' || p_line->str[start + i] == EOF){
        return i;
    }
    fprintf(stderr, "The operands have to be a number.\nerror on line:\n%s", p_line->str);
    return -1;
}

/**
 * @brief This function checks if the same word is stored in univerzum.
 * 
 * @param p_word the word we want to check
 * @param p_uni pointer to structure, where all words in univerzum are stored
 * @return char* pointer to the word we checked is returned, but if the word isn`t in univerzum NULL is returned
 */

char * is_in_uni(str *p_word, stg *p_uni){
    int i = 0;
    while (i < p_uni->length){
        if (strcmp(p_word->str, p_uni->array[i]) == 0){
            return p_uni->array[i];
        }
        i++;
    }
    return NULL;
}

/**
 * @brief This function reads a single line from a file.
 * 
 * @param p_file pointer to a file we want to read from
 * @param p_line a structure, where we want to store the line
 * @return int if the program run successfully, the return value is 1, if error occured, the value is -1
 */

int read_line(FILE *p_file, str *p_line){
    do {
        char *p_temporary;
        p_temporary = realloc(p_line->str, (sizeof(char) * (p_line->length + 2)));
        if (p_temporary == NULL){
            fprintf(stderr, "Memory issues, the program ran out of memory.");
            return -1;
        }
        p_line->str = p_temporary;
        p_line->str[p_line->length] = fgetc(p_file);
        p_line->str[p_line->length + 1] = '\0';
        p_line->length++;
    } while (p_line->str[p_line->length - 1] != '\n' && p_line->str[p_line->length - 1] != EOF);
    return 1;
}

/**
 * @brief This function seperates one word, tak is stored in line and stores it at it`s own place, for easier manipulation.
 * 
 * @param p_line pointer to where the line is stored as string
 * @param word pointer to the block of memory, where we want to store the word
 * @param progres indicates, how many characters from line have we already read, this way we don`t read any word twice
 * @return int if the program run successfully, the return value is 1, if error occured, the value is -1
 */

int seperate_word_in_line(str *p_line, str *word, int *progres){
    str_ctor(word);
    if (read_word(p_line, word, *progres) == -1){
        return -1;
    }
    if (word->length == 0){
        fprintf(stderr, "Wrong format in line:\n%s\n", p_line->str);
        return -1;
    }
    *progres += word->length + 1;
    return 1;
}

/**
 * @brief This function creates new storage to store pointers to all the words read from line indexed as univerzum.
 * 
 * @param p_line pointer to storage of the line, from which the univerzum is supposed to be created
 * @param p_uni pointer in which we want the address of the univerzum to be stored in
 * @return int if the program run successfully, the return value is 1, if error occured, the value is -1
 */

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
                word_dtor(word.str);
                fprintf(stderr, "Invalid input, you entered a forbidden word");
                return -1;
            }
            j++;
        }
        char **p_temporary;
        p_temporary = realloc(p_uni->array, sizeof(p_uni->array) * (p_uni->length + 1) );
        if (p_temporary == NULL){
            word_dtor(word.str);
            fprintf(stderr, "Memory issues, the program ran out of memory.");
            return -1;
        }
        p_uni->array = p_temporary;
        p_uni->array[i] = word.str;
        p_uni->length++;
        i++;
    }
    return 1;
}

/**
 * @brief This function stores pointers to all the words read from line indexed as set.
 * 
 * @param p_line pointer to storage of the line, from which the set is supposed to be created
 * @param p_set pointer in which we want to save the address of created set
 * @param p_uni pointer to storage of univerzum, that we need to compare the read word with words in univerzum
 * @return int if the program run successfully, the return value is 1, if error occured, the value is -1
 */

int make_set(str *p_line, stg *p_set, stg*p_uni){
    int progres = 2;
    int i = 0;
    str word;
    while (progres < p_line->length){
        if (seperate_word_in_line(p_line, &word, &progres) == -1){
            return -1;
        }
        char **p_temporary;
        p_temporary = realloc(p_set->array, sizeof(p_set->array) * (p_set->length + 1));
        if (p_temporary == NULL){
            word_dtor(word.str);
            fprintf(stderr, "Memory issues, the program ran out of memory.");
            return -1;
        }
        p_set->array = p_temporary;
        p_set->array[i] = is_in_uni(&word, p_uni);
        if (p_set->array[i] == NULL){
            word_dtor(word.str);
            fprintf(stderr, "The word '%s' isn`t located in the univerzum.", word.str);
            return -1;
        }
        word_dtor(word.str);
        p_set->length++;
        i++;
    }
    return 1;
}

/**
 * @brief This function stores pointers to all the words read from line indexed as relation. The domain and range of relation, are stored one after each other.
 * 
 * @param p_line pointer to storage of the line, from which the relations are supposed to be read
 * @param p_rel pointer in which we want to save the address of created relation
 * @param p_uni pointer to storage of univerzum, that we need to compare the read word with words in univerzum
 * @return int if the program run successfully, the return value is 1, if error occured, the value is -1
 */

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
            progres++;
            char **p_temporary;
            p_temporary = realloc(p_rel->array, sizeof(p_rel->array) * (p_rel->length + 2));
            if (p_temporary == NULL){
                word_dtor(word1.str);
                word_dtor(word2.str);
                fprintf(stderr, "Memory issues, the program ran out of memory.");
                return -1;
            }
            p_rel->array = p_temporary;
            p_rel->array[i] = is_in_uni(&word1, p_uni);
            p_rel->array[i + 1] = is_in_uni(&word2, p_uni);
            if ((p_rel->array[i] == NULL) || (p_rel->array[i + 1] == NULL)){
                word_dtor(word1.str);
                word_dtor(word2.str);
                fprintf(stderr, "The relation ('%s' '%s') isn`t located in the univerzum.", word1.str, word2.str);
                return -1;
            }
            word_dtor(word1.str);
            word_dtor(word2.str);
            p_rel->length += 2;
            i += 2;
        }
        else {
            fprintf(stderr, "Invalid formulation of relation, accepted format is (domain range)!!");
            return -1;
        }
    }
    return 1;
}

/**
 * @brief This functions reads arguments for command from a line.
 * 
 * @param p_line pointer to the line, that we want to read from
 * @param p_comm_stg pointer to the storage for the command
 * @param name_length length of the name of the command, to know how many characters to skip
 * @param limit the maximum ammount of arguments, that we need for the command
 * @return int returns -1, if any error occures and 1, if everything goes right
 */

int comm_args(str *p_line, comm *p_comm_stg, int name_length, int limit){
    int count_read_chars = 4 + name_length;
    int i = 0;
    while (i < limit){
        count_read_chars += 1 + read_num(p_line, &(p_comm_stg->operands[i]), count_read_chars);
        i++;
    }
    if (p_line->str[count_read_chars] != '\n' || p_line->str[count_read_chars] != EOF){
        fprintf(stderr, "Too many arguments after command.");
        return -1;
    }
    return 1;
}

/**
 * @brief This function, reads a command ilne a checks, if its valid command, then stores it with its arguments.
 * 
 * @param p_line pointer to a line from which we want to read the command
 * @param p_comm_stg pointer to a storage for the command data
 * @return int returns -1, if any error occures and 1, if everything goes right
 */

int make_comm(str *p_line, comm *p_comm_stg){
    str cmnd;
    int i = 0;
    read_word(p_line, &cmnd, 2);
    strcpy(p_comm_stg->str, cmnd.str);
    while(i < 10){
        if (strcmp(cmnd.str, single_arg_comm[i]) == 0){
            if (comm_args(p_line, p_comm_stg, cmnd.length, 1) == -1){
                return -1;
            }
            return 1;
        }
        i++;
    }

    i = 0;
    while(i < 6){
        if (strcmp(cmnd.str, double_arg_comm[i]) == 0){
            if (comm_args(p_line, p_comm_stg, cmnd.length, 2) == -1){
                return -1;
            }
            return 1;
        }
        i++;
    }

    i = 0;
    while(i < 3){
        if (strcmp(cmnd.str, triple_arg_comm[i]) == 0){
            if (comm_args(p_line, p_comm_stg, cmnd.length, 3) == -1){
                return -1;
            }
            return 1;
        }
        i++;
    }
    fprintf(stderr, "The name '%s' of command was not recognized.", cmnd.str);
    return -1;
}

/**
 * @brief This functions decides, what data is on line and stores it in designated storage.
 * 
 * @param p_file pointer to a file, that we want to read from
 * @param p_line pointer to memory where the line, is going to be stored 
 * @param p_uni pointer to where the univerzum is stored
 * @param p_stg pointer to storage, where we want data from each new line to be stored at
 * @param p_comm pointer to storage, where we want a command line output to be stored
 * @param lines_count a counter of lines to track the size of array, where it`s stored
 * @return int if the program run successfully, the return value is 1, if error occured, the value is -1, if we reach end of file, the program returns 0
 */

int store_line(FILE *p_file, stg *p_uni, stg *p_stg, comm *p_comm, int lines_count){
    str line;
    stg_ctor(p_stg, '\0');
    comm_ctor(p_comm);
    str_ctor(&line);
    if (read_line(p_file, &line) == -1){
        return -1;
    }

    if (line.str[0] == 'S' && lines_count != 0 && line.str[1] == 32){
        stg_ctor(p_stg, 'S');
        if (make_set(&line, p_stg, p_uni) == -1){
            word_dtor(line.str);
            stg_dtor(p_stg);
            return -1;
        }
    }

    else if (line.str[0] == 'R' && lines_count != 0 && line.str[1] == 32){
        stg_ctor(p_stg, 'R');
        if (make_rel(&line, p_stg, p_uni) == -1){
            word_dtor(line.str);
            stg_dtor(p_stg);
            return -1;
        }
    }

    else if (line.str[0] == 'C' && lines_count != 0 && line.str[1] == 32){
        comm_ctor(p_comm);
        if (make_comm(&line, p_comm) == -1){
            word_dtor(line.str);
            comm_dtor(p_comm);
        }
    }

    else if (line.str[0] == 'U' && lines_count == 0 && line.str[1] == 32){
        stg_ctor(p_uni, 'U');
        if (make_uni(&line, p_uni) == -1){
            word_dtor(line.str);
            return -1;
        }
    }

    else{
        fprintf(stderr, "Invalid format, after declaration of type of the line must come space.");
        return -1;
    }
    
    if (line.str[line.length - 1] == EOF){ //if the last character in line is EOF, we return 0, so the program know, we reached the end
        return 0;
    }
    word_dtor(line.str);
    return 1;
}

/**
 * @brief This function takes care of freeing the data stored in dynamically allocated memory.
 *
 * @param p_uni pointer to univerzum, in which we have stored pointers to all the words, that need to be freed
 * @param lines pointer to a list of pointers, which point to a memory, where we store all the read lines, that needs to be freed
 * @param lines_count count of how many lines we have read
 */

void clear_memory(stg *p_uni, stg **lines, comm **commands, int lines_count, int comms_count){
    int i = 0;
    while (i < p_uni->length){
        word_dtor(p_uni->array[i]);
        i++;
    }
    stg_dtor(p_uni);
    i = 0;
    while (i < lines_count){
        stg_dtor(lines[i]);
        i++;
    }
    i = 0;
    while (i < comms_count){
        comm_dtor(commands[i]);
        i++;
    }
}

/**
 * @brief This functions reads the whole file, line after line and stores each line in designated storage.
 * 
 * @param p_file pointer to oppened file, that is supposed to be read
 * @param lines pointer to a list of pointers, that point to the storage of each line                  *TYHLE 2 VECI SE NEJSPIS JESTE BUDOU NEJAK MENIT
 * @param commands pointer to a list of pointers, that point to the storage of each command line        *
 * @param uni pointer to storage of universum
 * @param lines_count pointer to count of read lines
 * @param comms_count pointer to cound of read command lines
 * @return int the value is -1, if any error occures, and 1 if the function doesnt fail 
 */

int read_file(FILE *p_file, stg ***lines, comm ***commands, stg *uni, int *lines_count, int *comms_count){
    stg *storage;
    comm *cmnd;
    stg **p_temporary;
    comm **p_temporary2;
    storage = malloc(sizeof(stg));
    cmnd = malloc(sizeof(comm));
    p_temporary = *lines;
    p_temporary2 = *commands;
    if (storage == NULL || cmnd == NULL){
        fprintf(stderr, "Memory issues, the program ran out of memory.");
        return -1;
    }
    int check; //this variable is used to store the outcome of function, that reads a single line
    do {
        check = store_line(p_file, uni, storage, cmnd, *lines_count);
        if (check == -1){
            clear_memory(uni, *lines, *commands, *lines_count, *comms_count);
            return -1;
        }
        if (storage->array != NULL){
            p_temporary = realloc(*lines, sizeof(**lines) * (*lines_count));
            if ((p_temporary == NULL)){
                stg_dtor(storage);
                clear_memory(uni, *lines, *commands, *lines_count, *comms_count);
                fprintf(stderr, "Memory issues, the program ran out of memory.");
                return -1;
            }
        }
        else if (cmnd->str != NULL){
            p_temporary2 = realloc(*commands, sizeof(**commands) * (*comms_count + 1));
            if ((p_temporary2 == NULL)){
                comm_dtor(cmnd);
                clear_memory(uni, *lines, *commands, *lines_count, *comms_count);
                fprintf(stderr, "Memory issues, the program ran out of memory.");
                return -1;
            }
        }
        else {
            *lines_count += 1;
            continue;
        }
        *lines = p_temporary;
        *commands = p_temporary2;
        if (storage->array != NULL){
            *lines[*lines_count - 1] = storage;
            storage = malloc(sizeof(stg));
            *lines_count += 1;
        }
        else if (cmnd->str != NULL){
            *commands[*comms_count] = cmnd;
            cmnd = malloc(sizeof(comm));
            *comms_count += 1;
        }
        if (storage == NULL || cmnd == NULL){
            fprintf(stderr, "Memory issues, the program ran out of memory.");
            return -1;
        }
        if ((*lines_count + *comms_count) > 1000){
            fprintf(stderr, "Too many lines, limit is 1000");
            return -1;
        }
    } while (check != 0);
    return 1;
}

int main(){
    FILE *p_f;
    p_f = fopen("file.txt", "r");
    if (p_f == NULL){
        return 0;
    }
    stg uni;
    stg **lines; //tohle je hlavni storage, ve kterem je kazdy radek, krom univerza.. budou v nem ulozene pointery na jednotlive radky
                //takze pristup je lines[i - 2] i = poradi radku (je to pocitane od 0, ale prvni radek ma mit index 2, takze s tim musime pocitat)
                //a to co dostaneme je pointer na strukturu STG, a pak si vybereme co potrebujeme, takze napr
                //line[i - 2]->array[j] j = je index toho kolikate slovo, line[i - 2]->length.. zjistit pocet slov na radku
                //line[i]->type zjistime o jaky druh radku se jedna
    comm **commands; //tohle struktura na ukladani commandu.. tam mame zase pointer na strukturu command
                    //takze pres commands[i]->string.. pristoupime na nazev toho commandu
                    //pomoci commands[i]->operands[j] j = min 0 max 2 si zjistime s jakymi radky mame operovat
    lines = NULL;
    commands = NULL;
    int lines_count = 0;
    int comms_count = 0;
    if (read_file(p_f, &lines, &commands, &uni, &lines_count, &comms_count) == -1){
        return -1;
    }
    int i = 0;
    int j = 0;
    while (i < lines_count){
        printf("%c| ", lines[i]->type);
        while (j < lines[i]->length){
            printf("%s ", lines[i]->array[j]);
            j++;
        }
        i++;
    }
    i = 0;
    while (i < comms_count){
        printf ("%s", commands[i]->str);
    }
    clear_memory(&uni, lines, commands, lines_count, comms_count);
    fclose(p_f);
    return 0;
}