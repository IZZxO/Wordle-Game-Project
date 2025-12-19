#ifndef WORDLE_H
#define WORDLE_H

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
#endif
