# SUDOKU IN C!
i lost a lot of time and patience over this.. sob<3

note: this is my first 'bigger' project in c so there's bound to be (a lot of) mistakes in the code😁

## ANYWAY! HOW IT ALL WORKS:
<pre>
  main menu: tutorial, new game, continue (a game), exit
    1. tutorial: quick text about the sudoku game as a whole
    2. new game:
      - menu for choosing size of the board; 4x4, 9x9, 16x16
      - menu for choosing difficulty; easy, moderate, hard, advanced
      - next the sudoku board is generated using an algorythm
      - user has to type in: row, column and value to solve the given board
      - user has 3 hearts (lives, mistakes), -1 for every wrongly placed value on the board, 0 = game over
      - during the game user can go into pause menu to: save the game to a file, continue playing and to exit safely
    3. continue:
      a board is loaded from the game.data file with a saved sudoku puzzle
      user can choose to solve it at any time that way
    4. exit: 
      safe exit from the app
</pre>
## GAME PREVIEW
![image](https://github.com/user-attachments/assets/4505b6ac-0c50-4854-b160-379e4a9abb35)

## PROBLEMS I HAD
- the board for 4x4 hard difficulty sometimes doesn't generate at all BUT ONLY SOMETIMES??? i have no idea why.
- my guess it has something to do with the removing of the cells but i've changed how it works over 4 times and it's still the same so i give up on that
- for the user to type in A-F in 16x16 board,, i hate ASCII with a passion<3
- HEADER FILES! creating my own didn't work for soooo long on clion:') i've switched to vscode in the end and divided the code :D

##

![image](https://github.com/user-attachments/assets/c981f70c-860a-4622-a332-a812f7f3bd20)

