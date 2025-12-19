#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "wordle.h"

// ===== MAIN =====
int main() {
    char dictionary[MAX_WORDS][WORD_LEN + 1];
    int wordCount = loadDictionary(dictionary, "words.txt");
    if (wordCount <= 0) return 1;

    srand(time(NULL));
    char target[WORD_LEN + 1];
    strcpy(target, dictionary[rand() % wordCount]);

    int mode;
    printf("=====================================\n");
printf("        WELCOME TO WORDLE GAME        \n");
printf("=====================================\n\n");

printf("Rules of the game:\n");
printf("- The computer has chosen a hidden 5-letter word.\n");
printf("- You have 6 attempts to guess the correct word.\n");
printf("- After each guess, you will receive feedback:\n");
printf("  * Green : Correct letter in correct position.\n");
printf("  * Yellow : Correct letter in wrong position.\n");
printf("  * Gray : Letter not in the word.\n\n");

printf("Type your 5-letter word and press Enter.\n");
printf("Good luck and have fun!\n\n");
printf("1-Manual Mode:\nYou will guess the word by typing each attempt yourself.\nYou have 6 tries to find the correct 5-letter word.\n\n");

printf("2-Automatic Mode:\nThe computer will generate guesses automatically.\nWatch how the game is solved step by step.\n\n");

    printf("Choix: ");
    scanf("%d", &mode);
    getchar();

    char possibleWords[MAX_WORDS][WORD_LEN + 1];
    int possibleCount = wordCount;
    for (int i = 0; i < wordCount; i++)
        strcpy(possibleWords[i], dictionary[i]);

    for (int attempt = 1; attempt <= MAX_ATTEMPTS; attempt++) {
        char guess[WORD_LEN + 1];
        char input[100];

        printf("\nAttempt %d/%d\n", attempt, MAX_ATTEMPTS);

        if (mode == 1) {
            printf("Enter a word: ");
            fgets(input, sizeof(input), stdin);

            size_t len = strlen(input);
            if (len > 0 && (input[len - 1] == '\n' || input[len - 1] == '\r'))
                input[--len] = '\0';

            if (len != WORD_LEN) {
                attempt--;
                continue;
            }

            for (int i = 0; i < WORD_LEN; i++)
                guess[i] = tolower(input[i]);
            guess[WORD_LEN] = '\0';

            if (!isValidWord(dictionary, wordCount, guess)) {
                attempt--;
                continue;
            }
        } else {
            solverGuess(possibleWords, possibleCount, guess);
            printf("Solver propose: %s\n", guess);
        }

        if (strcmp(guess, target) == 0) {
            printf(COLOR_GREEN "\nCONGRATULATIONS! You won! Word = %s\n" COLOR_RESET, target);

            return 0;
        }

        giveFeedback(target, guess);

        if (mode == 2)
            filterPossibleWords(possibleWords, &possibleCount, guess, target);
    }

    printf(COLOR_GRAY "\nBetter luck next time! The word was: %s\n" COLOR_RESET, target);

    return 0;
}

