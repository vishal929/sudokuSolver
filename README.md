# Welcome to my Sudoku Solver
This is just a fun side project I wanted to do to implement my own sudoku solver in c++. I also included a test in the repo to solve a sudoku problem I found online.

## Algorithm

1) First we go through the list of "givens" and initialize them in our board. This involves reducing the list of possible numbers each entry can take in the same column, row, and block as our givens.
2) Then, we iterate through the entire board and see which unassigned entry has the least number of possibilities for its entry. If the unassigned entry has 0 possibilities, we made a mistake somewhere and need to backtrack (in this case jump to step 4). If every entry is assigned, then we are done.
3) Then, we set this entry to the lowest possible number and reduce the list of possible numbers each entry can take in the same column, row, and block as the entry we just set and we push the index of the now set entry to our operations stack.
4) We pop the last entry modified from the operations stack. Then, we remove the value from the entry, and add to the possibilities list of every entry in the same row,column, and block as the entry we just un-assigned (with theproper row,column, and block checks for error cases).
5) If the next possible value can be assigned, we assign it and move back to step 2. Otherwise, we repeat step 4. 

## Running the algorithm
I was testing my project with the GNU compiler on my Linux system, g++.
I ran my project by typing in "g++ sudokuSolver.cpp" to compile the project, and then I ran the executable with "./a.out".

If you wish to try the test that I have included with the project, instead of running the  executable with "./a.out", we can feed the test to standard input by running the executable with "./a.out < test.txt".

##Please let me know if there are problems/ errors. 
I am always willing to listen to criticism, concerns, and advice so that I can be a better developer. Please do not hesitate to email me or send me a message about any issues with this program. If there are any suggestions for future projects which I can do, please also send me a message for those as well.
