#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "util.h"
#include "game.h"
#include "sudoku.h"
#include "io.h"


// functions to calculate time of the game :)
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

void tutorial(){
  printf("\n\nSudoku is a logic-based number placement puzzle. "
    "The goal is to fill a nxn grid with numbers 1 through 9, "
    "ensuring that each number appears only once in every row, column, and (usually) a 3x3 subgrid. \n\n");
  printf("BASIC RULES FOR A 9X9 BOARD:\n");
  printf("  1. Grid: The puzzle consists of a 9x9 grid divided into 9 nonets (3x3 subgrids).\n");
  printf("  2. The numbers 1 through 9 must be placed in each row, column, and nonet.\n");
  printf("  3. Each number can only appear once in each row, column, and nonet.\n");
  printf("  4. A Sudoku puzzle will have some numbers pre-filled in the grid, providing a starting point. \n\n\n");
}