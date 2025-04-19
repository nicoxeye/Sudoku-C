#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


// 1 - EASY, 2 - MODERATE, 3 - HARD, 4 - ADVANCED
int difficulty = 0;
// TODO: ADD HEARTS AND LIVES SYSTEM :D
int hearts = 3;
// 3X3, 4X4, 9X9, 16X16
int sizeOfTheBoard= 0;

int ** board;

//TODO: IMPLEMENT THIS FOR 4X4 TO WORK ETC
//boxSize = sqrt(sizeOfTheBoard);

// dynamic memory allocation since the user can choose the size of the board
int **initializeBoard(const int sizeOfTheBoard) {
  int ** newBoard = (int **)malloc(sizeOfTheBoard * sizeof(int *));

  if (newBoard == NULL) {
    perror("FAILED TO ALLOCATE BOARD ROWS");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < sizeOfTheBoard; i++) {
    newBoard[i] = (int *)malloc(sizeOfTheBoard * sizeof(int));

    if (newBoard[i] == NULL) {
      perror("FAILED TO ALLOCATE BOARD COLUMNS");
      exit(EXIT_FAILURE);
    }

    for (int j = 0; j < sizeOfTheBoard; j++) {
      newBoard[i][j] = 0;
    }

  }
  return newBoard;
}


// printing the board!
void printBoard(const int sizeOfTheBoard) {
  for (int i = 0; i < sizeOfTheBoard; i++) {
    for (int j = 0; j < sizeOfTheBoard; j++) {
      printf("%2d ", board[i][j]);
    }
    printf("\n");
  }
}

// pause to imitate terminal clear since nothing worked for me to do it differentlyT-T
void pause() {
  for (int i = 0; i < 100; i++) {
    printf("\n");
  }
}


void boardSize() {
  int choice = 0;
  while (1) {
    printf("======NEW GAME======\n");
    printf("CHOOSE SIZE OF THE BOARD\n");
    printf("1] 3x3\n"); //temporary
    printf("2] 4x4\n");
    printf("3] 9x9\n");
    printf("4] 16x16\n");
    printf("5] GO BACK\n");
    scanf("%d", &choice);

    if (choice == 5) return;

    const int boardSizeOptions[] = {3, 4, 9, 16};
    if (choice < 1 || choice > 4) {
      printf("CHOOSE A VALID OPTION\n");
      continue;
    }

    sizeOfTheBoard = boardSizeOptions[choice-1];
    printf("SIZE CHOSEN: %d\n", sizeOfTheBoard);
    break;
  }
}


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

    scanf("%d", &choice);

    if (choice == 5) return;

    const int difficultyOptions[] = {1, 2, 3, 4};

    if (choice < 1 || choice > 4) {
      printf("CHOOSE A VALID OPTION\n");
      continue;
    }

    difficulty = difficultyOptions[choice-1];
    printf("DIFFICULTY CHOSEN: %d\n", difficulty);
    break;
  }
}

//SUDOKU FUNCTIONS

// returns false if 3x3 box contains a number
// TODO: if user places a number and it's false -> -1 heart
bool unUsedInBox(int **grid, const int row, const int col, const int number) {

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (grid[row + i][col + j] == number) {
        return false;
      }
    }
  }
  return true;
}


//filling 3x3, random numbers to subgrid
void fillBox(int **grid, const int row, const int col) {
  int number;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      do {
        number = (rand() % 9) + 1;
      } while (!unUsedInBox(grid, row, col, number));

      grid[row + i][col + j] = number;
    }
  }
}


//check if it's safe to put a number in row i
bool unUsedInRow(int **grid, const int i, const int number) {
  for (int j = 0; j < 9; j++) {
    if (grid[i][j] == number) {
      return false;
    }
  }
  return true;
}

//check if it's safe to put a number in column j
bool unUsedInCol(int **grid, const int j, const int number) {
  for (int i = 0; i < 9; i++) {
    if (grid[i][j] == number) {
      return false;
    }
  }
  return true;
}

// combines all the previous functions to return a bool whether it's safe
// to put a number in the cell (i, j)
// ensures that the number isn't used in: row, column, box
bool checkIfSafe(int **grid, const int i, const int j, const int number) {
  return (unUsedInRow(grid, i, number) && unUsedInCol(grid, j, number) && unUsedInBox(grid, i - i % 3, j - j % 3, number));
}


// simplifying the process ;)
void fillDiagonal(int **grid) {
  for (int i = 0; i < 9; i+= 3) {
      fillBox(grid, i, i);
  }
}

bool fillRemaining(int **grid, const int i, const int j) {
  // end of grid
  if (i == 9) {
    return true;
  }

  // going to the next row if j=9
  if (j == 9) {
    return fillRemaining(grid, i+1, 0);
  }

  //skip if cell is filled (fill diagonal)
  if (grid[i][j] != 0) {
    return fillRemaining(grid, i, j+1);
  }

  for (int num = 1; num <= 9; num++) {
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

// TODO
// remove K digits randomly
void removeKDigits(int **grid, int k) {

}


int **sudokuGenerator(const int k) {
  int ** grid = initializeBoard(sizeOfTheBoard);

  fillDiagonal(grid);
  fillRemaining(grid, 0, 0);
  // TODO
  // removeKDigits(grid, k);
  return grid;
}



void menu() {
  int x = 1;
  int choice = 0;
  while (x == 1) {
    printf("======MENU======\n");
    printf("1] TUTORIAL*\n");
    printf("2] NEW GAME\n");
    printf("3] CONTINUE\n");
    printf("4] SEE PREVIOUS GAMES\n");
    printf("5] EXIT\n");

    scanf("%d", &choice);

    switch (choice) {
      case 1:
        printf("\n\nSudoku is a logic-based number placement puzzle. "
               "The goal is to fill a nxn grid with numbers 1 through 9, "
               "ensuring that each number appears only once in every row, column, and (usually) a 3x3 subgrid. \n\n");
        printf("BASIC RULES FOR A 9X9 BOARD:\n");
        printf("  1. Grid: The puzzle consists of a 9x9 grid divided into 9 nonets (3x3 subgrids).\n");
        printf("  2. The numbers 1 through 9 must be placed in each row, column, and nonet.\n");
        printf("  3. Each number can only appear once in each row, column, and nonet.\n");
        printf("  4. A Sudoku puzzle will have some numbers pre-filled in the grid, providing a starting point. \n\n\n");

        getchar();
        printf("Press Enter to continue...");
        getchar();
        break;
      case 2:
        boardSize();

        if (sizeOfTheBoard == 0) break;  // go back to menu

        gameDifficulty();

        if (difficulty == 0) break; // go back to menu

        board = sudokuGenerator(5);

        printBoard(sizeOfTheBoard);

        printf("pause to debug\n\n");
        scanf("%s");
        break;
      case 3:
        printf("WILL ADD IT\n");
        break;
      case 4:
        printf("WILL ADD IT\n");
        break;
      case 5:
        printf("CLOSING APP...\n");
        exit(0);
      default:
        printf("WRONG CHOICE.\n");
    }

  }
}


int main(){
  menu();

  if (board != NULL) {
    for (int i = 0; i < sizeOfTheBoard; i++) {
      free(board[i]);
    }
    free(board);
  }

  return 0;
}
