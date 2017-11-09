#ifndef SUDOKU_H
#define SUDOKU_H

#include "Searchable.h"
#include <set>
#include <vector>
#include <cmath>

using std::endl;
using namespace std;
using std::set;
using std::cout;
using std::unique_ptr;

// TODO: Your Sudoku class goes here:
class Sudoku: public Searchable{

private:

    vector<vector<set<int>>> incompleteGrid;
    set<int> allowedValues;
    int size;
    int boxSize;
    bool exists = false;

public:

    Sudoku(int sizeIn) : boxSize(sqrt(sizeIn)), size(sizeIn){
        //resize grid
        incompleteGrid.resize(size, vector<set<int>>(size));

        //store allowedValues
        for(int i=0; i< size; ++i) {
            allowedValues.insert(i+1);
        }
        //initialise grid
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

        do {

            incompleteGrid[row][col].clear();
            incompleteGrid[row][col].insert(value);
            exists = true;


            for (int col=0; col<size; col++) {
                for (int row=0; row<size; row++) {

                    if (incompleteGrid[row][col].size()==1) {
                        int rmValue = *incompleteGrid[row][col].begin();
                        if (!eraseRow(rmValue, exists, row, col)) {
                            return false;
                        }
                        if (!eraseCol(rmValue, exists, row, col)) {
                            return false;
                        }
                        if (!eraseBox(rmValue, exists, row, col)) {
                            return false;
                        }

                    }

                    if (incompleteGrid[row][col].size()==0) {
                        return false;
                    }

                    if (incompleteGrid[row][col].size()==2) {
                        eraseSameRowsColumns(row,col);

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

        vector<unique_ptr<Searchable> > tempVector;
        int var1, var2 = -1; int var3 = 10;
        for(auto row = 0; row < size; ++row) {
            for(auto col = 0; col < size; ++col) {
                if(incompleteGrid[row][col].size() != 1) {
                    if (incompleteGrid[row][col].size() < var3) {
                        var1 = row; var2 = col;
                        var3 = incompleteGrid[row][col].size();
                    }
                }
            }
        }

        if(var3 != 10) {
            for(auto cell : incompleteGrid[var1][var2]) {
                Sudoku *copySudoku = new Sudoku(*this);
                bool result = copySudoku->setSquare(var1, var2, cell);
                if (result) {
                    tempVector.push_back(unique_ptr<Searchable>(copySudoku));
                } else {
                    delete copySudoku;
                }
            }
            if(tempVector.size() == 1) {
                if (!tempVector.front()->isSolution()) {
                    unique_ptr<Searchable> heir (std::move(tempVector.front()));
                    tempVector.pop_back();
                    tempVector = heir->successors();
                }
            }
            return tempVector;
        }

    }

    virtual int heuristicValue() const override{
        int heuristicValue = 0;
        for(auto i = 0; i < size; ++i) {
            for(auto j = 0; j < size; ++j) {
                if(incompleteGrid[i][j].size() > 1){
                    ++heuristicValue;
                }
            }
        }
        return heuristicValue;
    }

    virtual void write(ostream & os) const override{
    // os << "-------------------------------------------------------" << endl;
    //   for (int row = 0; row < size; row++) {
    //       for (int col=0; col< size; col++) {
    //           os << "| ";
    //           for (int element: incompleteGrid[row][col]) {
    //               os << " :" << element << ": ";
    //               if (!((col+1) % size))
    //                   os << " | ";
    //           }
    //           if (!((row+1) %boxSize))
    //               os << endl << "-------------------------------------------------------" << endl;
    //           else
    //               os << " ";
    //     }
    //   }
    }

    bool eraseRow(const int & rmValue, bool exists, const int row, const int col) {
        for (int index = 0; index < size; ++index) {
            if (index != col) {
                if (incompleteGrid[row][index].find(rmValue) != incompleteGrid[row][index].end()) {
                    incompleteGrid[row][index].erase(rmValue);
                    this->exists = false;
                    // for (int val : copyOfGrid) {
                    //     cout << val << ", ";
                    // }
                    // cout << endl;
                }
            }

            if (incompleteGrid[row][index].size() == 0) {
                return false;
            }
        }
        return true;
    }

    bool eraseCol(const int & rmValue, bool exists, const int row, const int col) {
        for (int index = 0; index < size; ++index) {
            if (index != row) {
                if (incompleteGrid[index][col].find(rmValue) != incompleteGrid[index][col].end()) {
                    incompleteGrid[index][col].erase(rmValue);
                    this->exists = false;
                    // for (int val : copyOfGrid) {
                    //     cout << val << ", ";
                    // }
                    // cout << endl;
                }
            }

            if (incompleteGrid[index][col].size() == 0) {
                return false;
            }
        }
        return true;
    }



    bool eraseBox(const int & rmValue, bool exists, const int row, const int col) {
        int endRow = row%boxSize;
        int startBoxRow = row - endRow;
        int endBoxRow = startBoxRow + boxSize;
        int endCol = col%boxSize;
        int startBoxCol = col - endCol;
        int endBoxCol = startBoxCol + boxSize;


        for (int i=startBoxRow; i<endBoxRow; i++) {
            for (int j=startBoxCol; j<endBoxCol; j++) {

                if (i!=row) {
                    if (j!=col) {
                        if(incompleteGrid[i][j].find(rmValue) != incompleteGrid[i][j].end()) {
                            incompleteGrid[i][j].erase(rmValue);
                            this->exists = false;

                            if (incompleteGrid[i][j].size() == 0) {
                                return false;
                            }
                        }
                    }
                }
            }
        }
        return true;
    }


    bool eraseSameRowsColumns(const int row, const int col) {
        for (int k = 0; k<size; ++k) {

            if ((incompleteGrid[row][col] == incompleteGrid[row][k]) && (k != col)) {
                auto point = incompleteGrid[row][col].begin();
                int num = *point;
                ++point;
                int num2 = *point;
                int column = 0;
                while (column < size) {
                    if ((column != col) && (column != k)) {
                        incompleteGrid[row][column].erase(num);
                        if (incompleteGrid[row][column].size() == 0) {
                            return false;
                        }
                        incompleteGrid[row][column].erase(num2);
                        if (incompleteGrid[row][column].size() == 0) {
                            return false;
                        }
                    }
                    ++column;
                }
            }

            if ((incompleteGrid[row][col] == incompleteGrid[k][col]) && (k != row)) {
                auto point2 = incompleteGrid[row][col].begin();
                int num3 = *point2;
                ++point2;
                int num4 = *point2;
                int rRow = 0;
                while (rRow < size) {
                    if ((rRow != row) && (rRow != k)) {
                        incompleteGrid[rRow][col].erase(num3);
                        if (incompleteGrid[rRow][col].size() == 0) {
                            return false;
                        }
                        incompleteGrid[rRow][col].erase(num4);
                        if (incompleteGrid[rRow][col].size() == 0) {
                            return false;
                        }

                    }
                    ++rRow;
                }

            }

        }

    }

};

#endif
