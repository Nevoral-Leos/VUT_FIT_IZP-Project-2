/**
 * @name Projekt 2 - Praca s datovymi strukturami
 * @authors - Marek Spirka <xspirk01@stud.fit.vutbr.cz>
 *          - Michaela Novotna <xnovot2i@stud.fit.vutbr.cz>
 *          - Jakub Korbel <xkorbe03@stud.fit.vutbr.cz>
 *          - Leos Nevoral <xnevor03@stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LENGTH 30 //maximum length of the universum
#define MAX_ARG 2 //number of required arguments

//list of all used commands and forbidden words
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
char *all_comms[19] = {command1, command2, command3, command4, command5, command6, command7, command8, command9, command10, command11, command12, command13, command14, command15, command16, command17, command18, command19};
char *list_of_forbidden_elements[21] = {command1, command2, command3, command4, command5, command6, command7, command8, command9, command10, command11, command12, command13, command14, command15, command16, command17, command18, command19, forbidden_word1, forbidden_word2};

/**
 * This structure is used to store each words and lines before they get seperated in a single words.
 * The words are stored as strings and we also know it`s length for the purpose of efficiency in some functions.
 */

typedef struct string_t{
    char *str;
    int length;
} str;

/**
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
 * This structure stores all lines.
 */

typedef struct lines_t{
    stg **line;
    int count;
} lns;

//functions that open file and check arguments
int open_file(char** argv, FILE **fp);
int arg_check (int argc);
//functions that read file and store lines from it
int read_file(FILE *p_file, lns *all_lines);
int read_line(FILE *p_file, str *p_line);
int seperate_word_in_line(str *p_line, str *word, int *progres);
int read_word(str *p_line, str *p_word, int start);
int read_num(str *p_line, int *ans, int start);
int store_line(FILE *p_file, stg *p_uni, stg *p_stg, comm *p_comm, bool *read_only_command, int lines);
void print_line(stg *line);
//functions that creates structures used in program
void str_ctor(str *p_word);
void stg_ctor(stg *p_stg, char type);
void comm_ctor(comm *p_comm);
void lns_ctor(lns *all_lines);
//functions that create (read) and control the universes
int make_uni(str *p_line, stg *p_uni);
char * is_in_uni(str *p_word, stg *p_uni);
//functions that create (read) sets and control it
int make_set(str *p_line, stg *p_set, stg *p_uni);
int unique_element(stg *p_set);
//functions that create (read) and control sessions
int make_rel(str *p_line, stg *p_rel, stg *p_uni);
int unique_pair(stg *rel);
//functions that create (read) and control commands
int make_comm(str *p_line, comm *p_comm_stg);
int comm_args(str *p_line, comm *p_comm_stg, int name_length, int limit);
//functions that check the correctness of commands and break them down into commands for sessions and sets
int find_command(comm *cmnd);
int run_comm_set(comm *cmnd, lns *lines, int comm_index);
int run_comm_rel(comm *cmnd, lns *lines, int comm_index);
//functions that control commands over sets
int save_true_false(stg *ans, char result);
int is_set(stg *storage);
int empty (stg *storage, stg *ans);
int card (stg *storage, stg *ans);
int complement (stg *storage, stg *uni, stg *ans);
int make_union (stg *storage1, stg *storage2, stg *ans);
int intersect (stg *storage1, stg *storage2, stg *ans);
int minus (stg *storage1, stg *storage2, stg *ans);
int is_subset(stg *storage1, stg *storage2);
int subseteq(stg *storage1, stg *storage2, stg *ans);
int subset(stg *storage1, stg *storage2, stg *ans);
int equals(stg *storage1, stg *storage2, stg *ans);
//functions that control commands over relations
int reflexive(int uni_count, stg *storage, stg *ans);
int symmetric(stg *storage, stg *ans);
int antisymmetric(stg *storage, stg *ans);
int transitive(stg *storage, stg *ans);
int function(stg *storage, stg *ans);
int domain(stg *storage, stg *ans);
int codomain(stg *storage, stg *ans);
bool is_in_set(stg *storage1, stg *storage2, int domain_range);
int is_injective(stg *storage1, stg *storage2, stg *storage3);
int injective(stg *storage1, stg *storage2, stg *storage3, stg *ans);
int is_surjective(stg *storage1, stg *storage2, stg *storage3);
int surjective(stg *storage1, stg *storage2, stg *storage3, stg *ans);
int bijective(stg *storage1, stg *storage2, stg *storage3, stg *ans);
//destructors
void word_dtor(char *p_word);
void stg_dtor(stg *p_stg);
void comm_dtor(comm *p_comm);
void clear_memory(lns *all_lines);

int main(int argc, char **argv){
    FILE *p_f;
    p_f = NULL;
    lns all_lines;
    int i = 0;
    if (arg_check(argc) == -1){
    }
    else open_file(argv, &p_f);
    if (p_f == NULL){
        return -1;
    }
    lns_ctor(&all_lines);
    if (read_file(p_f, &all_lines) == -1){
        fclose(p_f);
        return -1;
    }
    while (i < all_lines.count){
        print_line(all_lines.line[i]);
        i++;
    }
    clear_memory(&all_lines);
    fclose(p_f);
    return 0;
}

/**
 * @brief this function checks, if the given input file is readable
 * @param argv pointer to pointer of the string, where the name of file is stored
 * @param fp pointer to where we want to store the pointer to accessed file
 * @return int -1 on error and 1 on succesfull run through
 */

int open_file(char **argv, FILE **fp){
    *fp = fopen(argv[1], "r");
        if (*fp == NULL){
            fprintf(stderr,"File not found!\n");
            return -1;
        }    
    return 1;
}

/**
 * @brief checks if the input to the program was correct
 * @param argc the ammount of arguments
 * @return int -1 on error and 1 on succesfull run through
 */

int arg_check (int argc){
    if (argc != MAX_ARG){
        fprintf(stderr,"Invalid count of arguments!\n");
        return -1;
    }
    return 1;
}
/**
 * @brief This function reads the whole file.
 * @param p_file pointer to oppened file, that is supposed to be read
 * @param all_lines pointer to a structure of pointers, that point to the storage of each line
 * @return int -1 on error and 1 on succesfull run through
 * check - this variable is used to store the outcome of function, that reads a single line
 * check - this variable is used to indicate, if we read a command.. if so, then we can`t read anything else than command
 */

