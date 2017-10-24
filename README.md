# Sudoku

In this coursework you will be working on writing code that will solve Sudoku puzzles.

To avoid setting a deadline for this work during reading week, and so everyone has a lab session before the deadline, both parts of this assignment are due in on *Thursday the 9th of November at 11pm*.

As with the previous two assignments, part (d) is advanced and worth 3 marks; parts (a) to (c) add up to 7 marks.

For the avoidance of doubt -- you may add extra `#include` and `using` statements this week, if you need to.

# a) Making a Sudoku board class

In the file `Sudoku.h` make a class `Sudoku` that holds an *incomplete* [Sudoku](https://en.wikipedia.org/wiki/Sudoku) solution.

It should have a constructor that takes a single argument -- the size of the board.  For instance, for a 9x9 Sudoku, the constructor would be given the value 9.

You need to store the incomplete solution as a member variable. The *recommended* way to do this is to have a vector of vectors (a square array), in which each square is represented as a `set<int>` that holds the values that could possibly go in that square.  Initially, for a 9x9 Sudoku, if the grid is completely blank, each set will contain the values `{1,2,3,4,5,6,7,8,9}`.  When a square is given some value, the set is cleared and replaced with a set containing just that one value -- the other options are removed.

Write a function `getSquare(int row, int col)` that returns the value in the cell in the square at the given row and column:

- If there is only one value in the set for that square, return the number in that set.
- Otherwise, return -1 (a dummy value to indicate we don't know what should be in that square yet)

# b) Setting the value of a Sudoku square

Write a function `setSquare(int row, int col, int value)` that sets the value in the cell in the square at the given row and column, then updates the sets of possible values in the rest of the grid to remove choices that have been eliminated.  For instance, if we put a '3' on a given row, then nothing else on that row can have the value 3.

The implementation of setSquare is split into two parts.

First, the easy part: the set of possible values for that cell is cleared, and `value` is inserted.  This forces that cell to have that value.

Then, a loop begins that does the following:

- Loop over the entire grid
- For each square that has only one value in it, remove that value from the sets of possible values for:
 - All the other squares on that row
 - All the other squares in that column
 - All the other squares in the same *box*.  A 9x9 grid is divided into 9 boxes, each 3x3: no two values in the same box can have the same value.  For larger grids (e.g. 16x16), the size of the box is always the square root of the size of the grid.
 

If at any point the set of values for a square becomes empty, the function should return false: it has been shown that there is no value that can go in a square.

The loop should continue whilst values are still being removed from the sets of possible values.  The reason for this is that after setting the given square, we might end up with only one option being left for some other squares on the grid.  For instance, if on a given row some of the squares had the sets:

`{3,4} {3,5}  {4,5}`

...and we call setSquare to set the middle square to have the value 3, then before the loop:

`{3,4} {3}  {4,5}`

On the first pass of the loop, we would find the square containing `3` and remove this from the other sets on the row (and the other sets in the same column and box).  The row then looks like:

`{4} {3}  {4,5}`

We then start the loop again, and find the square containing the value '4'.  This is removed from the other sets on the row (and column and box) to give:

`{4} {3} {5}`

We then start the loop again, and find the square containing the value '5'.

This process stops when, having looped over the board, and updated the sets by removing values, our sets have stopped getting any smaller.  At this point the function returns true.

For simple Sudodu puzzles, this process here is enough to solve the puzzle.  No guesswork is needed: setting the squares of the board to hold the initial values specified in the puzzle, is enough to cause all the other squares of the board to have only one option left.

You can test this by compiling and running BasicSudoku.cpp:

`g++ -std=c++11 -g -o BasicSudoku BasicSudoku.cpp`

This calls `setSquare` for the values in a simple Sudoku puzzle; then uses `getSquare` to check your code has the right answer.  

# c) Searching for a solution

For more complex puzzles, after putting in the initial values using setSquare, some of the squares on the board have more than one value left in their set of possible values -- using logic alone, we cannot deduce what the value has to be; we have to make a guess and see what happens.

For this, we are going to use the Searchable class.  This is an *abstract class* for puzzles, containing the following virtual functions:

- `isSolution()`: this returns true if the puzzle has been solved.  For Sudoku, this means all the squares contain just one value.
- `write(ostream & o)`: a debugging function to print the board to screen.
- `heuristicValue()`: an estimate of how far the puzzle is from being solved.  We will return to this in part (d)
- `successors()`: in a situation where a guess is needed, this returns several new puzzle objects, each of which corresponds to a different guess having been made.


Make your Sudoku class inherit from Searchable, by changing the opening of the class definition to `class Sudoku : public Searchable`

Implement isSolution() to only return true if the puzzle has been solved; i.e. every set in every square is of size 1.

Implement a write() function to print the board.  You can display the board however you like.  A reasonable implementation is to print out the board one row at a time:

- If the square has more than one value in its set, print a space character
- Otherwise, print the value from the set.

(I'm not marking write(), it is to help your debugging.  If you want to print nothing at all, that's fine.)

Implement successors() as follows:

- Make an empty vector of successors to return.  This should be a `vector<unique_ptr<Searchable> > ` object.
- Find the first row containing a square that still has more than one option in its set
- Find the left-most square on that row
- For each value in the set for that square:
 - Make a *copy* of the current Sudoku object (this) using new
 - Use setSquare on the *copy* to set the value of the square
 - If setSquare returns true, add the pointer to the back of the vector of successors to return.  Otherwise, delete the pointer.  (You can use a unique_ptr for this if you wish.)

- Once done, return the vector of successors

Once you have implemented these functions, you can test your code by using BreadthFSSudoku:

`g++ -std=c++11 -g -o BreadthFSSudoku BreadthFSSudoku.cpp`

# d) Advanced

We can improve on the basic sudoku solver in a few ways.   Your mark for this part will be based on how quickly your solution works: the faster it runs, the higher the marks.  As well as 9x9 boards, I will be testing your solution on larger (e.g. 16x16) Sudoku boards.

## A better setSquare

Suppose we had the following squares on a row:

`{3,4} {3,4,6} {3,4,6,7} {3,4}`

There are *2* cells that contain identical sets of size *2*.  There are only two ways of satisfying this:

- 3 in the left-most cell; 4 in the right-most cell
- 4 in the left-most cell; 3 in the right-most cell

In either case, no other value on the row can have the value 3 or 4.  We can thus remove 3 and 4 from the other sets:

`{3,4} {6} {6,7} {3,4}`

As before, this process loops and carries on until the sets stop getting any smaller -- because there is a cell that contains just 6, we would then get:

`{3,4} {6} {7} {3,4}`

Extend the implementation of setSquare to include this sort of reasoning for duplicates sets of values in rows, columns and boxes: look for pairs of squares, with identical sets of size 2.

To test your code, compile and run BreadthFSSudoku again -- it should explore fewer nodes than it did before you made these changes.

If you wish, you can then continue this work to add additional logic to setSquare.  For instance, looking for 3 identical sets of size 3, 4 identical sets of size 4, etc.  Or, any other deduction rule for Sudoku that you can find online, and implement in your work -- add a comment to the website or paper you used as your source. 

## Better search with a heuristic

Open up `BreadthFirstSearch.h`.  It defines a search strategy known as breadth-first search.  It has a queue of incomplete solutions (initially, the starting configuration of the board.  Then, it repeatedly takes a board off the queue, and if it's not a solution, gets its successors, and puts them on the queue.

The queue used follows a `first in, first out` (FIFO) strategy: the next board is always taken off the front of the queue; and new boards are always put on the back of the queue.

Breadth-first search can be improved by using a heuristic: an estimate of how close a board is to being finished.  The `Searchable` class defines a `heuristicValue()` function that calculates an estimate of this.

Add a `heuristicValue()` function to your Sudoku class, that returns the number of squares on the board whose sets are of size greater than 1.  On paper this corresponds to the number of squares we haven't written a number into yet; and the fewer the squares, the closer it looks like that board is to being a solution.

Complete the `BestFirstSearch` class provided, so that instead of using a first-in first-out queue (as in breadth-first search) it uses a priority queue, sorted in ascending order of heuristic value.  That is, when we get the successors to a board, these are inserted into the queue so that the board with the smallest heuristic value is always at the front.

To test your code, compile and run BestFSSudoku:

`g++ -std=c++11 -g -o BestFSSudoku BestFSSudoku.cpp`

This should expand fewer nodes than breadth-first search

NB: Your BestFirstSearch code should never assume `Searchable` objects are Sudoku objects.  Only use functions defined in the Searchable base class.

## A better successors function

One last edit.  In the successors function, we choose a square, and make successors corresponding to setting that square to have each of its possible values.  We keep only the successors for which setSquare returned true.

If when we do this, we keep only one successor -- and that successor isn't a solution -- then instead of returning a vector of just that one successor, recursively call `successors()` on that; and return what *it* returns.

The intuition for this is as follows.  If setSquare only returned true for one of the possible successors, we have shown that, actually, having tried all the options, only one of the possible values for that square was acceptable.  Thus, we can instead return the successors of that one valid option.  We only need to return successors to search, to go on the queue, if there is more than one option to choose from.

As with before, implementing this should allow both breadth-first search and best-first search to expand fewer nodes.
