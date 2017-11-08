#ifndef SUDOKU_H
#define SUDOKU_H

#include "Searchable.h"
#include <set>
#include <vector>
// #include <string>
// #include <iostream>
#include <cmath>

using namespace std;
using std::unique_ptr;
using std::set;
using std::endl;
// using std::vector;
using std::cout;
// using std::string;
// using std::iterator;

// TODO: Your Sudoku class goes here:
class Sudoku: public Searchable{

private:

    vector<vector<set<int>>> incompleteGrid;
    set<int> allowedValues;
    int size;
    int boxSize;

public:

    Sudoku(int sizeIn) : boxSize(sqrt(sizeIn)), size(sizeIn){

        incompleteGrid.resize(size, vector<set<int>>(size));

        for(int i=0; i< size; ++i) {
            allowedValues.insert(i+1);
        }

         for (int col=0; col<size; ++col) {
            for (int row=0; row< size; ++row) {
                incompleteGrid[row][col]=allowedValues;
            }
         }
    }



    int getSquare(int row, int col) {
        if(incompleteGrid[row][col].size()==1) {
            return *incompleteGrid[row][col].begin();
        } else {

            //cout << "BFFS goes here" << endl;
            return -1;
        }

    }


    bool setSquare(int row, int col, int value) {

        bool exists = false;
        do {

            incompleteGrid[row][col].clear();
            incompleteGrid[row][col].insert(value);

            exists = true;

            for (int col=0; col<size; col++) {
                for (int row=0; row<size; row++) {

                    if (incompleteGrid[row][col].size()==1) {

                        int index=0;
                        while (index<size) {

                            set<int> copyOfGrid2= incompleteGrid[row][index];
                            int rmValue2= *incompleteGrid[row][col].begin();

                            if (index != col) {
                                if (copyOfGrid2.find(rmValue2) != copyOfGrid2.end()) {
                                    copyOfGrid2.erase(rmValue2);
                                    exists = false;
                                    incompleteGrid[row][index] = copyOfGrid2;
                                    // for (int val : copyOfGrid) {
                                    //     cout << val << ", ";
                                    // }
                                    // cout << endl;
                                }
                            }
                            
                            eraseRowCol(index, row, col, exists);



                            if (incompleteGrid[row][index].size() == 0) {
                                return false;
                            }


                            eraseBox(row, col, exists);
                            ++index;
                        }
                    }
                }
            }

        } while (exists == false);

        return true;

    }

    virtual bool isSolution() const override {

        for (int col=0; col<size; col++) {
            for (int row=0; row<size; row++) {
                if (incompleteGrid[row][col].size()!=1) {
                    return false;
                }
            }
        }
        return true;
    }

    virtual vector<unique_ptr<Searchable>> successors() const override {

        vector<unique_ptr<Searchable>> tempVector;

        for (int row=0; row<size; ++row) {
            for(int col=0; col<size; ++col) {
                if (incompleteGrid[row][col].size()>1) {

                    for (int value: incompleteGrid[row][col]) {

                        Sudoku *copySudoku = new Sudoku(*this);
                        bool result = copySudoku->setSquare(row,col,value);
                        if (result) {
                            tempVector.push_back(unique_ptr<Searchable>(copySudoku));
                        }
                        else {
                            delete copySudoku;
                        }
                    }
                    return tempVector;
                }
            }
        }

    }

    virtual void write(ostream & os) const override{
        os << "-------------------------------------------------------" << endl;
      for (int row = 0; row < size; row++) {
          for (int col=0; col< size; col++) {
              os << "| ";
              for (int element: incompleteGrid[row][col]) {
                  os << " *" << element << "* ";
                  if (!((col+1) % size))
                      os << " | ";
              }
              if (!((row+1) %boxSize))
                  os << endl << "-------------------------------------------------------" << endl;
              else
                  os << " ";
        }
      }



        // for (int i=0; i<size; ++i) {
        //     for (int j=0; j<size; ++j) {
        //         os <<"  |  ";
        //         for (int element: incompleteGrid[i][j]) {
        //             os <<" "<<element<<" ";
        //         }
        //         os<<"  |  ";
        //     }
        //     os<<endl;
        // }
    }

    virtual int heuristicValue() const override{
        for (int i=0; i<size; ++i) {
            for (int j=0; j<size; ++j) {
                if (incompleteGrid[i][j].size() > 1) {
                    cout << incompleteGrid[i][j].size();
                    return incompleteGrid[i][j].size();

                }

            }
        }
    }

    bool eraseRowCol(int index, int row, int col, bool exists) {
        set<int> copyOfGrid= incompleteGrid[index][col];
        int rmValue= *incompleteGrid[row][col].begin();

        if (index != row) {
            if (copyOfGrid.find(rmValue) != copyOfGrid.end()) {
                copyOfGrid.erase(rmValue);
                exists = false;
                incompleteGrid[index][col] = copyOfGrid;
            }
        }

        if (incompleteGrid[index][col].size() == 0) {
            return false;
        }

    }

    bool eraseColRow(int index, int row, int col, bool exists) {

    }




    bool eraseBox(int row, int col, bool exists) {
        int endRow = row%boxSize;
        int startBoxRow = row - endRow;
        int endBoxRow = startBoxRow + boxSize;
        int endCol = col%boxSize;
        int startBoxCol = col - endCol;
        int endBoxCol = startBoxCol + boxSize;


        for (int i=startBoxRow; i<endBoxRow; i++) {
            for (int j=startBoxCol; j<endBoxCol; j++) {

                set<int> copyOfGrid= incompleteGrid[i][j];
                int rmValue= *incompleteGrid[row][col].begin();

                if (i!=row) {
                    if (j!=col) {
                        if(copyOfGrid.find(rmValue) != copyOfGrid.end()) {
                            copyOfGrid.erase(rmValue);
                            exists = false;
                            incompleteGrid[i][j] = copyOfGrid;

                            if (incompleteGrid[i][j].size() == 0) {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }




};

#endif