int read_file(FILE *p_file, lns *all_lines){
    comm command;
    stg **p_temp;
    int check;
    bool read_only_comms = false;
    do {
        p_temp = realloc(all_lines->line, sizeof(stg *) * (all_lines->count + 1));
        if (p_temp == NULL){
            clear_memory(all_lines);
            fprintf(stderr, "Memory issues, the program ran out of memory\n.");
            return -1;
        }
        all_lines->line = p_temp;
        all_lines->line[all_lines->count] = malloc(sizeof(stg));
        if (all_lines->line[all_lines->count] == NULL){
            clear_memory(all_lines);
            fprintf(stderr, "Memory issues, the program ran out of memory\n.");
            return -1;
        }

        check = store_line(p_file, all_lines->line[0], all_lines->line[all_lines->count], &command, &read_only_comms, all_lines->count);
        if (check == 2){
            return 1;
        }
        if (check == -1){
            clear_memory(all_lines);
            return -1;
        }
        if (command.str != NULL){ //when we get any command name, we run it
            int i = 0;
            int max_operands = 3;
            int set_comm_count = 9;
            int rel_comm_count = 10;
            while (i < max_operands){
                if (command.operands[i] >= all_lines->count){ //when the operand of command is higher than count of lines, we can`t execute it
                    fprintf(stderr, "Invalid command argument!\n");
                    return -1;
                }
                i++;
            }
            int comm_index = find_command(&command); 
            if (comm_index == -1){
                return -1;
            }
            else if (comm_index < set_comm_count){
                if (run_comm_set(&command, all_lines, comm_index) == -1){
                    return -1;
                }
            }
            else if (comm_index < (set_comm_count + rel_comm_count) && comm_index >= set_comm_count){
                if (run_comm_rel(&command, all_lines, comm_index) == -1){
                    return -1;
                }
            }
            comm_dtor(&command);
        }

        ++all_lines->count;
        if (all_lines->count > 1000){
            fprintf(stderr, "Too many lines, limit is 1000\n");
            return -1;
        }
    } while (check != 0);
    return 1;
}

/**
 * @brief This function reads a single line from a file.
 * @param p_file pointer to a file we want to read from
 * @param p_line a structure, where we want to store the line
 * @return int if the function run successfully, the return value is 1, if error occured, the value is -1
 */

int read_line(FILE *p_file, str *p_line){
    do { //runs untill new line or end of file character is loccated
        char *p_temporary;
        p_temporary = realloc(p_line->str, (sizeof(char) * (p_line->length + 2))); //allocates place for 1 new character and \0 ->
        if (p_temporary == NULL){                //character, on first go through, it allocates 2 chars and then 1 extra everytime
            fprintf(stderr, "Memory issues, the program ran out of memory\n.");
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
 * @param p_line pointer to where the line is stored as string
 * @param word pointer to the block of memory, where we want to store the word
 * @param progres indicates, how many characters from line have we already read, this way we don`t read any word twice
 * @return int if the function run successfully, the return value is 1, if error occured, the value is -1
 */

int seperate_word_in_line(str *p_line, str *word, int *progres){
    str_ctor(word);
    if (read_word(p_line, word, *progres) == -1){
        return -1;
    }
    if (word->length > 0){
        *progres += word->length + 1; //if we read a word, we add the length of it + one space after it to progres
    }
    return 1;
}

/**
 * @brief This function reads a single word from given line and stores it at given memory.
 * @param p_line - pointer to the line, that we want to read the word from
 * @param p_word - pointer to the memory, where we want the word to be stored
 * @param start - tells us, how many characters to skip in line
 * @return int - if no error occures, the program returns 1, value -1 means error
 */

int read_word(str *p_line, str *p_word, int start){
    int i = 0;
    char word[MAX_LENGTH + 1];
    //the loop runs, until the word ends, end is indicated by space, new line, end of file or ')' character.
    while (p_line->str[start + i] != ' ' && p_line->str[start + i] != '\0' && p_line->str[start + i] != ')' && p_line->str[start + i] != '\n' && p_line->str[start + i] != EOF){
        if (i == MAX_LENGTH){
            fprintf(stderr, "The string is too long. Given string:\n%s\n", p_line->str);
            return -1;
        }
        //only character from english alphabet are allowed, no numbers or special chars
        else if (!((p_line->str[start + i] >= 'A' && p_line->str[start + i] <= 'Z') || (p_line->str[start + i] >= 'a' && p_line->str[start + i] <= 'z'))){
            fprintf(stderr, "Some characters in the string aren`t allowed. Given string:\n%s\n", p_line->str);
            return -1;
        }

        word[i] = p_line->str[i + start];
        i++;
    }

    p_word->str = malloc(sizeof(char) * (i + 1));
    if (p_word->str == NULL){
        fprintf(stderr, "Memory issues, the program ran out of memory.\n");
        return -1;
    }
    word[i] = '\0'; //makes string from the word
    strcpy(p_word->str, word); //copies given string to our allocated memory, so we don`t lose it
    p_word->length = i;
    return 1;
}

/**
 * @brief This function reads a number from line
 * @param p_line pointer to the line, that we want to read from
 * @param ans pointer to where we want to store our read number
 * @param start count of how many characters, do we need to skip before reaching the arguments
 * @return int returns -1, if any error occures and 1, if everything goes right
 * max_num_len_in_chars - if then num is 3+ chars long, it has to be over 1000, which is not allowed number of lines
 */

int read_num(str *p_line, int *ans, int start){
    int i = 0;
    int max_num_len_in_chars = 4;
    while (p_line->str[start + i] >= '0' && p_line->str[start + i] <= '9'){
        if (i >= max_num_len_in_chars){
            fprintf(stderr, "The operand is too high limit is 999.\n");
            return -1;
        }
        *ans = ((10 * *ans) + (p_line->str[start + i] - '0')); //takes the previous number times 10 and adds the new number so for example (10 * 5) + 9 -> line 59 
        i++;
    }
    if (p_line->str[start + i] == ' ' || p_line->str[start + i] == '\n' || p_line->str[start + i] == EOF){
        --*ans; //the index of line in storage is one less the number on line
        return i;
    }
    //if we dont have number, space, new line or end of file character, there must some invalid input
    fprintf(stderr, "The operands have to be a number. Error on line:\n%s", p_line->str);
    return -1;
}

/**
 * @brief This functions decides, what data is on line and stores it in designated storage.
 * @param p_file pointer to a file, that we want to read from
 * @param p_uni pointer to where the univerzum is stored
 * @param p_stg pointer to storage, where we want data from each new line to be stored at
 * @param p_comm pointer to storage, where we want a command line output to be stored
 * @param read_only_comms if set to true, the function accepts only input of type command
 * @param lines store, how many lines we have already read
 * @return int if the function run successfully, the return value is 1, if error occured, the value is -1, if we reach end of file, the program returns 0, if the first character is EOF, it returns 2
 */

int store_line(FILE *p_file, stg *p_uni, stg *p_stg, comm *p_comm, bool *read_only_comms, int lines){
    str line;
    str_ctor(&line);
    if (read_line(p_file, &line) == -1){
        return -1;
    }
    if (line.str[0] == '\n' || line.str[0] == ' '){//if the lines is empty, we just ignore it
        fprintf (stderr, "Empty lines, are not allowed");
        return -1;
    }
    if (line.str[0] == EOF){
        return 2;
    }
    int check;

    stg_ctor(p_stg, line.str[0]);
    comm_ctor(p_comm);
    if (line.str[1] == ' ' || line.str[1] == '\n' || line.str[1] == EOF){//after indication of type of line must come space or end, otherwise it`s invalid input
        if (p_stg->type == 'S' && lines > 0 && !(*read_only_comms)){
            check = make_set(&line, p_stg, p_uni);
        }
        else if (p_stg->type == 'R' && lines > 0 && !(*read_only_comms)){
            check = make_rel(&line, p_stg, p_uni);
        }
        else if (p_stg->type == 'C' && lines > 0){
            *read_only_comms = true;
            check = make_comm(&line, p_comm);
        }
        else if (p_stg->type == 'U' && lines == 0){
            check = make_uni(&line, p_uni);
        }
        else {
            fprintf(stderr, "Wrong input!! on line\n %s\n",line.str);
            word_dtor(line.str);
            return -1;
        }
    }
    else{
        fprintf(stderr, "Invalid format of line.\n %s\n",line.str);
        word_dtor(line.str);
        return -1;
    }
    if (check == -1){
        word_dtor(line.str);
        stg_dtor(p_stg);
        return -1;
    }

    if (line.str[line.length - 1] == EOF){ //if the last character in line is EOF, we return 0, so the program know, we reached the end
        word_dtor(line.str);
        return 0;
    }
    word_dtor(line.str);
    return 1;
}

