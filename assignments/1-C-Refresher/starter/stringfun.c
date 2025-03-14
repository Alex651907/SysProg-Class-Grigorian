#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
void reverse_string(char *, int);
void word_print(char *, int);


int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    char *src = user_str;
    char *dst = buff;
    int count = 0;
    int prev_space = 1;

    while (*src != '\0' && count < len) {
        if (isspace(*src)) {
            if (!prev_space) {
                *dst++ = ' ';
                count++;
                prev_space = 1;
            }
        } else {
            *dst++ = *src;
            count++;
            prev_space = 0;
        }
        src++;
    }

    // String is too large
    if (*src != '\0' && count == len) { 
        return -1;
    }

    // Remove the last space if it's at the end
    if (count > 0 && *(dst - 1) == ' ') {
        dst--;
        count--;
    }

    //Fill the rest of the buffer with periods
    while (count < len) {
        *dst++ = '.';
        count++;
    }

    //Return the actual number of characters copied
    return count;
}

void print_buff(char *buff, int len){
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar(']');
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    int word_count = 0;
    int word = 0;

    for (int i = 0; i < str_len; i++) {
        if (*(buff + i) != ' ' && *(buff + i) != '.') {
            if (!word) {
                word_count++;
                word = 1;
            }
        } else {
            word = 0;
        }
    }
    return word_count;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS
void reverse_string(char *buff, int str_len) {
    // Reverse the string in place in the buffer
    char *start = buff;
    char *end = buff + str_len - 1;

    // Reverse the characters in the buffer
    while (end > start) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }

    // Print the reversed string (ignoring periods)
    printf("Reversed String: ");
    for (int i = 0; i < str_len; i++) {
        if (buff[i] != '.') {
            putchar(buff[i]);
        }
    }
    printf("\n");
}





void word_print(char *buff, int str_len) {
    printf("Word Print\n----------\n");
    int index = 1;
    int char_count = 0;
    int word = 0;

    //Word count should ignore periods
    for (int i = 0; i < str_len; i++) {
        if (*(buff + i) != ' ' && *(buff + i) != '.') {
            if (!word) {
                printf("%d. ", index++);
                word = 1;
            }
            putchar(*(buff + i));
            char_count++;
        } else {
            if (word) {
                printf(" (%d)\n", char_count);
                char_count = 0;
                word = 0;
            }
        }
    }
    if (word) {
        printf(" (%d)\n", char_count);
    }
    printf("\nNumber of words returned: %d\n", index - 1);
}

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    /* 
        This is safe because the code first checks the first part of the if statement, argc < 2. argc is the number
        of arguments given, and argv starts indexing at 0. Therefore, if there are less than 2 args, it will not even
        bother checking to see if argv[1] != '-', because the if statement will fail, and this will not throw an error.
    */
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //Handle search and replace flag before processing anything
    if (opt == 'x') {
        if (argc != 5) {
            printf("Error: Missing arguments for -x. Usage: ./stringfun -x \"string\" search replace\n");
            exit(1);
        }

        //Placeholder for now
        printf("Not Implemented!\n");
        exit(3);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    /*
        The program needs a minimum of 3 arguments to run - The command, 'stringfun', a flag, e.g. '-r', and a string.
        If a minimum of 3 arguments is not given, something must be missing, and the program will not be able to run, so
        print an error message to the user showing them how to correctly use the program.
    */
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char *)malloc(BUFFER_SZ);
    if (!buff) {
        perror("Error with malloc");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        free(buff);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        // Reverse string
        case 'r':
            reverse_string(buff, user_str_len);
            break;

        // Print words and their lengths
        case 'w': 
            word_print(buff, user_str_len);
            break;

        default:
            usage(argv[0]);
            free(buff);
            exit(1);
    }


    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
/*
            The length is still important, because even though we have buffer size
            hardcoded to 50, this allows the program to theoretically work on any
            buffer size as long as we change the constant. This way we can make
            sure to prevent overflow errors while keeping it scalable to any
            buffer size.
*/