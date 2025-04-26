#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "util.h"


void menu(); // prototype to let the compiler know that a function like this exists
void saveGameToFile(int ** grid);


typedef struct {
  int sizeOfTheBoard;
  int boxSize;
  int **board;
  int hearts;
  int correctMoves;
  int difficulty;
} GameState;

GameState gameState;


// dynamic memory allocation since the user can choose the size of the board
int **initializeBoard() {
  int ** newBoard = (int **)malloc(gameState.sizeOfTheBoard * sizeof(int *));

  if (newBoard == NULL) {
    perror("FAILED TO ALLOCATE BOARD ROWS");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < gameState.sizeOfTheBoard; i++) {
    newBoard[i] = (int *)malloc(gameState.sizeOfTheBoard * sizeof(int));

    if (newBoard[i] == NULL) {
      perror("FAILED TO ALLOCATE BOARD COLUMNS");
      exit(EXIT_FAILURE);
    }

    for (int j = 0; j < gameState.sizeOfTheBoard; j++) {
      newBoard[i][j] = 0;
    }

  }
  return newBoard;
}


void freeBoard(){
  if (gameState.board != NULL) {
    for (int i = 0; i < gameState.sizeOfTheBoard; i++) {
      free(gameState.board[i]);
    }
    free(gameState.board);
    gameState.board = NULL; 
  }
}


// printing the board
void printBoard() {

  for (int i = 0; i < gameState.sizeOfTheBoard; i++) {
    for (int j = 0; j < gameState.sizeOfTheBoard; j++) {

      if (gameState.board[i][j] == 0) {
        printf(" . ");
      } 
      else if (gameState.board[i][j] <= 9) {
        printf("%2d ", gameState.board[i][j]);
      } 
      else {
        // it will replace 10-16 with a char of A-F
        printf(" %c ", 'A' + (gameState.board[i][j] - 10));
      }
    }
    printf("\n");
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
    printf("4] ADVANCED\n");
    printf("5] GO BACK\n");

    if (scanf("%d", &choice) != 1) {
      printf("INVALID INPUT.\n");
      while (getchar() != '\n'); // flush input
      continue;
    }

    if (choice == 5) return;

    const int difficultyOptions[] = {1, 2, 3, 4};

    if (choice < 1 || choice > 4) {
      printf("CHOOSE A VALID OPTION\n");
      continue;
    }

    gameState.difficulty = difficultyOptions[choice-1];
    printf("DIFFICULTY CHOSEN: %d\n", gameState.difficulty);
    break;
  }
}


// SUDOKU FUNCTIONS
// returns false if 3x3 box contains a number
bool unUsedInBox(int **grid, const int row, const int col, const int number) {

  for (int i = 0; i < gameState.boxSize; i++) {
    for (int j = 0; j < gameState.boxSize; j++) {
      if (grid[row + i][col + j] == number) {
        return false;
      }
    }
  }
  return true;
}


// filling 3x3, random numbers to subgrid
void fillBox(int **grid, const int row, const int col) {
  int number;
  for (int i = 0; i < gameState.boxSize; i++) {
    for (int j = 0; j < gameState.boxSize; j++) {
      do {
        if (gameState.boxSize == 3) {
          number = (rand() % 9) + 1;
        }
        else if (gameState.boxSize == 2) {
          number = (rand() % 4) + 1;
        }
        else if (gameState.boxSize == 4) {
          number = (rand() % 16) + 1;
        }
      } while (!unUsedInBox(grid, row, col, number));

      grid[row + i][col + j] = number;
    }
  }
}


// check if it's safe to put a number in row i
bool unUsedInRow(int **grid, const int i, const int number) {
  for (int j = 0; j < gameState.sizeOfTheBoard; j++) {
    if (grid[i][j] == number) {
      return false;
    }
  }
  return true;
}


// check if it's safe to put a number in column j
bool unUsedInCol(int **grid, const int j, const int number) {
  for (int i = 0; i < gameState.sizeOfTheBoard; i++) {
    if (grid[i][j] == number) {
      return false;
    }
  }
  return true;
}