/**
 * @brief This function prints given line in accepted format
 * @param line the line, that is to be prnited
 */

void print_line(stg *line){
    int i = 0;
    if (line->type == 'C'){ //in lines, that are of type C, we store true / false values and other values, that are not sets or relations
        printf("%s", line->array[0]);
    }
    if (line->type == 'S' || line->type == 'U'){
        printf("%c", line->type);
        while (i < line->length){
            printf(" %s", line->array[i]);
            i++;
        }
    }
    else if (line->type == 'R'){
        printf("%c", line->type);
        while (i < line->length){
            printf(" (%s %s)", line->array[i], line->array[i+1]);
            i += 2;
        }
    }
    printf("\n");
}

/**
 * @brief This function initialiazes a string_t storage.
 * @param p_word pointer to the memory, at which the structure is stored
 */

void str_ctor(str *p_word){
    p_word->str = NULL;
    p_word->length = 0;
}

/**
 * @brief This function initialiazes a storage_t structure and defines, what kind of data it stores.
 * @param p_stg pointer to the memory, where the sctructure is stored
 * @param type declares the kind of data, that is supposed to be stored in the storage
 */

void stg_ctor(stg *p_stg, char type){
    p_stg->array = NULL;
    p_stg->type = type;
    p_stg->length = 0;
}

/**
 * @brief This function initialiazes a command_t.
 * @param p_comm pointer to the storage of command
 */

void comm_ctor(comm *p_comm){
    p_comm->str = NULL;
    p_comm->operands[0] = 0;
    p_comm->operands[1] = 0;
    p_comm->operands[2] = 0;
}

/**
 * @brief This function initialiazes a lines_t.
 * @param all_lines pointer to the data, where it`s supposed to be initialiazed
 */

void lns_ctor(lns *all_lines){
    all_lines->line = NULL;
    all_lines->count = 0;
}

/**
 * @brief This function creates new storage to store pointers to all the words read from line indexed as univerzum.
 * @param p_line pointer to storage of the line, from which the univerzum is supposed to be created
 * @param p_uni pointer in which we want the address of the univerzum to be stored in
 * @return int if the function run successfully, the return value is 1, if error occured, the value is -1
 * progres - indicates, how many character in line were already read
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
        if (word.length == 0 && progres == 2){ //if no word was found, and we are still at beggining of line, we create empty univerzum
            word_dtor(word.str);
            return 1;
        }
        while (j < 21){
            if ((strcmp(word.str, list_of_forbidden_elements[j]) == 0)){
                word_dtor(word.str);
                fprintf(stderr, "Invalid input, you entered a forbidden word.\n");
                return -1;
            }
            j++;
        }
        char **p_temporary;
        p_temporary = realloc(p_uni->array, sizeof(p_uni->array) * (p_uni->length + 1) );
        if (p_temporary == NULL){
            word_dtor(word.str);
            fprintf(stderr, "Memory issues, the program ran out of memory.\n");
            return -1;
        }
        p_uni->array = p_temporary;
        p_uni->array[i] = word.str;
        p_uni->length++;
        i++;
    }
    if (unique_element(p_uni) == -1){
        return -1;
    }
    return 1;
}

/**
 * @brief This function checks if the same word is stored in univerzum.
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
 * @brief This function stores pointers to all the words read from line indexed as set.
 * @param p_line pointer to storage of the line, from which the set is supposed to be created
 * @param p_set pointer in which we want to save the address of created set
 * @param p_uni pointer to storage of univerzum, that we need to compare the read word with words in univerzum
 * @return int if the function run successfully, the return value is 1, if error occured, the value is -1
 * progres - indicates, how many character in line were already read
 */

int make_set(str *p_line, stg *p_set, stg *p_uni){
    int progres = 2;
    int i = 0;
    str word;
    while (progres < p_line->length){
        if (seperate_word_in_line(p_line, &word, &progres) == -1){
            return -1;
        }
        if (word.length == 0 && progres == 2){ //if no word was found, and we are still at beggining of line, we create empty set
            word_dtor(word.str);
            return 1;
        }
        char **p_temporary;
        p_temporary = realloc(p_set->array, sizeof(p_set->array) * (p_set->length + 1));
        if (p_temporary == NULL){
            word_dtor(word.str);
            fprintf(stderr, "Memory issues, the program ran out of memory.\n");
            return -1;
        }
        p_set->array = p_temporary;
        p_set->array[i] = is_in_uni(&word, p_uni);
        if (p_set->array[i] == NULL){
            word_dtor(word.str);
            fprintf(stderr, "The word isn't located in the univerzum.\n");
            return -1;
        }
        word_dtor(word.str);
        p_set->length++;
        i++;
    }

    if (unique_element(p_set) == -1){
        return -1;
    }
    return 1;
}

/**
 * @brief Checks if all ellements in a set are unique. 
 * @param p_set pointer to the set, that is to be checked
 * @return int if the function run successfully, the return value is 1, if error occured, the value is -1
 */


int unique_element(stg *p_set){
    for (int i = 0; i < p_set->length - 1; i++){
        for (int j = i + 1; j < p_set->length; j++){
            if (strcmp(p_set->array[i], p_set->array[j]) == 0){
                fprintf(stderr, "Element %s is not unique.\n", p_set->array[i]);
                return -1;
            }
        }
    }
    return 1;
}

/**
 * @brief This function stores pointers to all the words read from line indexed as relation. The domain and range of relation, are stored one after each other.
 * @param p_line pointer to storage of the line, from which the relations are supposed to be read
 * @param p_rel pointer in which we want to save the address of created relation
 * @param p_uni pointer to storage of univerzum, that we need to compare the read word with words in univerzum
 * @return int if the function run successfully, the return value is 1, if error occured, the value is -1
 */

