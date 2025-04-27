#ifndef GAME_H
#define GAME_H

typedef struct {
    int sizeOfTheBoard;
    int boxSize;
    int **board;
    int hearts;
    int correctMoves;
    int difficulty;
} GameState;

extern GameState gameState;

void menu();
void boardSize();
void gameDifficulty();

#endif
