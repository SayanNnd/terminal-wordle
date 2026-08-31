#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define COLOR_RESET  "\x1b[39;49m"
#define COLOR_BLACK  "\033[40;37m"
#define COLOR_GREEN  "\x1b[42;30m"
#define COLOR_YELLOW "\x1b[43;30m"
#define COLOR_RED    "\x1b[41;30m"
#define COLOR_BLUE   "\x1b[46;30m"
#define NO_OF_WORDS  12944
#define VALID_WORDS  2315

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
    char validWords[NO_OF_WORDS][7];
    int loader = load_file(validWords);
    if (loader == 1) {
        printf("Could not succesfully open the file\n");
        return 0;
    }
    char word[10];
    strcpy(word, validWords[word_picker()]);
    char guess[20];
    printf("*WORDLE*\nGuess the correct word\n");
    for (int i=1; i<=6; i++) {
        fgets(guess, sizeof(guess), stdin);
        printf("\x1b[A\x1b[2K");
        if (strlen(guess) != 6) {
            printf(COLOR_RED "Please Enter a 5-Letter Word" COLOR_RESET "\n");
            i--;
            continue;
        }
        int resp = word_check(guess, validWords);
        if (resp == 1) {
            printf(COLOR_RED "Please Enter a valid Word" COLOR_RESET "\n");
            i--;
            continue;
        }
        
        int checker = 0;
        for (int j=0; j<5; j++) {
            if (toupper(word[j]) == toupper(guess[j])) {
                printf(COLOR_GREEN " %c " COLOR_RESET " ",toupper(guess[j]));
                checker++;
            }
            else if (toupper(word[0]) == toupper(guess[j]) || toupper(word[1]) == toupper(guess[j]) || toupper(word[2]) == toupper(guess[j]) || toupper(word[3]) == toupper(guess[j]) || toupper(word[4]) == toupper(guess[j])){
                printf(COLOR_YELLOW " %c " COLOR_RESET " ",toupper(guess[j]));
            }
            else {
                printf(COLOR_BLACK " %c " COLOR_RESET " ",toupper(guess[j]));
            }
        }
        printf("\n");
        if (checker == 5) {
            printf("CONGRATS!!\n");
            break;
        }
        if (i == 6) {
            printf("You Lost!!\n");
            for (int k = 0; k < 5; k++) {
                printf(COLOR_BLUE " %c " COLOR_RESET " ",toupper(word[k]));
            }
            printf(" was the correct word.\nBetter Luck Tomorrow\n");
            break;
        }
    }
    printf("Enter any key to exit :- ");
    getchar();
    return 0;
}