#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// TODO: ADD GOING BACK IN MENU :D

// 1 - EASY, 2 - MODERATE, 3 - HARD, 4 - ADVANCED
int difficulty = 0;
// 3X3, 4X4, 9X9, 16X16
int sizeOfTheBoard= 0;

int ** board;

// dynamic memory allocation since the user can choose the size of the board
void initializeBoard(const int sizeOfTheBoard) {
  board = (int **)malloc(sizeOfTheBoard * sizeof(int *));

  if (board == NULL) {
    perror("FAILED TO ALLOCATE BOARD ROWS");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < sizeOfTheBoard; i++) {
    board[i] = (int *)malloc(sizeOfTheBoard * sizeof(int));

    if (board[i] == NULL) {
      perror("FAILED TO ALLOCATE BOARD COLUMNS");
      exit(EXIT_FAILURE);
    }

    // filling the board with 0s
    for (int j = 0; j < sizeOfTheBoard; j++) {
      board[i][j] = 0;
    }

  }

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
  int choice;
  printf("======NEW GAME======\n");
  printf("CHOOSE SIZE OF THE BOARD\n");
  printf("1] 3x3\n");
  printf("2] 4x4\n");
  printf("3] 9x9\n");
  printf("4] 16x16\n");
  scanf("%d", &choice);

  const int boardSizeOptions[] = {3, 4, 9, 16};
  if (choice < 1 || choice > 4) {
    printf("CHOOSE A VALID OPTION\n");
    return;
  }

  sizeOfTheBoard = boardSizeOptions[choice-1];
  initializeBoard(sizeOfTheBoard);
}


void gameDifficulty() {
  printf("======NEW GAME======\n");
  printf("CHOOSE DIFFICULTY\n");
  printf("1] EASY\n");
  printf("2] MODERATE\n");
  printf("3] HARD\n");
  printf("4] ADVANCED\n");

  int choice = 0;
  scanf("%d", &choice);

  const int difficultyOptions[] = {1, 2, 3, 4};

  if (choice < 1 || choice > 4) {
    printf("CHOOSE A VALID OPTION\n");
    return;
  }

  difficulty = difficultyOptions[choice-1];
  printf("DIFFICULTY CHOSEN: %d\n", difficulty);
}

//SUDOKU FUNCTIONS



void menu() {
  printf("======MENU======\n");
  printf("1] TUTORIAL*\n");
  printf("2] NEW GAME\n");
  printf("3] CONTINUE\n");
  printf("4] SEE PREVIOUS GAMES\n");
  printf("5] EXIT\n");

  int choice = 0;
  scanf("%d", &choice);

  switch (choice) {
    case 1:
      printf("WILL ADD IT\n");
      break;
    case 2:
      pause();
      boardSize();
      pause();
      gameDifficulty();
      pause();
      printBoard(sizeOfTheBoard);
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