int make_rel(str *p_line, stg *p_rel, stg *p_uni){
    int progres = 2;
    int i = 0;
    str word1;
    str word2;
    while (progres < p_line->length){
        if (p_line->str[progres] == '(' || p_line->str[progres] == '\n' || p_line->str[progres] == EOF){
            progres++;
            if ((seperate_word_in_line(p_line, &word1, &progres) == -1) || (seperate_word_in_line(p_line, &word2, &progres) == -1)){
                return -1;
            }
            if (word1.length == 0 && progres == 2){ //if no word was found, and we are still at beggining of line, we create empty relation
                word_dtor(word1.str);
                word_dtor(word2.str);
                return 1;
            }
            if (p_line->str[progres - 1] != ')'){
                fprintf(stderr, "Invalid relation");
                return -1;
            }
            progres++;
            char **p_temporary;
            p_temporary = realloc(p_rel->array, sizeof(p_rel->array) * (p_rel->length + 2));
            if (p_temporary == NULL){
                word_dtor(word1.str);
                word_dtor(word2.str);
                fprintf(stderr, "Memory issues, the program ran out of memory.\n");
                return -1;
            }
            p_rel->array = p_temporary;
            p_rel->array[i] = is_in_uni(&word1, p_uni);
            p_rel->array[i + 1] = is_in_uni(&word2, p_uni);
            if ((p_rel->array[i] == NULL) || (p_rel->array[i + 1] == NULL)){
                word_dtor(word1.str);
                word_dtor(word2.str);
                fprintf(stderr, "The relation isn't located in the univerzum.\n");
                return -1;
            }
            word_dtor(word1.str);
            word_dtor(word2.str);
            p_rel->length += 2;
            i += 2;
        }
        else {
            fprintf(stderr, "Invalid formulation of relation, accepted format is (domain range)!!\n");
            return -1;
        }
    }
    if (unique_pair(p_rel) == -1){
        return -1;
    }
    return 1;
}

/**
 * @brief this functions checks, if we dont have two same relations on one line
 * @param rel the storage of relation, that need to be checked
 * @return int if the function run successfully, the return value is 1, if error occured, the value is -1 
 */

int unique_pair(stg *rel){
    const int pair = 2;
    for (int i = 0; i < rel->length; i += pair){
        for (int j = i + pair; j < rel->length; j += pair){
            if(rel->array[i] == rel->array[j] && rel->array[i+1] == rel->array[j+1]){
                fprintf(stderr, "Invalid relation (one pair more then once).\n");
                return -1;
            }
        }
    }
    return 1;
}

/**
 * @brief This function, reads a command ilne a checks, if its valid command, then stores it with its arguments.
 * @param p_line pointer to a line from which we want to read the command
 * @param p_comm_stg pointer to a storage for the command data
 * @return int returns -1, if any error occures and 1, if everything goes right
 */

int make_comm(str *p_line, comm *p_comm_stg){
    str cmnd; //we create a string, so we could use already made function, and then we just copy it
    int i = 0;
    read_word(p_line, &cmnd, 2);
    p_comm_stg->str = malloc(sizeof(char) * cmnd.length + 1);
    if (p_comm_stg->str == NULL){
        word_dtor(cmnd.str);
        fprintf(stderr, "Memory issues, the program ran out of memory.\n");
        return -1;
    }
    strcpy(p_comm_stg->str, cmnd.str);
    word_dtor(cmnd.str);
    while(i < 10){ //the ammount of single operand commands
        if (strcmp(p_comm_stg->str, single_arg_comm[i]) == 0){
            if (comm_args(p_line, p_comm_stg, cmnd.length, 1) == -1){
                return -1;
            }
            return 1;
        }
        i++;
    }

    i = 0;
    while(i < 6){ //the ammount of double operand commands
        if (strcmp(p_comm_stg->str, double_arg_comm[i]) == 0){
            if (comm_args(p_line, p_comm_stg, cmnd.length, 2) == -1){
                return -1;
            }
            return 1;
        }
        i++;
    }

    i = 0;
    while(i < 3){ //the ammount of triple operand commands
        if (strcmp(p_comm_stg->str, triple_arg_comm[i]) == 0){
            if (comm_args(p_line, p_comm_stg, cmnd.length, 3) == -1){
                return -1;
            }
            return 1;
        }
        i++;
    }
    fprintf(stderr, "The name '%s' of command was not recognized.\n", p_comm_stg->str);
    comm_dtor(p_comm_stg);
    return -1;
}

/**
 * @brief This functions reads arguments for command from a line.
 * @param p_line pointer to the line, that we want to read from
 * @param p_comm_stg pointer to the storage for the command
 * @param name_length length of the name of the command, to know how many characters to skip
 * @param limit the maximum ammount of arguments, that we need for the command
 * @return int returns -1, if any error occures and 1, if everything goes right
 */

int comm_args(str *p_line, comm *p_comm_stg, int name_length, int limit){
    int skipped_space = 3;
    int count_read_chars = skipped_space + name_length;
    int temp;
    int i = 0;
    while (i < limit){
        if ((temp = read_num(p_line, &(p_comm_stg->operands[i]), count_read_chars)) == -1){
            return -1;
        }
        count_read_chars += 1 + temp;
        if (p_comm_stg->operands[i] == -1){
            fprintf(stderr, "Wrong argument.\n");
            return -1;
        }
        i++;
    }
    //if after last operand the line doesn`t end, it`a invalid input.. because most likely too many arguments were given
    if (p_line->str[count_read_chars-1] != EOF && p_line->str[count_read_chars-1] != '\n'){
        fprintf(stderr, "Too many arguments after command.\n");
        return -1;
    }
    return 1;
}

/**
 * @brief this function finds the index of command, that we need to run
 * @param cmnd the data storage of command, that we want to run
 * @return int returns -1, if any error occures and 1, if everything goes right
 */

int find_command(comm *cmnd){
    int i = 0;
    while (i < 20){
        if(strcmp(cmnd->str, all_comms[i]) == 0){
            return i;
        }
        i++;
    }
    fprintf(stderr, "No coresponding command name.");
    return -1;
}

/**
 * @brief This function runs commands, that are applied on sets
 * @param cmnd the command, that we need to run and get operands from
 * @param lines all lines, that it can be applied to
 * @param comm_index the index of the correct command
 * @return int returns -1, if any error occures and 1, if everything goes right
 */

int run_comm_set(comm *cmnd, lns *lines, int comm_index){
    int check;
    switch (comm_index){
        case 0:
            check = empty(lines->line[cmnd->operands[0]], lines->line[lines->count]);
            break;
        case 1:
            check = card(lines->line[cmnd->operands[0]], lines->line[lines->count]);
            break;
        case 2:
            check = complement(lines->line[cmnd->operands[0]], lines->line[0], lines->line[lines->count]);
            break;
        case 3:
            check = make_union(lines->line[cmnd->operands[0]], lines->line[cmnd->operands[1]], lines->line[lines->count]);
            break;
        case 4:
            check = intersect(lines->line[cmnd->operands[0]], lines->line[cmnd->operands[1]], lines->line[lines->count]);
            break;
        case 5:
            check = minus(lines->line[cmnd->operands[0]], lines->line[cmnd->operands[1]], lines->line[lines->count]);
            break;
        case 6:
            check = subseteq(lines->line[cmnd->operands[0]], lines->line[cmnd->operands[1]], lines->line[lines->count]);
            break;
        case 7:
            check = subset(lines->line[cmnd->operands[0]], lines->line[cmnd->operands[1]], lines->line[lines->count]);
            break;
        case 8:
            check = equals(lines->line[cmnd->operands[0]], lines->line[cmnd->operands[1]], lines->line[lines->count]);
            break;
        default:
            break;
    }
    if (check == -1){
        return -1;
    }
    return 1;
}