// combines all the previous functions to return a bool whether it's safe to put a number in the cell (i, j)
// ensures that the number isn't used in: row, column, box
bool checkIfSafe(int **grid, const int i, const int j, const int number) {
  return (unUsedInRow(grid, i, number) && unUsedInCol(grid, j, number) && unUsedInBox(grid, i - i % gameState.boxSize, j - j % gameState.boxSize, number));
}


// filling the diagonal
void fillDiagonal(int **grid) {
  for (int i = 0; i < gameState.sizeOfTheBoard; i+= gameState.boxSize) {
      fillBox(grid, i, i);
  }
}


// filling the rest with the board
bool fillRemaining(int **grid, const int i, const int j) {
  // if end of grid
  if (i == gameState.sizeOfTheBoard) {
    return true;
  }

  // going to the next row if j == sizeOfTheBoard
  if (j == gameState.sizeOfTheBoard) {
    return fillRemaining(grid, i+1, 0);
  }

  //skip if cell is filled (fill diagonal)
  if (grid[i][j] != 0) {
    return fillRemaining(grid, i, j+1);
  }

  for (int num = 1; num <= gameState.sizeOfTheBoard; num++) {
    if (checkIfSafe(grid, i, j, num)) {
      grid[i][j] = num;
      if (fillRemaining(grid, i, j+1)) {
        return true;
      }
      grid[i][j] = 0;
    }
  }
  return false;
}


// remove K digits randomly
void removeCells(int **grid) {

  int total_cells = gameState.sizeOfTheBoard * gameState.sizeOfTheBoard;
  int cells_to_remove;

  switch (gameState.difficulty) {
    case 1:      // EASY
        cells_to_remove = total_cells * 20 / 100;
        break;
    case 2:      // MODERATE
        cells_to_remove = total_cells * 35 / 100;
        break;
    case 3:      // HARD
        cells_to_remove = total_cells * 50 / 100;
        break;
    case 4:      // ADVANCED
        if (gameState.sizeOfTheBoard == 4) {
            cells_to_remove = total_cells * 60 / 100;
        } else {
            cells_to_remove = total_cells * 70 / 100;
        }
        break;
    default:
        cells_to_remove = total_cells * 30 / 100; // Default EASY
        break;
  }

  while (cells_to_remove > 0) {
    const int cell = rand() % (gameState.sizeOfTheBoard * gameState.sizeOfTheBoard);

    const int i = cell / gameState.sizeOfTheBoard;
    const int j = cell % gameState.sizeOfTheBoard;

    if (grid[i][j] != 0) {
      grid[i][j] = 0;
      cells_to_remove--;
    }
  }

}


// generating the sudoku board
int **sudokuGenerator() {
  int ** grid = initializeBoard();

  fillDiagonal(grid);
  fillRemaining(grid, 0, 0);

  removeCells(grid);

  return grid;
}


// empty spots to fill on the board -> spots that the user will have to fill
int countEmptyCells(int **grid) {
  int count = 0;
  for (int i = 0; i < gameState.sizeOfTheBoard; i++) {
    for (int j = 0; j < gameState.sizeOfTheBoard; j++) {
      if (grid[i][j] == 0) count++;
    }
  }
  return count;
}


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


//int hearts; // "lives"
//int correctMoves;

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
      scanf("%d", &value);

      // value validation
      if (value < 1 || value > gameState.sizeOfTheBoard) {
          printf("INVALID VALUE!\n");
          continue;
        }
    }
    else if (gameState.sizeOfTheBoard == 9) {
      printf("ENTER VALUE (1-9): ");
      scanf("%d", &value);
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
        system("pause");
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
        system("pause");
        break;
      case 2:
        freeBoard();
        gameState.correctMoves = 0;
        gameState.hearts = 3;

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
        system("pause");
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
          system("pause");
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


int main(){
  menu();

  return 0;
}
