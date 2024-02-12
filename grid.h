//-------------------------------------------------
// TODO:  Copy in your Grid class OR replace this
//          file with grid.h from Project 06.
//          This class is needed for this Project,
//          but it is not tested independently.
//-------------------------------------------------

/*-------------------------------------------
Program 7: Outlast the Baddies & Avoid the Abyss
Course: CS 211, Fall 2023, UIC
System: Clion/Zybooks
Author: Evelyn Johnson
------------------------------------------- */


#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>
#include <algorithm>

using namespace std;

template<typename T>
class Grid {
private:
    struct ROW {
        T*  Cols;     // array of column elements
        size_t NumCols;  // total # of columns (0..NumCols-1)
    };

    ROW* Rows;     // array of ROWs
    size_t  NumRows;  // total # of rows (0..NumRows-1)

public:
    //
    // default constructor:
    //
    // Called automatically by to construct a 4x4 Grid.
    // All elements initialized to default value of T.
    //
    Grid() {
        Rows = new ROW[4];  // 4 rows
        NumRows = 4;

        // initialize each row to have 4 columns:
        for (size_t r = 0; r < NumRows; ++r) {
            Rows[r].Cols = new T[4];
            Rows[r].NumCols = 4;

            // initialize the elements to their default value:
            for (size_t c = 0; c < Rows[r].NumCols; ++c) {
                Rows[r].Cols[c] = T();  // default value for type T:
            }
        }
    }

// TODO:  Update the member function comments in your own style.

    // TODO:  Write this parameterized constructor.
    // parameterized constructor:
    //
    // Called automatically to construct a Grid
    // with R rows, where each row has C columns.
    // All elements initialized to default value of T.
    Grid(size_t R, size_t C) {
        Rows = new ROW[R];  // R rows
        NumRows = R;

        // initialize each row to have C columns:
        for (size_t r = 0; r < NumRows; ++r) {
            Rows[r].Cols = new T[C];
            Rows[r].NumCols = C;

            // initialize the elements to their default value:
            for (size_t c = 0; c < Rows[r].NumCols; ++c) {
                Rows[r].Cols[c] = T();  // default value for type T:
            }
        }
    }


    // TODO:  Write this destructor.
    // destructor:
    //
    // Called automatically to free memory for this Grid.
    virtual ~Grid() {
        // Free memory for each row's columns
        for (size_t r = 0; r < NumRows; ++r) {
            delete[] Rows[r].Cols;
        }

        // Free memory for rows
        delete[] Rows;
    }


    // TODO:  Write this copy constructor.
    // copy constructor:
    //
    // Called automatically to construct a Grid that contains a
    // copy of an existing Grid.  Example: this occurs when passing
    // Grid as a parameter by value
    //
    //   void somefunction(Grid<int> G2)  <--- G2 is a copy:
    //   { ... }
    Grid(const Grid<T>& other) {
        NumRows = other.NumRows;
        Rows = new ROW[NumRows];

        // Copy each row's columns
        for (size_t r = 0; r < NumRows; ++r) {
            Rows[r].NumCols = other.Rows[r].NumCols;
            Rows[r].Cols = new T[Rows[r].NumCols];

            // Copy elements
            for (size_t c = 0; c < Rows[r].NumCols; ++c) {
                Rows[r].Cols[c] = other.Rows[r].Cols[c];
            }
        }
    }


    // TODO:  Write this copy operator.
    // copy operator=
    //
    // Called when one Grid is assigned into another, i.e. this = other;
    Grid& operator=(const Grid& other) {
        // Check for self-assignment
        if (this != &other) {
            // Free memory for current object
            for (size_t r = 0; r < NumRows; ++r) {
                delete[] Rows[r].Cols;
            }
            delete[] Rows;

            // Copy data from other object
            NumRows = other.NumRows;
            Rows = new ROW[NumRows];

            for (size_t r = 0; r < NumRows; ++r) {
                Rows[r].NumCols = other.Rows[r].NumCols;
                Rows[r].Cols = new T[Rows[r].NumCols];

                for (size_t c = 0; c < Rows[r].NumCols; ++c) {
                    Rows[r].Cols[c] = other.Rows[r].Cols[c];
                }
            }
        }

        return *this;
    }


    // TODO:  Write this function.
    // numrows:
    //
    // Returns the # of rows in the Grid.
    // The indices for these rows are 0..numrows-1.
    size_t numrows() const {
        return NumRows;
    }


    // TODO:  Write this function.
    // numcols:
    //
    // Returns the # of columns in row r.
    // The indices for these columns are 0..numcols-1.
    // For now, each row has the same number of columns.
    size_t numcols(size_t r) const {
        if (r < NumRows) {
            return Rows[r].NumCols;
        }
        throw out_of_range("Invalid row index");
    }


    // TODO:  Write this function.
    // size
    //
    // Returns the total # of elements in the Grid.
    size_t size() const {
        size_t totalSize = 0;
        for (size_t r = 0; r < NumRows; ++r) {
            totalSize += Rows[r].NumCols;
        }
        return totalSize;
    }


    // TODO:  Write the parentheses overloaded operator
    // ():
    //
    // Returns a reference to the element at location (r, c);
    // this allows you to access or assign the element:
    //
    //    grid(r, c) = ...
    //    cout << grid(r, c) << endl;
    T& operator()(size_t r, size_t c) {
        if (r < NumRows && c < Rows[r].NumCols) {
            return Rows[r].Cols[c];
        }
        throw out_of_range("Invalid indices");
    }


    // TODO:  Write this function.
    // _output
    //
    // Outputs the contents of the grid; for debugging purposes. This is not tested.
    void _output() {
        for (size_t r = 0; r < NumRows; ++r) {
            for (size_t c = 0; c < Rows[r].NumCols; ++c) {
                cout << Rows[r].Cols[c] << ' ';
            }
            cout << endl;
        }
    }

};