/**
 * @brief This function runs commands, that are applied on relations
 * @param cmnd the command, that we need to run and get operands from
 * @param lines all lines, that it can be applied to
 * @param comm_index the index of the correct command
 * @return int returns -1, if any error occures and 1, if everything goes right
 */

int run_comm_rel(comm *cmnd, lns *lines, int comm_index){
    int check;
    switch (comm_index++){
        case 9:
            check = reflexive(lines->line[0]->length ,lines->line[cmnd->operands[0]], lines->line[lines->count]);
            break;
        case 10:
            check = symmetric(lines->line[cmnd->operands[0]], lines->line[lines->count]);
            break;
        case 11:
            check = antisymmetric(lines->line[cmnd->operands[0]], lines->line[lines->count]);
            break;
        case 12:
            check = transitive(lines->line[cmnd->operands[0]], lines->line[lines->count]);
            break;
        case 13:
            check = function(lines->line[cmnd->operands[0]], lines->line[lines->count]);
            break;
        case 14:
            check = domain(lines->line[cmnd->operands[0]], lines->line[lines->count]);
            break;
        case 15:
            check = codomain(lines->line[cmnd->operands[0]], lines->line[lines->count]);
            break;
        case 16:
            check = injective(lines->line[cmnd->operands[0]], lines->line[cmnd->operands[1]], lines->line[cmnd->operands[2]], lines->line[lines->count]);
            break;
        case 17:
            check = surjective(lines->line[cmnd->operands[0]], lines->line[cmnd->operands[1]], lines->line[cmnd->operands[2]], lines->line[lines->count]);
            break;
        case 18:
            check = bijective(lines->line[cmnd->operands[0]], lines->line[cmnd->operands[1]], lines->line[cmnd->operands[2]], lines->line[lines->count]);
            break;
        default:
            break;
    }
    if (check == -1){
        return -1;
    }
    return 1;
}

//---------------------------------------functions sets ---------------------------------------//

/**
 * @brief this function saves as answer true or false
 * @param ans where we want the ans saved
 * @param result indicates, which answer we want to store
 * @return int returns -1, if any error occures and 1, if everything goes right
 */

int save_true_false(stg *ans, char result){
    char *word;
    int length;
    length = (result == 't') ? 5 : 6;
    word = malloc(sizeof(char) * length);
    ans->array = malloc(sizeof(char*));
    if (word == NULL || ans->array == NULL){
        fprintf(stderr, "Memory issues, the program ran out of memory.");
        return -1;
    }
    if (result == 't'){
        word[0] = 't'; word[1] = 'r'; word[2] = 'u'; word[3] = 'e'; word[4] = '\0';
    }
    else {
        word[0] = 'f'; word[1] = 'a'; word[2] = 'l'; word[3] = 's'; word[4] = 'e'; word[5] = '\0';
    }
    ans->array[0] = word;
    ans->length = 1;
    return 1;
}

/**
 *@brief This function finds out if the line is a set.
 *@param storage poiner to a structure where the line is stored
 *@return if there is a set on a line it returns 1 other way it returns -1
 **/

int is_set(stg *storage){
    if (!(storage->type == 'S' || storage->type == 'U'))
    {
        fprintf (stderr, "This command runs only on set!\n");
        return -1;
    }
    return 1;
}

/**
 * @brief This function finds out if there are any elements in the set. If there are any, then it prints true, other way it prints false.
 * @param storage poiner to a structure where the line is stored
 * @param ans pointer to a structure where the output of this function is stored
 * @return it returns -1 when the function wasn't done correctly, if it was done correctly it returns 1
 **/

int empty (stg *storage, stg *ans){
    if (is_set(storage) == -1){
        return -1;
    }
    if (storage->length == 0){
        if (save_true_false(ans, 't') == -1){
            return -1;
        }
    }
    else {
        if (save_true_false(ans, 'f') == -1){
            return -1;
        }
    }
    return 1;
}

/**
 * @brief This function prints number of elements.
 * @param storage poiner to a structure where the line is stored
 * @param ans pointer to a structure where the output of this function is stored
 * @return it returns -1 when the function wasn't done correctly, if it was done correctly it returns 1
 **/

int card (stg *storage, stg *ans){
    if (is_set(storage) == -1){
        return -1;
    }
    char *word;
    word = malloc(sizeof(char) * 5);
    ans->array = malloc(sizeof(char*));
    if (word == NULL || ans->array == NULL){
        fprintf(stderr, "Memory issues, the program ran out of memory.");
        return -1;
    }
    sprintf(word, "%d", storage->length);
    ans->array[0] = word;
    ans->type = 'C';
    ans->length = 1;
    return 1;
}

/**
 * @brief This function prints elements that are in universum, but they aren't in the set.
 * @param storage poiner to a structure where the line is stored
 * @param uni pointer to a stucture where the universum is stored
 * @param ans pointer to a structure where the output of this function is stored
 * @return it returns -1 when the function wasn't done correctly, if it was done correctly it returns 1
 * counter - it counts elements that belongs to complement
 * comp_tmp - it's used to check if reallocation of the memory to the words in answer went succesfully
 * found - if the element is in the set and in the universum, it's true and if not, the memory is reallocated and the element is added to complement
 **/

int complement (stg *storage, stg *uni, stg *ans) {
    if (is_set(storage) == -1){
        return -1;
    }
    int counter = 0;
    char **comp_tmp;
    bool found = false;

    for(int i = 0; i<uni->length; i++){
        for(int j = 0; j<storage->length; j++){
            if(uni->array[i] == storage->array[j]){
                found = true;
                break;
            }
        }
        if(!(found)){
            counter++;
            comp_tmp = realloc(ans->array, sizeof(char*)*counter);
            if (comp_tmp == NULL){
                fprintf(stderr,"Memory error\n");
                return -1;
            }
            ans->array = comp_tmp;
            ans->array[counter-1] = uni->array[i];
        }
        found = false;
    }
    ans->type = 'S';
    ans->length = counter;
    return 1;
}

/**
 * @brief This function prints all elements from both sets.
 * @param storage poiner to a structure where the line is stored
 * @param ans pointer to a structure where the output of this function is stored
 * @return it returns -1 when the function wasn't done correctly, if it was done correctly it returns 1
 * counter - it counts elements that belongs to union
 * union_tmp - it's used to reallocate the memory for elements of union
 * found - if the element is already in union and in the set as well, it's true and if not, the memory is reallocated and the element is added to union
 **/

