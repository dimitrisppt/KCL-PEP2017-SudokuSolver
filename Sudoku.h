#ifndef SUDOKU_H
#define SUDOKU_H

#include "Searchable.h"
#include <set>
#include <vector>
#include<string>
#include<iostream>
#include <cmath>

using namespace std;
using std::unique_ptr;
using std::set;
using std::endl;
using std::vector;
using std::cout;
using std::string;
using std::iterator;

// TODO: Your Sudoku class goes here:
class Sudoku {

private:

  vector<vector<set<int>>> incompleteGrid;
  set<int> allowedValues;
  int size;
  int boxSize;

public:

  Sudoku(int sizeIn) : boxSize(sqrt(sizeIn)), size(sizeIn){
    for(int i=0; i< size; ++i) {
      allowedValues.insert(i+1);
    }
  }



  int getSquare(int row, int col) {
    if(incompleteGrid[row][col].size()!=1) {
      return -1;
    } else {
      return *incompleteGrid[row][col].begin();
    }

  }

  bool setSquare(int row, int col, int value) {
    incompleteGrid[row][col].clear();
    incompleteGrid[row][col].insert(value);

    // Loop over the entire grid
    for (int col=0; col<size; col++) {
      for (int row=0; row<size; row++) {

         if (incompleteGrid[row][col].size()==1) {

            for(int i= 0; i< size; ++i) {
              int removeValue= *incompleteGrid[row][col].begin();
              set<int> tempSet= incompleteGrid[row][i];


            }

        }
      }
    }
  }




};

#endif
