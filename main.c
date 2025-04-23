#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// 1 - EASY, 2 - MODERATE, 3 - HARD, 4 - ADVANCED
int difficulty = 0;
// 4X4, 9X9, 16X16
int sizeOfTheBoard= 0;
int ** board;
int boxSize; // 4x4 -> 2x2, 16x16 -> 4x4
void menu(); // prototype to let teh compiler know that a function like this exists
void saveGameToFile(int ** grid);

// dynamic memory allocation since the user can choose the size of the board
int **initializeBoard() {
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
void printBoard() {
  for (int i = 0; i < sizeOfTheBoard; i++) {
    for (int j = 0; j < sizeOfTheBoard; j++) {
      if (board[i][j] == 0) {
        printf(" . ");
      } else if (board[i][j] <= 9) {
        printf("%2d ", board[i][j]);
      } else {
        // it will replace 10-16 with a char of A-F
        printf(" %c ", 'A' + (board[i][j] - 10));
      }
    }
    printf("\n");
  }
}


void boardSize() {
  int choice = 0;
  while (1) {
    printf("======NEW GAME======\n");
    printf("CHOOSE SIZE OF THE BOARD\n");
    printf("1] 4x4\n");
    printf("2] 9x9\n");
    printf("3] 16x16\n");
    printf("4] GO BACK\n");
    scanf("%d", &choice);

    if (choice == 4) return;

    const int boardSizeOptions[] = {4, 9, 16};
    if (choice < 1 || choice > 3) {
      printf("CHOOSE A VALID OPTION\n");
      continue;
    }

    sizeOfTheBoard = boardSizeOptions[choice-1];
    printf("SIZE CHOSEN: %d\n", sizeOfTheBoard);
    boxSize = sqrt(sizeOfTheBoard); //assigning the boxSize value after choosing size
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
bool unUsedInBox(int **grid, const int row, const int col, const int number) {

  for (int i = 0; i < boxSize; i++) {
    for (int j = 0; j < boxSize; j++) {
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
  for (int i = 0; i < boxSize; i++) {
    for (int j = 0; j < boxSize; j++) {
      do {
        if (boxSize == 3) {
          number = (rand() % 9) + 1;
        }
        else if (boxSize == 2) {
          number = (rand() % 4) + 1;
        }
        else if (boxSize == 4) {
          number = (rand() % 16) + 1;
        }
      } while (!unUsedInBox(grid, row, col, number));

      grid[row + i][col + j] = number;
    }
  }
}


//check if it's safe to put a number in row i
bool unUsedInRow(int **grid, const int i, const int number) {
  for (int j = 0; j < sizeOfTheBoard; j++) {
    if (grid[i][j] == number) {
      return false;
    }
  }
  return true;
}


//check if it's safe to put a number in column j
bool unUsedInCol(int **grid, const int j, const int number) {
  for (int i = 0; i < sizeOfTheBoard; i++) {
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
  return (unUsedInRow(grid, i, number) && unUsedInCol(grid, j, number) && unUsedInBox(grid, i - i % boxSize, j - j % boxSize, number));
}


// simplifying the process ;)
void fillDiagonal(int **grid) {
  for (int i = 0; i < sizeOfTheBoard; i+= boxSize) {
      fillBox(grid, i, i);
  }
}


bool fillRemaining(int **grid, const int i, const int j) {
  // end of grid
  if (i == sizeOfTheBoard) {
    return true;
  }

  // going to the next row if j=9
  if (j == sizeOfTheBoard) {
    return fillRemaining(grid, i+1, 0);
  }

  //skip if cell is filled (fill diagonal)
  if (grid[i][j] != 0) {
    return fillRemaining(grid, i, j+1);
  }

  for (int num = 1; num <= sizeOfTheBoard; num++) {
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
void removeKDigits(int **grid, int k) {
  while (k > 0) {
    const int cell = rand() % (sizeOfTheBoard * sizeOfTheBoard);

    const int i = cell / sizeOfTheBoard;
    const int j = cell % sizeOfTheBoard;

    if (grid[i][j] != 0) {
      grid[i][j] = 0;
      k--;
    }
  }

}


// depending on the difficulty chosen in the menu
// it converts that into a k amount of digits that will be removed
// from the puzzle :)
// (how much percent of the board will be taken away)
int difficultyToK(const int difficultyNumber) {
  const int totalCells = sizeOfTheBoard * sizeOfTheBoard;
  float percent;

  switch (difficultyNumber) {
    case 1:
      percent = 0.3f;
      break; // easy
    case 2:
      percent = 0.4f;
      break; // moderate
    case 3:
      percent = 0.5f;
      break; // hard
    case 4:
      percent = 0.6f;
      break; // advanced
    default:
      printf("INVALID DIFFICULTY\n");
    exit(-1);
  }

  return (int)(totalCells * percent);
}


int **sudokuGenerator() {
  int ** grid = initializeBoard();

  fillDiagonal(grid);
  fillRemaining(grid, 0, 0);

  const int k = difficultyToK(difficulty);

  removeKDigits(grid, k);

  return grid;
}


// empty spots to fill on the board
int countEmptyCells(int **grid) {
  int count = 0;
  for (int i = 0; i < sizeOfTheBoard; i++) {
    for (int j = 0; j < sizeOfTheBoard; j++) {
      if (grid[i][j] == 0) count++;
    }
  }
  return count;
}


void pauseMenu(int **grid) {
  int choice;
  while (1) {
    printf("\n===== PAUSE MENU =====\n");
    printf("1] Save Game\n");
    printf("2] Return to Main Menu\n");
    printf("3] Continue Game\n");
    printf("4] Exit\n");
    printf("======================\n");
    scanf("%d", &choice);

    switch (choice) {
      case 1:
        saveGameToFile(grid);
        break;
      case 2:
        printf("RETURNING TO MAIN MENU...\n");
        menu();
        break;
      case 3:
        printBoard();
        printf("RESUMING GAME...\n");
        return;
      case 4:
        exit(0);
      default:
        printf("INVALID CHOICE\n");
    }
  }
}


// "lives"
int hearts = 3;
int correctMoves;

// user types in a row. column and value
// validates user info and assigns the value if correct
// otherwise takes a heart and informs of the error
void insertValueToGrid(int **grid) {
  const int totalToFill = countEmptyCells(grid);

  while (correctMoves < totalToFill & hearts > 0) {
    int row, col, value;

    char input2[1];
    printf("ENTER A ROW (1-%d) OR 'p' TO PAUSE: ", sizeOfTheBoard);
    scanf("%s", input2);

    if (input2[0] == 'p' || input2[0] == 'P') {
      pauseMenu(grid);
      continue;
    }

    row = atoi(input2); // atoi() - ASCII to Integer
    row--; // adjusting to 0-based indexing

    printf("ENTER A COLUMN (1-%d): ", sizeOfTheBoard);
    scanf("%d", &col);
    col--;

    if (sizeOfTheBoard == 4) {
      printf("ENTER VALUE (1-4): ");
      scanf("%d", &value);
      // value validation
      if (value < 1 || value > sizeOfTheBoard) {
          printf("INVALID VALUE!\n");
          continue;
        }
    }
    else if (sizeOfTheBoard == 9) {
      printf("ENTER VALUE (1-9): ");
      scanf("%d", &value);
      // value validation
      if (value < 1 || value > sizeOfTheBoard) {
          printf("INVALID VALUE!\n");
          continue;
      }
    }
    else if (sizeOfTheBoard == 16) {
      char input[1];
      printf("ENTER VALUE (1-9) OR (A-F): ");
      scanf("%s", input);

      if (input[0] >= '1' && input[0] <= '9') {
        value = input[0] - '0';
      } else if (input[0] >= 'A' && input[0] <= 'G') {
        value = 10 + (input[0] - 'A');
      } else if (input[0] >= 'a' && input[0] <= 'g') {
        value = 10 + (input[0] - 'a');
      } else {
        printf("INVALID INPUT!\n");
        continue;
      }
    }


    // position validation
    if (row < 0 || row >= sizeOfTheBoard || col < 0 || col >= sizeOfTheBoard) {
      printf("INVALID POSITION!\n");
      continue;
    }

    // so it has to be 0
    // in the board it shows as a dot but in reality it's 0 :D
    if (grid[row][col] != 0) {
      printf("CELL ALREADY FILLED!\n");
      continue;
    }

    if (checkIfSafe(grid, row, col, value)) {
      // if it's safe to do so assign the value to the grid and print the board
      grid[row][col] = value;
      correctMoves++;
      printBoard();
      printf("\nHEARTS: %d | REMAINING SPOTS: %d\n", hearts, totalToFill - correctMoves);
    } else {
      //if it's not -> -1 to hearts
      hearts--;
      printf("WRONG! HEARTS LEFT: %d\n", hearts);

      //game over
      if (hearts <= 0) {
        printf("GAME OVER\n");
        system("pause");
        exit(0);
      }
    }
  }
}

//TODO!!!
void deleteValueFromGrid(int **grid) {

}


// saving array to file to be able to continue a game
void saveGameToFile(int **grid) {
  FILE *f = fopen("game.data", "wb");

  if (!f) {
    printf("ERROR OPENING THE FILE\n");
    return;
  }

  // saving game metadata
  fwrite(&sizeOfTheBoard, sizeof(int), 1, f);
  fwrite(&difficulty, sizeof(int), 1, f);
  fwrite(&hearts, sizeof(int), 1, f);
  fwrite(&correctMoves, sizeof(int), 1, f);

  // writing row by row
  for (int i = 0; i < sizeOfTheBoard; i++) {
    fwrite(grid[i], sizeof(int), sizeOfTheBoard, f);
  }

  fclose(f);
  printf("GAME SAVED SUCCESSFULLY\n");
}


// loading a saved game
int **loadGameFromFile() {
  FILE *f = fopen("game.data", "rb");

  if (!f) {
    printf("ERROR OPENING THE FILE\n");
    return NULL;
  }

  // reading game metadata
  fread(&sizeOfTheBoard, sizeof(int), 1, f);
  fread(&difficulty, sizeof(int), 1, f);
  fread(&hearts, sizeof(int), 1, f);
  fread(&correctMoves, sizeof(int), 1, f);
  boxSize = sqrt(sizeOfTheBoard); // updating box size

  int **grid = initializeBoard(); // initializing the board with 0s

  for (int i = 0; i < sizeOfTheBoard; i++) {
    fread(grid[i], sizeof(int), sizeOfTheBoard, f); //writing the data to the board
  }

  fclose(f);
  return grid;
}


//functions to calculate time of the game :)
time_t start, end;
void startTime(){
  time(&start);
  printf("GAME STARTED AT: %s", ctime(&start));
}

void endTime(){
  time(&end);
  printf("GAME ENDED AT: %s", ctime(&end));
}

void calculateTimeElapsed(){
  const double elapsed = difftime(end, start);
  printf("ELAPSED TIME: %.0f seconds\n", elapsed);
}


void menu() {
  int choice = 0;
  while (1) {
    printf("======MENU======\n");
    printf("1] Tutorial\n");
    printf("2] New Game\n");
    printf("3] Continue\n");
    printf("4] Exit\n");

    scanf("%d", &choice);

    switch (choice) {
      case 1:
        //TODO: function fr this to clean up code
        printf("\n\nSudoku is a logic-based number placement puzzle. "
               "The goal is to fill a nxn grid with numbers 1 through 9, "
               "ensuring that each number appears only once in every row, column, and (usually) a 3x3 subgrid. \n\n");
        printf("BASIC RULES FOR A 9X9 BOARD:\n");
        printf("  1. Grid: The puzzle consists of a 9x9 grid divided into 9 nonets (3x3 subgrids).\n");
        printf("  2. The numbers 1 through 9 must be placed in each row, column, and nonet.\n");
        printf("  3. Each number can only appear once in each row, column, and nonet.\n");
        printf("  4. A Sudoku puzzle will have some numbers pre-filled in the grid, providing a starting point. \n\n\n");

        system("pause");
        break;
      case 2:
        correctMoves = 0;
        hearts = 3;

        boardSize();

        if (sizeOfTheBoard == 0) break;  // if something went wrong with choosing size go -> back to menu

        gameDifficulty();

        if (difficulty == 0) break; // the same

        // changes the “seed” or the starting point of the algorithm. A seed is an integer used to initialize the random number generator.
        // ensure a different sequence of random numbers; ergo a different sudoku puzzle every time
        srand(time(0));
        startTime();
        printf("\n");

        board = sudokuGenerator();

        printBoard(sizeOfTheBoard);

        insertValueToGrid(board);

        printf("\n");
        endTime();
        printf("\nGAME WON!\n\n");
        calculateTimeElapsed();
        system("pause");
        exit(0);
      case 3:
        board = loadGameFromFile();
        if (board != NULL) {
          printBoard(sizeOfTheBoard);
          startTime();
          insertValueToGrid(board);

          printf("\n");
          endTime();
          printf("\nGAME WON!\n\n");
          calculateTimeElapsed();
          system("pause");
          exit(0);
        }
        break;
      case 4:
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