int make_union (stg *storage1, stg *storage2, stg *ans){
    if (is_set(storage1) == -1 || is_set(storage2) == -1){
        return -1;
    }

    int counter = storage1->length;
    char **union_tmp;
    bool found = false;

    ans->array = malloc(sizeof(char*)*counter);
    if (ans->array == NULL){
        fprintf(stderr,"Memory error\n");
        return -1;
    }
    for(int i = 0; i<storage1->length; i++){
        ans->array[i] = storage1->array[i];
    }

    for(int i = 0; i<storage2->length; i++){
        for(int j = 0; j<counter;j++){
            if(storage2->array[i]==ans->array[j]){
                found = true;
                break;
            }
        }
        if(!(found)){
            counter++;
            union_tmp = realloc(ans->array, sizeof(char*)*counter);
            if (union_tmp == NULL){
                fprintf(stderr,"Memory error\n");
                return -1;
            }
            ans->array = union_tmp;
            ans->array[counter-1] = storage2->array[i];
        }
        found = false;
    }
    ans->type = 'S';
    ans->length = counter;
    return 1;
}

/**
 * @brief This function prints elements intersect of two sets.
 * @param storage1 poiner to a structure where the line is stored
 * @param storage2 poiner to a structure where the line is stored
 * @param ans pointer to a structure where the output of this function is stored
 * @return it returns -1 when the function wasn't done correctly, if it was done correctly it returns 1
 * counter - it counts elements that belongs to intersect
 * intersect_tmp - this pointer is used to reallocate the memory where the intersect is stored
 **/

int intersect (stg *storage1, stg *storage2, stg *ans){
    if (is_set(storage1) == -1 || is_set(storage2) == -1){
        return - 1;
    }

    int counter = 0;
    char **intersect_tmp;

    for(int i = 0; i<storage1->length; i++){
        for(int j = 0; j<storage2->length; j++){
            if(storage1->array[i] == storage2->array[j]){
                counter++;
                intersect_tmp = realloc(ans->array, sizeof(char*)*counter);
                if(intersect_tmp == NULL){
                    fprintf(stderr,"Memory error\n");
                    return -1;
                }
                ans->array = intersect_tmp;
                ans->array[counter-1] = storage1->array[i];
                break;
            }
        }
    }
    ans->type = 'S';
    ans->length = counter;
    return 1;
}

/**
 * @brief This function prints elements that are in the first set but aren't in the second set.
 * @param storage1 poiner to a structure where the line is stored
 * @param storage2 poiner to a structure where the line is stored
 * @param ans pointer to a structure where the output of this function is stored
 * @return it returns -1 when the function wasn't done correctly, if it was done correctly it returns 1
 * counter - it counts elements that are in the first set but aren't in the second set
 * minus_tmp - it's used to reallocate the memory for elements 
 * found - if the element is in both sets than it's true, else it is false
 **/

int minus (stg *storage1, stg *storage2, stg *ans){
    if (is_set(storage1) == -1 || is_set(storage2) == -1){
        return -1;
    }

    bool found = false;
    int counter = 0;
    char **minus_tmp;

    for(int i = 0; i < storage1->length; i++){
        for(int j = 0; j < storage2->length; j++){
            if(storage1->array[i] == storage2->array[j]){
                found = true;
                break;
            }
        }
        if(!(found)){
            counter++;
            minus_tmp = realloc(ans->array, sizeof(char*)*counter);
            if(minus_tmp == NULL){
                fprintf(stderr, "Memory error\n");
                return -1;
            }
            ans->array = minus_tmp;
            ans->array[counter-1] = storage1->array[i];
            found = false;
        }
    }
    ans->type = 'S';
    ans->length = counter;
    return 1;
}

/**
 * @brief This function if the second set is subset of the first set.
 * @param storage1 poiner to a structure where the line is stored
 * @param storage2 poiner to a structure where the line is stored
 * @return if both sets are equal, it returns 2, if the second set is subset of the first one, but they are not equal, it returns 1, and if the second set is not subset of the first set it returns 0
 * counter - it counts how many elements are in both sets
 **/

int is_subset(stg *storage1, stg *storage2){ //zjistuje jestli je mnozina A podmnozinou B
    if (is_set(storage1)==-1 || is_set(storage2)==-1){
        return -1;
    }

    int counter = 0;
    for (int i = 0; i<storage1->length; i++){
        for (int j = 0; j<storage2->length; j++){
            if (storage1->array[i]==storage2->array[j]){
                counter++;
            }
        }
    }
    if (counter == storage1->length && counter == storage2->length){
        return 2;
    }
    if (counter == storage1->length){
        return 1;
    }
    return 0;
}

/**
 * @brief This function prints true if both sets are equal or if the second set is subset of the first one, other way it prints false.
 * @param storage1 poiner to a structure where the line is stored
 * @param storage2 poiner to a structure where the line is stored
 * @param ans pointer to a structure where the output of this function is stored
 * @return it returns -1 when the function wasn't done correctly, if it was done correctly it returns 1
 **/

int subseteq(stg *storage1, stg *storage2, stg *ans){
    int subeq = is_subset(storage1, storage2);
    if (subeq == -1){
        return -1;
    }
    if (subeq == 1 || subeq == 2){
        if (save_true_false(ans, 't') == -1){
            return -1;
        }
    }
    else {
        if (save_true_false(ans, 'f') == -1){
            return -1;
        }
    }
    return 1;
}

/**
 * @brief This function prints true if the second set is subset of the first one, but they are not equal, other way it prints false.
 * @param storage1 poiner to a structure where the line is stored
 * @param storage2 poiner to a structure where the line is stored
 * @param ans pointer to a structure where the output of this function is stored
 * @return it returns -1 when the function wasn't done correctly, if it was done correctly it returns 1
 **/

int subset(stg *storage1, stg *storage2, stg *ans){
    int sub = is_subset(storage1, storage2);
    if (sub == -1){
        return -1;
    }
    else if (sub == 1){
        if (save_true_false(ans, 't') == -1){
            return -1;
        }
    }
    else {
        if (save_true_false(ans, 'f') == -1){
            return -1;
        }
    }
    return 1;
}

/**
 * @brief This function prints true if both sets are equal, other way it prints false.
 * @param storage1 poiner to a structure where the line is stored
 * @param storage2 poiner to a structure where the line is stored
 * @param ans pointer to a structure where the output of this function is stored
 * @return it returns -1 when the function wasn't done correctly, if it was done correctly it returns 1
 **/

int equals(stg *storage1, stg *storage2, stg *ans){
    int eq = is_subset(storage1, storage2);
    if(eq == -1){
        return -1;
    }
    else if (eq == 2){
        if (save_true_false(ans, 't') == -1){
            return -1;
        }
    }
    else {
        if (save_true_false(ans, 'f') == -1){
            return -1;
        }
    }
    return 1;
}

//---------------------------------------functions relations---------------------------------------//

/**
 * @brief This function prints true if the relation is reflexive, else it prints false
 * @param storage poiner to a structure where the line is stored
 * @param uni_count number of elements in universum
 * @param ans pointer to a structure where the output of this function is stored
 * @return it returns -1 when the function wasn't done correctly, if it was done correctly it returns 1
 * ref_count - it counts how many pairs have both elements same
 **/

int reflexive(int uni_count, stg *storage, stg *ans){
    if(storage->type != 'R'){
        fprintf(stderr, "Command can't work with a line, that's not a relation.\n");
        return -1;
    }
    int ref_count = 0;
    for(int i = 0; i<storage->length; i+=2){
        if(storage->array[i] == storage->array[i+1]){
            ref_count++;
        }
    }
    if(ref_count == uni_count){
        if (save_true_false(ans, 't') == -1){
            return -1;
        }
    }else {
        if (save_true_false(ans, 'f') == -1){
            return -1;
        }
    }
    return 1;
}

