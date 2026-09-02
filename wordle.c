#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

//colors
#define COLOR_RESET  "\x1b[39;49m"           
#define COLOR_BLACK  "\033[40;37m"
#define COLOR_GREEN  "\x1b[42;30m"
#define COLOR_YELLOW "\x1b[43;30m"
#define COLOR_RED    "\x1b[41;30m"
#define COLOR_BLUE   "\x1b[46;30m"
#define NO_OF_WORDS  12979
#define VALID_WORDS  2315

//Loads the file into an array
int load_file(char validWords[][7]) {
    FILE *wordle;
    wordle = fopen("words.txt","r");
    if (wordle == NULL) {
        printf("Error: Could not open words.txt\n");
        return 1;
    }
    for (int i=0; i<NO_OF_WORDS; i++) {
        fgets(validWords[i],7,wordle); 
    }
    fclose(wordle);
    return 0;
}

//Picks word of the day
int word_picker() {
    time_t rawtime;
    time(&rawtime);
    struct tm *local_time = localtime(&rawtime);
    int year = local_time->tm_year + 1900;
    int month = local_time->tm_mon + 1;
    int day = local_time->tm_mday;
    int shorted = (year*10000)+(month*100)+(day);
    int seed = (shorted * 1103515245 + 12345) & 0x7fffffff;
    return (seed % (VALID_WORDS + 1));
}

//Checks if a word is valid ------ Needs to be optimized 
int word_check(char word[10], char validWords[][7]) {
    for (int i=0; i<NO_OF_WORDS; i++) {
        int j=0;
        for (int k=0; k<5; k++) {
            if (toupper(word[k])==toupper(validWords[i][k])) j++;
        }
        if (j==5) return 0;
    }
    return 1;
}

int main() {
    char validWords[NO_OF_WORDS][7];    //stores all words

    //error handling for file
    int loader = load_file(validWords);
    if (loader == 1) {
        printf("Could not succesfully open the file\n");
        printf("Enter any key to exit :- ");
        getchar();
        return 0;
    }

    char word[10];
    strcpy(word, validWords[word_picker()]);
    char guess[20];
    int status = 0;
    printf("*WORDLE*\nGuess the correct word\n");

    //Game Logic
    for (int i=1; i<=6; i++) {
        fgets(guess, sizeof(guess), stdin);
        //removes older error lines
        if (status == 1) {
            printf("\x1b[A\x1b[2K");
        }
        printf("\x1b[A\x1b[2K");

        if (strlen(guess) != 6) {
            printf(COLOR_RED "Please Enter a 5-Letter Word" COLOR_RESET "\n");
            status = 1;
            i--;
            continue;
        }
        int resp = word_check(guess, validWords);
        if (resp == 1) {
            printf(COLOR_RED "Please Enter a valid Word" COLOR_RESET "\n");
            status = 1;
            i--;
            continue;
        }

        char hash[10];         //temporary storage for the word which can be edited
        char colors[10][10];   //tracks colors for each tile
        strcpy(hash,word);     
        int checker = 0;       //counts how many letters are correct
        
        //word comparision
        for (int j=0; j<5; j++) {
            if (toupper(hash[j]) == toupper(guess[j])) {
                hash[j] = '\0';
                strcpy(colors[j],COLOR_GREEN);
                checker++;
            }
        }
        for (int j=0; j<5; j++) {
            int done = 0;
            if (hash[j] != '\0') {
                for (int t=0; t<5; t++) {
                    if (toupper(hash[t]) == toupper(guess[j])) {
                        hash[t] = '\0';
                        strcpy(colors[j],COLOR_YELLOW);
                        done = 1;
                        break;
                    }
                }
            if (done == 0) strcpy(colors[j],COLOR_BLACK);;    
            }
        }

        //print the word
        for (int j=0; j<5; j++) {
            printf("%s %c "COLOR_RESET " ",colors[j],toupper(guess[j]));
        }
        printf("\n");

        //win condition
        if (checker == 5) {
            printf("CONGRATS!!\n");
            break;
        }

        //lose condition
        if (i == 6) {
            printf("You Lost!!\n");
            for (int k = 0; k < 5; k++) {
                printf(COLOR_BLUE " %c " COLOR_RESET " ",toupper(word[k]));
            }
            printf(" was the correct word.\nBetter Luck Tomorrow\n");
            break;
        }
        status = 0;
    }
    printf("Enter any key to exit :- ");
    getchar();
    return 0;
}