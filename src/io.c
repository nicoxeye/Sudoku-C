#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "util.h"
#include "game.h"
#include "sudoku.h"
#include "io.h"

// a pause on the game
void pauseMenu(int **grid) {
    int choice;
    while (1) {
      printf("\n===== PAUSE MENU =====\n");
      printf("1] Save Game\n");
      printf("2] Continue Game\n");
      printf("3] Exit\n");
      printf("======================\n");
      if (scanf("%d", &choice) != 1) {
        printf("INVALID INPUT.\n");
        while (getchar() != '\n'); // flush input
        continue;
      }
  
  
      switch (choice) {
        case 1:
          saveGameToFile(grid);
          break;
        case 2:
          printBoard();
          printf("RESUMING GAME...\n");
          return;
        case 3:
          freeBoard();
          exit(0);
        default:
          printf("INVALID CHOICE\n");
          return;
      }
    }
  }
  
  
  // user types in a row. column and value
  // validates user info and assigns the value if correct
  // otherwise takes a heart and informs of the error made
  void makeMove(int **grid) {
    const int totalToFill = countEmptyCells(grid);
  
    while (gameState.correctMoves < totalToFill && gameState.hearts > 0) {
      int row, col, value;
  
      char input2[10];
      printf("ENTER A ROW (1-%d) OR 'p' TO PAUSE: ", gameState.sizeOfTheBoard);
      scanf("%s", input2);
  
      if (input2[0] == 'p' || input2[0] == 'P') {
        pauseMenu(grid);
        continue;
      }
  
      row = atoi(input2); // atoi() - ASCII to Integer
      row--; // adjusting to 0-based indexing
  
      printf("ENTER A COLUMN (1-%d): ", gameState.sizeOfTheBoard);
  
      if (scanf("%d", &col) != 1) {
        printf("INVALID INPUT.\n");
        while (getchar() != '\n'); // flush input
        continue;
      }
  
      col--;
  
      if (gameState.sizeOfTheBoard == 4) {
        printf("ENTER VALUE (1-4): ");
        
        if (scanf("%d", &value) != 1) {
          printf("INVALID INPUT.\n");
          while (getchar() != '\n'); // flush input
          continue;
        }
  
        // value validation
        if (value < 1 || value > gameState.sizeOfTheBoard) {
            printf("INVALID VALUE!\n");
            continue;
          }
      }
      else if (gameState.sizeOfTheBoard == 9) {
        printf("ENTER VALUE (1-9): ");
  
        if (scanf("%d", &value) != 1) {
          printf("INVALID INPUT.\n");
          while (getchar() != '\n'); // flush input
          continue;
        }
        // value validation
        if (value < 1 || value > gameState.sizeOfTheBoard) {
            printf("INVALID VALUE!\n");
            continue;
        }
      }
      else if (gameState.sizeOfTheBoard == 16) {
        char input[10];
        printf("ENTER VALUE (1-9) OR (A-F): ");
        scanf("%s", input);
  
        if (input[0] >= '1' && input[0] <= '9') {
          value = input[0] - '0';
        } else if (input[0] >= 'A' && input[0] <= 'F') {
          value = 10 + (input[0] - 'A');
        } else if (input[0] >= 'a' && input[0] <= 'f') {
          value = 10 + (input[0] - 'a');
        } else {
          printf("INVALID INPUT!\n");
          continue;
        }
      }
  
  
      // position validation
      if (row < 0 || row >= gameState.sizeOfTheBoard || col < 0 || col >= gameState.sizeOfTheBoard) {
        printf("INVALID POSITION!\n");
        continue;
      }
  
      // cell user wants to fill has to be 0
      if (grid[row][col] != 0) {
        printf("CELL ALREADY FILLED!\n");
        continue;
      }
  
      if (checkIfSafe(grid, row, col, value)) {
        // if it's safe to do so assign the value to the grid and print the board
        grid[row][col] = value;
        gameState.correctMoves++;
        printBoard();
        printf("\nHEARTS: %d | REMAINING SPOTS: %d\n", gameState.hearts, totalToFill - gameState.correctMoves);
      } else {
        //if it's not -> -1 to hearts
        gameState.hearts--;
        printf("WRONG! HEARTS LEFT: %d\n", gameState.hearts);
  
        //game over
        if (gameState.hearts <= 0) {
          printf("GAME OVER\n");
          printf("Press any key to continue...");
          getchar();
          getchar();
          return;
        }
      }
  
      // winning the game
      if (gameState.correctMoves == totalToFill){
        printf("GAME WON!\n");
        return;
      }
  
    }
  }
  
  
  // saving array to file to be able to continue a game
  void saveGameToFile(int **grid) {
    FILE *f = fopen("game.data", "wb"); // writing a binary file
  
    if (!f) {
      printf("ERROR OPENING THE FILE\n");
      return;
    }
  
    // saving game metadata
    fwrite(&gameState.sizeOfTheBoard, sizeof(int), 1, f);
    fwrite(&gameState.difficulty, sizeof(int), 1, f);
    fwrite(&gameState.hearts, sizeof(int), 1, f);
    fwrite(&gameState.correctMoves, sizeof(int), 1, f);
  
    // writing the board row by row
    for (int i = 0; i < gameState.sizeOfTheBoard; i++) {
      fwrite(grid[i], sizeof(int), gameState.sizeOfTheBoard, f);
    }
  
    fclose(f);
    printf("GAME SAVED SUCCESSFULLY\n");
  }
  
  
  // loading a saved game
  int **loadGameFromFile() {
    FILE *f = fopen("game.data", "rb"); // read binary
  
    if (!f) {
      printf("ERROR OPENING THE FILE\n");
      return NULL;
    }
  
    // reading game metadata
    fread(&gameState.sizeOfTheBoard, sizeof(int), 1, f);
    fread(&gameState.difficulty, sizeof(int), 1, f);
    fread(&gameState.hearts, sizeof(int), 1, f);
    fread(&gameState.correctMoves, sizeof(int), 1, f);
    gameState.boxSize = sqrt(gameState.sizeOfTheBoard); // updating box size
  
    int **grid = initializeBoard(); // initializing the board with 0s
  
    for (int i = 0; i < gameState.sizeOfTheBoard; i++) {
      fread(grid[i], sizeof(int), gameState.sizeOfTheBoard, f); //writing the data to the board
    }
  
    fclose(f);
    return grid;
  }
  