/**
 * @brief This function prints true if the relation is symmetric, else it prints false
 * @param storage poiner to a structure where the line is stored
 * @param ans pointer to a structure where the output of this function is stored
 * @return it returns -1 when the function wasn't done correctly, if it was done correctly it returns 1
 * pair_count - it counts how many pairs are in relation
 * sym_count - it counts how many symmetric pairs are in relation
 **/

int symmetric(stg *storage, stg *ans){
    if(storage->type != 'R'){
        fprintf(stderr, "Command can't work with a line, that's not a relation.\n");
        return -1;
    }
    int pair_count = 0;
    int sym_count = 0;
    for(int i = 0; i<storage->length; i+=2){
        pair_count++;
        for(int j = 0; j<storage->length; j+=2){
            if(storage->array[j] == storage->array[i+1] && storage->array[j+1] == storage->array[i]){
                sym_count++;
            }
        }
    }
    if(pair_count == sym_count){
        if (save_true_false(ans, 't') == -1){
            return -1;
        }
    } else {
        if (save_true_false(ans, 'f') == -1){
            return -1;
        }
    }
    return 1;
}

/**
 * @brief This function prints true if the relation is antisymmetric, else it prints false
 * @param storage poiner to a structure where the line is stored
 * @param ans pointer to a structure where the output of this function is stored
 * @return it returns -1 when the function wasn't done correctly, if it was done correctly it returns 1
 **/

int antisymmetric(stg *storage, stg *ans){
    if(storage->type != 'R'){
        fprintf(stderr, "Command can't work with a line, that's not a relation.\n");
        return -1;
    }
    for(int i = 0; i < storage->length; i+=2){
        if(storage->array[i] == storage->array[i+1]){
            continue;
        }
        for(int j = 0; j < storage->length; j+=2){
            if(storage->array[i] == storage->array[j+1] && storage->array[i+1] == storage->array[j]){
                if (save_true_false(ans, 'f') == -1){
                    return -1;
                }
                return 1;
            }
        }
    }
    if (save_true_false(ans, 't') == -1){
        return -1;
    }
    return 1;
}

/**
 * @brief This function prints true if the relation is transitive, else it prints false
 * @param storage poiner to a structure where the line is stored
 * @param ans pointer to a structure where the output of this function is stored
 * @return it returns -1 when the function wasn't done correctly, if it was done correctly it returns 1
 **/

int transitive(stg *storage, stg *ans){
    if(storage->type != 'R'){
        fprintf(stderr, "Command can't work with a line, that's not a relation.\n");
        return -1;
    }
    bool found = false;
    for(int i = 0; i < storage->length; i+=2){
        for(int j = 0; j < storage->length; j+=2){
            if(storage->array[i+1]==storage->array[j]){
                found = false;
                for(int k = 0; k < storage->length; k+=2){
                    if(storage->array[i]==storage->array[k] && storage->array[j+1]==storage->array[k+1]){
                        found = true;
                    }
                }
                if (!found){
                    if (save_true_false(ans, 'f') == -1){
                        return -1;
                    }
                    return 1;
                }
            }
        }
    }
    if (save_true_false(ans, 't') == -1){
        return -1;
    }
    return 1;
}

/**
 * @brief This function prints true if the relation is function, else it prints false
 * @param storage poiner to a structure where the line is stored
 * @param ans pointer to a structure where the output of this function is stored
 * @return it returns -1 when the function wasn't done correctly, if it was done correctly it returns 1
 **/

int function(stg *storage, stg *ans){
    if(storage->type != 'R'){
        fprintf(stderr, "Command can't work with a line, that's not a relation.\n");
        return -1;
    }
    for(int i = 0; i < storage->length; i+=2){
        for(int j = 0; j < storage->length; j+=2){
            if(storage->array[i] == storage->array[j]){
                if(storage->array[i+1] != storage->array[j+1]){
                    if (save_true_false(ans, 'f') == -1){
                        return -1;
                    }
                    return 1;
                }
            }
        }
    }
    if (save_true_false(ans, 't') == -1){
        return -1;
    }
    return 1;
}

/**
 * @brief This function prints elements of domain (first element of pair). 
 * @param storage poiner to a structure where the line is stored
 * @param ans pointer to a structure where the output of this function is stored
 * @return it returns -1 when the function wasn't done correctly, if it was done correctly it returns 1
 * counter - it counts elements of domain
 * dom_tmp - pointer that  is used to reallocate memory for domain
 * found - if the first element of pair, and element that is already stored in domain, are the same, it's true, else it's false and the first element of pair is added to domain
 **/

int domain(stg *storage, stg *ans){
    int counter = 0;
    bool found = true;
    char **dom_tmp;

    if(storage->type != 'R'){
        fprintf(stderr, "Command can't work with a line, that's not a relation.\n");
        return -1;
    }
    for(int i = 0; i<storage->length; i+=2){
        found = true;
        for(int j = 0; j<counter; j++){
            if(storage->array[i] == ans->array[j]){
                found = false;
                break;
            }
        }
        if(found){
            counter++;
            dom_tmp = realloc(ans->array, counter*sizeof(char*));
            if(dom_tmp==NULL){
                fprintf(stderr, "Memory error\n");
                return -1;
            }
            ans->array = dom_tmp;
            ans->array[counter-1] = storage->array[i];
        }
    }
    ans->type = 'S';
    ans->length = counter;
    return 1;
}

/**
 * @brief This function prints elements of codomain (second element of pair). 
 * @param storage poiner to a structure where the line is stored
 * @param ans pointer to a structure where the output of this function is stored
 * @return it returns -1 when the function wasn't done correctly, if it was done correctly it returns 1
 * counter - it counts elements of codomain
 * codomain_tmp - pointer that  is used to reallocate memory for codomain
 * found - if the second element of pair, and element that is already stored in domain, are the same, it's true, else it's false and the second element of pair is added to codomain
 **/

int codomain(stg *storage, stg *ans){ 
    int counter = 0;
    bool found = true;
    char **codomain_tmp;
    if(storage->type != 'R'){
        fprintf(stderr, "Command can't work with a line, that's not a relation.\n");
        return -1;
    }
    for(int i = 0; i<storage->length; i+=2){
        found = true;
        for(int j = 0; j<counter; j++){
            if(storage->array[i+1] == ans->array[j]){
                found = false;
                break;
            }
        }
        if(found){
            counter++;
            codomain_tmp = realloc(ans->array,counter*sizeof(char*));
            if(codomain_tmp==NULL){
                fprintf(stderr, "Memory error\n");
                return -1;
            }
            ans->array = codomain_tmp;
            ans->array[counter-1] = storage->array[i + 1];
        }
    }
    ans->type = 'S';
    ans->length = counter;
    return 1;
}

/**
 * @brief This function finds out if the elements of pair of relation is from set.
 * @param storage1 poiner to a structure where the line is stored (it should be relation)
 * @param storage2 poiner to a structure where the line is stored (it shoul be set)
 * @return function returns true, if all elements of pair of relaion are from set, other way it returns false
 * counter - if the element of pair is from set, it is incremented by one
 **/

