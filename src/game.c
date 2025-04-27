#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "util.h"
#include "game.h"
#include "sudoku.h"
#include "io.h"
#define MAX_HEARTS 3

GameState gameState;

void menu() {
    int choice = 0;
    while (1) {
      printf("======MENU======\n");
      printf("1] Tutorial\n");
      printf("2] New Game\n");
      printf("3] Continue\n");
      printf("4] Exit\n");
  
      if (scanf("%d", &choice) != 1) {
        printf("INVALID INPUT.\n");
        while (getchar() != '\n'); // flush input
        continue;
      }
  
      switch (choice) {
        case 1:
          tutorial();
          printf("Press any key to continue...");
          getchar();
          getchar();
          break;
        case 2:
          freeBoard();
          gameState.correctMoves = 0;
          gameState.hearts = MAX_HEARTS;
  
          boardSize();
  
          if (gameState.sizeOfTheBoard == 0) break;  // if something went wrong with choosing size go -> back to menu
  
          gameDifficulty();
  
          if (gameState.difficulty == 0) break; // the same
  
          // changes the “seed” or the starting point of the algorithm. A seed is an integer used to initialize the random number generator.
          // ensure a different sequence of random numbers; ergo a different sudoku puzzle every time
          srand(time(0));
          startTime();
          printf("\n");
  
          gameState.board = sudokuGenerator();
  
          printBoard();
  
          makeMove(gameState.board);
  
          printf("\n");
          endTime();
          calculateTimeElapsed();
          printf("Press any key to continue...");
          getchar();
          getchar();
          break;
        case 3:
          freeBoard();
          gameState.board = loadGameFromFile();
  
          if (gameState.board != NULL) {
            printBoard();
            startTime();
            makeMove(gameState.board);
  
            printf("\n");
            endTime();
            calculateTimeElapsed();
            printf("Press any key to continue...");
            getchar();
            getchar();
            break;
          }
          break;
        case 4:
          freeBoard();
          printf("CLOSING APP...\n");
          exit(0);
        default:
          printf("WRONG CHOICE.\n");
      }
  
    }
  }
  
// menu for the user to choose the board size
void boardSize() {
    int choice = 0;
    while (1) {
      printf("======NEW GAME======\n");
      printf("CHOOSE SIZE OF THE BOARD\n");
      printf("1] 4x4\n");
      printf("2] 9x9\n");
      printf("3] 16x16\n");
      printf("4] GO BACK\n");
  
      if (scanf("%d", &choice) != 1) {
        printf("INVALID INPUT.\n");
        while (getchar() != '\n'); // flush input
        continue;
      }
  
      if (choice == 4) return;
  
      const int boardSizeOptions[] = {4, 9, 16};
      if (choice < 1 || choice > 3) {
        printf("CHOOSE A VALID OPTION\n");
        continue;
      }
  
      gameState.sizeOfTheBoard = boardSizeOptions[choice-1];
      printf("SIZE CHOSEN: %d\n", gameState.sizeOfTheBoard);
      gameState.boxSize = sqrt(gameState.sizeOfTheBoard); // assigning the boxSize value after choosing size
      break;
    }
  }
  
  
  // menu for the user to choose the game difficulty
  void gameDifficulty() {
    int choice = 0;
    while (1) {
      printf("======NEW GAME======\n");
      printf("CHOOSE DIFFICULTY\n");
      printf("1] EASY\n");
      printf("2] MODERATE\n");
      printf("3] HARD\n");
      printf("4] GO BACK\n");
  
      if (scanf("%d", &choice) != 1) {
        printf("INVALID INPUT.\n");
        while (getchar() != '\n'); // flush input
        continue;
      }
  
      if (choice == 4) return;
  
      const int difficultyOptions[] = {1, 2, 3};
  
      if (choice < 1 || choice > 3) {
        printf("CHOOSE A VALID OPTION\n");
        continue;
      }
  
      gameState.difficulty = difficultyOptions[choice-1];
      printf("DIFFICULTY CHOSEN: %d\n", gameState.difficulty);
      break;
    }
  }
  
    