#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define WORD_LEN 5
#define MAX_WORDS 6000
#define MAX_ATTEMPTS 6

// ANSI Colors
#define COLOR_GREEN  "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_GRAY   "\x1b[90m"
#define COLOR_RESET  "\x1b[0m"

// ===== Prototypes =====
int loadDictionary(char words[][WORD_LEN + 1], const char *filename);
int isValidWord(char words[][WORD_LEN + 1], int wordCount, const char *guess);
void giveFeedback(const char *target, const char *guess);

// Solver
void solverGuess(char possibleWords[][WORD_LEN + 1], int possibleCount, char *guess);
int isWordCompatible(const char *word, const char *guess, const char *target);
void filterPossibleWords(
    char possibleWords[][WORD_LEN + 1],
    int *possibleCount,
    const char *guess,
    const char *target
);

// ===== Load dictionary =====
int loadDictionary(char words[][WORD_LEN + 1], const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Erreur: impossible d'ouvrir le fichier %s\n", filename);
        return -1;
    }

    int count = 0;
    char buffer[100];

    while (fgets(buffer, sizeof(buffer), f)) {
        for (int i = 0; buffer[i]; i++)
            buffer[i] = tolower(buffer[i]);

        size_t len = strlen(buffer);
        if (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r'))
            buffer[--len] = '\0';

        if (len == WORD_LEN && count < MAX_WORDS) {
            strcpy(words[count], buffer);
            count++;
        }
    }

    fclose(f);
    return count;
}

// ===== Check word =====
int isValidWord(char words[][WORD_LEN + 1], int wordCount, const char *guess) {
    for (int i = 0; i < wordCount; i++)
        if (strcmp(words[i], guess) == 0)
            return 1;
    return 0;
}

// ===== Feedback =====
void giveFeedback(const char *target, const char *guess) {
    int result[WORD_LEN] = {0};
    char temp[WORD_LEN + 1];
    strcpy(temp, target);

    for (int i = 0; i < WORD_LEN; i++) {
        if (guess[i] == temp[i]) {
            result[i] = 2;
            temp[i] = '*';
        }
    }

    for (int i = 0; i < WORD_LEN; i++) {
        if (result[i] == 2) continue;
        for (int j = 0; j < WORD_LEN; j++) {
            if (guess[i] == temp[j]) {
                result[i] = 1;
                temp[j] = '*';
                break;
            }
        }
    }

    printf("Feedback: ");
    for (int i = 0; i < WORD_LEN; i++) {
        if (result[i] == 2)
            printf(COLOR_GREEN "%c" COLOR_RESET, toupper(guess[i]));
        else if (result[i] == 1)
            printf(COLOR_YELLOW "%c" COLOR_RESET, toupper(guess[i]));
        else
            printf(COLOR_GRAY "%c" COLOR_RESET, toupper(guess[i]));
    }
    printf("\n");
}

// ===== Solver =====
void solverGuess(char possibleWords[][WORD_LEN + 1], int possibleCount, char *guess) {
    strcpy(guess, possibleWords[0]);
}

int isWordCompatible(const char *word, const char *guess, const char *target) {
    for (int i = 0; i < WORD_LEN; i++) {
        if (guess[i] == target[i] && word[i] != guess[i])
            return 0;

        if (guess[i] != target[i]) {
            int found = 0;
            for (int j = 0; j < WORD_LEN; j++)
                if (guess[i] == target[j])
                    found = 1;

            if (!found) {
                for (int j = 0; j < WORD_LEN; j++)
                    if (word[j] == guess[i])
                        return 0;
            }
        }
    }
    return 1;
}

void filterPossibleWords(
    char possibleWords[][WORD_LEN + 1],
    int *possibleCount,
    const char *guess,
    const char *target
) {
    char temp[MAX_WORDS][WORD_LEN + 1];
    int newCount = 0;

    for (int i = 0; i < *possibleCount; i++) {
        if (isWordCompatible(possibleWords[i], guess, target)) {
            strcpy(temp[newCount], possibleWords[i]);
            newCount++;
        }
    }

    *possibleCount = newCount;
    for (int i = 0; i < newCount; i++)
        strcpy(possibleWords[i], temp[i]);
}

// ===== MAIN =====
int main() {
    char dictionary[MAX_WORDS][WORD_LEN + 1];
    int wordCount = loadDictionary(dictionary, "words.txt");
    if (wordCount <= 0) return 1;

    srand(time(NULL));
    char target[WORD_LEN + 1];
    strcpy(target, dictionary[rand() % wordCount]);

    int mode;
    printf("Mode de jeu:\n");
    printf("1 - Joueur humain\n");
    printf("2 - Solver automatique\n");
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

        printf("\nTentative %d/%d\n", attempt, MAX_ATTEMPTS);

        if (mode == 1) {
            printf("Entrez un mot: ");
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
            printf(COLOR_GREEN "\nGAGNÉ ! Mot = %s\n" COLOR_RESET, target);
            return 0;
        }

        giveFeedback(target, guess);

        if (mode == 2)
            filterPossibleWords(possibleWords, &possibleCount, guess, target);
    }

    printf(COLOR_GRAY "\nPERDU ! Mot = %s\n" COLOR_RESET, target);
    return 0;
}