bool is_in_set(stg *storage1, stg *storage2, int domain_range){
    int counter = 0;
    for(int i = 0; i<storage1->length; i+=2){
        for(int j = 0; j<storage2->length; j++){
            if(strcmp(storage1->array[i+domain_range], storage2->array[j]) == 0){
                counter++;
                break;
            }
        }
    }
    if (counter == (storage1->length / 2)){
        return true;
    }
    return false;

}

/**
 * @brief This function finds out if the relation is injective (the first element of pair can't have two, or more, different second elements)
 * @param storage1 poiner to a structure where the line is stored (it should be relation)
 * @param storage2 poiner to a structure where the line is stored (it shoul be set)
 * @param storage3 poiner to a structure where the line is stored (it shoul be set)
 * @return function returns 0, if it's not injective, and it returns 1, if it is injective, it returns -1, if the line is wrong type
 **/

int is_injective(stg *storage1, stg *storage2, stg *storage3){
    
    if(!(storage1->type == 'R') || !(storage2->type == 'S') || !(storage3->type == 'S')){
        fprintf(stderr, "Invalid command\n");
        return -1;
    }
    if (!(is_in_set(storage1, storage2, 0)) || !(is_in_set(storage1, storage3, 1))){
        return 0;
    }
    for (int i = 0; i < storage1->length; i+=2){
        for (int j = 0; j < storage1->length; j+=2){
            if(i==j){
                continue;
            }
            if (storage1->array[i+1] == storage1->array[j+1]){
                return 0;
            }
        }
    }
    return 1;
}

/**
 * @brief It prints true if the relation is injective, and false if it's not
 * @param storage1 poiner to a structure where the line is stored (it should be relation)
 * @param storage2 poiner to a structure where the line is stored (it shoul be set)
 * @param storage3 poiner to a structure where the line is stored (it shoul be set)
 * @param ans pointer to a structure where the output of this function is stored
 * @return it returns -1 when the function wasn't done correctly, if it was done correctly it returns 1
 **/

int injective(stg *storage1, stg *storage2, stg *storage3, stg *ans){
    int inj = is_injective(storage1, storage2, storage3);

    if (inj==-1){
        return -1;
    }
    if (inj==0){
        if (save_true_false(ans, 'f') == -1){
            return -1;
        }
    } else if (inj==1){
        if (save_true_false(ans, 't') == -1){
            return -1;
        }
    }
    return 1;
}

/**
 * @brief This function finds out if the relation is surjective (every element of the second set have to be used)
 * @param storage1 poiner to a structure where the line is stored (it should be relation)
 * @param storage2 poiner to a structure where the line is stored (it shoul be set)
 * @param storage3 poiner to a structure where the line is stored (it shoul be set)
 * @return function returns 0, if it's not surjective, and it returns 1, if it is surjective, it returns -1, if the line is wrong type
 * counter - it counts how manu elements of the second set are used in relation
 **/

int is_surjective(stg *storage1, stg *storage2, stg *storage3){
    int counter = 0;

    if(!(storage1->type == 'R') || !(storage2->type == 'S') || !(storage3->type == 'S')){
        return -1;
    }
    if (!(is_in_set(storage1, storage2, 0)) || !(is_in_set(storage1, storage3, 1))){
        return 0;
    }
    for(int i = 0; i<storage3->length; i++){
        for(int j = 0; j<storage1->length; j+=2){
            if(storage3->array[i]==storage1->array[j+1]){
                counter++;
                break;
            }
        }
    }
    if(counter==storage3->length){
        return 1;
    }
    return 0;
}

/**
 * @brief It prints true if the relation is surjective, and false if it's not
 * @param storage1 poiner to a structure where the line is stored (it should be relation)
 * @param storage2 poiner to a structure where the line is stored (it shoul be set)
 * @param storage3 poiner to a structure where the line is stored (it shoul be set)
 * @param ans pointer to a structure where the output of this function is stored
 * @return it returns -1 when the function wasn't done correctly, if it was done correctly it returns 1
 **/

int surjective(stg *storage1, stg *storage2, stg *storage3, stg *ans){
    int sur = is_surjective(storage1, storage2, storage3);

    if (sur==-1){
        return -1;
    }
    if (sur==0 || (storage1->length == 0 && storage3->length != 0)){
        if (save_true_false(ans, 'f') == -1){
            return -1;
        }
    } else if (sur==1){
        if (save_true_false(ans, 't') == -1){
            return -1;
        }
    }
    return 1;
}

/**
 * @brief It prints true if the relation is injective and surjecive, and false if it's not
 * @param storage1 poiner to a structure where the line is stored (it should be relation)
 * @param storage2 poiner to a structure where the line is stored (it shoul be set)
 * @param storage3 poiner to a structure where the line is stored (it shoul be set)
 * @param ans pointer to a structure where the output of this function is stored
 * @return it returns -1 when the function wasn't done correctly, if it was done correctly it returns 1
 **/

int bijective(stg *storage1, stg *storage2, stg *storage3, stg *ans){
    int sur = is_surjective(storage1, storage2, storage3);
    int inj = is_injective(storage1, storage2, storage3);

    if(sur==-1||inj==-1){
        return -1;
    } else if (sur==0||inj==0){
        if (save_true_false(ans, 'f') == -1){
            return -1;
        }
    } else if (sur==1||inj==1){
        if (save_true_false(ans, 't') == -1){
            return -1;
        }
    }
    return 1;
}


/**
 * @brief This function clears all the memory dynamically allocated in the area, that is pointed by the given pointer.
 * @param p_word pointer to the dynamically allocated arrea
 */

void word_dtor(char *p_word){
    if (p_word != NULL){
        free(p_word);
    }
}


/**
 * @brief This function clears all the memory dynamically allocated in the area, that is pointed by the given pointer.
 * @param array pointer to the dynamically allocated arrea of pointers to each word
 */

void stg_dtor(stg *p_stg){
    if (p_stg->array != NULL){
        free(p_stg->array);
    }
    if (p_stg != NULL){
        free(p_stg);
    }
}


/**
 * @brief This function clears all the memory dynamically allocated in the area designated for command, that is pointed by the given pointer.
 * @param p_comm pointer to the dynamically allocated area
 */

void comm_dtor(comm *p_comm){
    if (p_comm->str != NULL){
        free(p_comm->str);
        p_comm->str = NULL;
    }   
}


/**
 * @brief This function takes care of freeing the data stored in dynamically allocated memory.
 * @param all_lines pointer to storage of all lines, that need to be freed
 */

void clear_memory(lns *all_lines){
    int i = all_lines->line[0]->length;
    if (all_lines->count > 0){
        while (i > 0){
            word_dtor(all_lines->line[0]->array[i-1]);
            i--;
        }
    }
    i = 0;
    while (i < all_lines->count){
        if (all_lines->line[i]->type == 'C'){
            word_dtor(all_lines->line[i]->array[0]);
        }
        stg_dtor(all_lines->line[i]);
        i++;
    }
    free(all_lines->line);
}



