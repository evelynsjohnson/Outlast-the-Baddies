#ifndef _GAMEBOARD_H
#define _GAMEBOARD_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <stdexcept>

#include "boardcell.h"
#include "grid.h"

using namespace std;

class GameBoard {
private:
    Grid<BoardCell*> board;
    size_t numRows;
    size_t numCols;
    int HeroRow; // Hero's position row
    int HeroCol; // Hero's position column
    int numMonsters;
    int numSuperMonsters;
    int numAbysses;
    int numBats;
    bool wonGame; // false, unless the Hero reached the exit successfully


public:
    /* default constructor */
    GameBoard() {
        numMonsters = 4;
        numSuperMonsters = 2;
        numAbysses = 50;
        numBats = 2;
        wonGame = false;

        this -> numRows = 15;
        this -> numCols = 40;

        Grid<BoardCell*> boardnew(numRows, numCols);
        board = boardnew;

        blankBoard();
    }

    /* param constructor */
    GameBoard(size_t numRows, size_t numCols) {
        numMonsters = 4;
        numSuperMonsters = 2;
        numAbysses = 20;
        numBats = 3;
        wonGame = false;

        this -> numRows = numRows;
        this -> numCols = numCols;

        Grid<BoardCell*> boardnew(numRows, numCols);
        board = boardnew;

        blankBoard();
    }

    /* destructor */
    virtual ~GameBoard() {
        for (size_t row = 0; row < board.numrows(); row++) {
            for (size_t col = 0; col < board.numcols(row); col++) {
                delete board(row, col);
            }
        }
    }

    void blankBoard() {
        for (size_t row = 0; row < board.numrows(); row++) {
            for (size_t col = 0; col < board.numcols(row); col++) {
                board(row, col) = new Nothing(row,col);
            }
        }
    }

    char getCellDisplay(size_t r, size_t c) {
        return board(r,c)->display();
    }

    void setCell(BoardCell* myCell, size_t r, size_t c) {
        board(r,c) = myCell;
    }

    void freeCell(size_t r, size_t c) {
        delete board(r,c);
    }

    // fills board with by randomly placing...
    //  - Hero (H) in the first three columns
    //  - EscapeLadder (*) in last three columns
    //  - 3 vertical Walls (+), each 1/2 of board height, in middle segment
    //  - Abyss cells (#), quantity set by numAbysses, in middle segment
    //  - Baddies [Monsters (m), Super Monsters (M), & Bats (~)] in middle segment;
    //    number of Baddies set by numMonsters, numSuperMonsters, & numBats
    void setupBoard(int seed) {
        srand(seed);
        size_t r,c;

        r = rand() % numRows;
        c = rand() % 3;
        delete board(r,c);
        board(r,c) = new Hero(r,c);
        HeroRow = r;
        HeroCol = c;

        r = rand() % numRows;
        c = numCols - 1 - (rand() % 3);
        delete board(r,c);
        board(r,c) = new EscapeLadder(r,c);

        int sizeMid = numCols - 6;

        c = 3 + (rand() % sizeMid);
        for (r = 0; r < numRows/2; ++r) {
            delete board(r,c);
            board(r,c) = new Wall(r,c);
        }
        size_t topc = c;

        while (c == topc || c == topc-1 || c == topc+1) {
            c = 3 + (rand() % sizeMid);
        }
        for (r = numRows-1; r > numRows/2; --r) {
            delete board(r,c);
            board(r,c) = new Wall(r,c);
        }
        size_t botc = c;

        while (c == topc || c == topc-1 || c == topc+1 || c == botc || c == botc-1 || c == botc+1) {
            c = 3 + (rand() % sizeMid);
        }
        for (r = numRows/4; r < 3*numRows/4; ++r) {
            delete board(r,c);
            board(r,c) = new Wall(r,c);
        }

        for (int i = 0; i < numMonsters; ++i) {
            r = rand() % numRows;
            c = 3 + (rand() % sizeMid);
            while (board(r,c)->display() != ' ') {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
            }
            delete board(r,c);
            board(r,c) = new Monster(r,c);
            board(r,c)->setPower(1);
        }

        for (int i = 0; i < numSuperMonsters; ++i) {
            r = rand() % numRows;
            c = 3 + (rand() % sizeMid);
            while (board(r,c)->display() != ' ') {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
            }
            delete board(r,c);
            board(r,c) = new Monster(r,c);
            board(r,c)->setPower(2);
        }

        for (int i = 0; i < numBats; ++i) {
            r = rand() % numRows;
            c = 3 + (rand() % sizeMid);
            while (board(r,c)->display() != ' ') {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
            }
            delete board(r,c);
            board(r,c) = new Bat(r,c);
        }

        for (int i = 0; i < numAbysses; ++i) {
            r = rand() % numRows;
            c = 3 + (rand() % sizeMid);
            while (board(r,c)->display() != ' ') {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
            }
            delete board(r,c);
            board(r,c) = new Abyss(r,c);
        }
    }

    // neatly displaying the game board
    void display( ) {
        cout << '-';
        for (size_t col = 0; col < board.numcols(0); col++) {
            cout << '-';
        }
        cout << '-';
        cout << endl;
        for (size_t row = 0; row < board.numrows(); row++) {
            cout << '|';
            for (size_t col = 0; col < board.numcols(row); col++) {
                cout << board(row,col)->display();
            }
            cout << '|';
            cout << endl;
        }
        cout << '-';
        for (size_t col = 0; col < board.numcols(0); col++) {
            cout << '-';
        }
        cout << '-';
        cout << endl;

    }

    bool getWonGame() {
        return wonGame;
    }

    // distributing total number of monsters so that
    //  ~1/3 of num are Super Monsters (M), and
    //  ~2/3 of num are Regular Monsters (m)
    void setNumMonsters(int num) {
        numSuperMonsters = num/3;
        numMonsters = num - numSuperMonsters;
    }

    void setNumAbysses(int num) {
        numAbysses = num;
    }

    void setNumBats(int num) {
        numBats = num;
    }

    size_t getNumRows() {
        return numRows;
    }

    size_t getNumCols() {
        return numCols;
    }


    //---------------------------------------------------------------------------------
    // void getHeroPosition(size_t& row, size_t& col)
    //
    // getter for Hero's position, which are private data members
    //      int HeroRow;
    //      int HeroCol;
    // note: row and col are passed-by-reference
    //---------------------------------------------------------------------------------
    void getHeroPosition(size_t& row, size_t& col) {
        row = HeroRow;
        col = HeroCol;
    }


    //---------------------------------------------------------------------------------
    // void setHeroPosition(size_t row, size_t col)
    //
    // setter for Hero's position, which are private data members
    //      int HeroRow;
    //      int HeroCol;
    //---------------------------------------------------------------------------------
    void setHeroPosition(size_t row, size_t col) {
        HeroRow = row;
        HeroCol = col;
    }


    //---------------------------------------------------------------------------------
    // findHero()
    //
    // updater for Hero's position, which are private data members
    //      int HeroRow;
    //      int HeroCol;
    // this function should find Hero in board and update
    //      HeroRow and HeroCol with the Hero's updated position;
    // if Hero cannot be found in board, then set Hero's position to (-1,-1)
    //---------------------------------------------------------------------------------
    void findHero() {
        for (size_t r = 0; r < numRows; ++r) {
            for (size_t c = 0; c < numCols; ++c) {
                if (board(r, c)->isHero()) {
                    // Hero found, update HeroRow and HeroCol
                    HeroRow = r;
                    HeroCol = c;
                    return;
                }
            }
        }

        // Hero not found, set position to (-1, -1)
        HeroRow = -1;
        HeroCol = -1;
    }


    //---------------------------------------------------------------------------------
    // bool makeMoves(char HeroNextMove)
    //
    // This is the primary gameplay operation for a single round of the game.
    // A LOT happens in this function...
    // General steps:
    //  - Allow user to input their next move
    //  - Get the attempted move position for the Hero
    //  - Move the hero, catching/resolving any potential collision exceptions...
    //      - attempted move out-of-bounds: change row &/or col to stay on board
    //      - attempted move into a barrier: change row &/or col to stay off barrier
    //      - attempted move to the exit: remove hero from board, hero escaped!
    //      - attempted move into a hole: remove hero from board, hero did not escape
    //      - attempted move to a baddie: remove hero from board, hero did not escape
    //      - attempted move to an empty space: actual move is the attempted move
    //  - For each baddie (regular Monster, super Monster, or Bat) on the board...
    //      - check that this baddies hasn't already moved this round
    //      - get its attempted move position
    //      - move the baddie, catching/resolving any potential collision exceptions...
    //          - attempted move out-of-bounds: change row &/or col to stay on board
    //          - attempted move into a barrier: change row &/or col to stay off barrier
    //          - attempted move to the exit: change row &/or col to stay off exit
    //          - attempted move into a hole: remove baddie from board
    //          - attempted move to hero: remove hero from board, hero did not escape
    //          - attempted move to a baddie: ignore attempted move, no position change
    //          - attempted move to an empty space: actual move is the attempted move
    //
    // Note: all baddies (and the hero) fall into holes if their attempted
    //       move ENDs on a hole (i.e. even Bats are drawn into the
    //       Abyss if their attempted move takes them to an Abyss cell);
    //       BUT, baddies can travel over holes, as long as their attempted
    //       move does NOT END on a hole; this only applies, in practice,
    //       to super monsters and bats, since their step sizes can be more
    //       than 1 (monsters and the hero are limited to steps of size 1)
    //
    // Note: also, whereas baddies (and the hero) can never move onto a
    //       barrier (i.e. a wall), they can step over barriers as long
    //       as the attempted move does NOT END on a barrier; this only
    //       applies, in practice, to super monsters and bats, since their
    //       step sizes can be more than 1 (monsters and the hero are limited
    //       to steps of size 1)
    //
    // Note: to prevent a single baddie from making multiple moves
    //       during a single round of the game, whenever a baddie
    //       has moved, it should be marked as "already moved" in
    //       some way; this can be done using the [moved] data member
    //       of the BoardCell base class, which has setter/getter
    //       functions provided. The [moved] data members must be
    //       reset for all BoardCells at the beginning of each round.
    //
    // Note: the [myRow] and [myCol] data members for BoardCell derived
    //       class objects must be updated whenever a move is made;
    //       additionally, [HeroRow] and [HeroCol] data members for the
    //       GameBoard must be updated whenever the Hero has moved,
    //       which can be done easily with a call to findHero()
    //
    // Note: many actual moves made by non-static board cell objects
    //       (i.e. Heros, Monsters, Bats) involve constructing and/or
    //       destructing objects; be careful with memory management;
    //       specifically, make sure to free (delete) the memory for
    //       BoardCell derived class objects when you are done with it
    //
    // return true if Hero is still on board at the end of the round
    // return false if Hero is NOT on board at the end of the round
    //---------------------------------------------------------------------------------
    bool makeMoves(char HeroNextMove) {
        // TODO: Task IV - GameBoard makeMoves() for Hero movement ONLY
        // Get the attempted move position for the Hero
        size_t newR, newC;
        board(HeroRow,HeroCol)->setNextMove(HeroNextMove);
        board(HeroRow,HeroCol)->attemptMoveTo(newR,newC,HeroRow,HeroCol);

        for (int row = 0; row < numRows; row++) {
            for (int col = 0; col < numCols; col++) {
                board(row, col)->setMoved(false);
            }
        }

        try {
            // attempted move out-of-bounds in rows
            if (newR < 0 || newR >= numRows) {
                throw runtime_error("Hero trying to move out-of-bounds with an invalid row");
            }
            else if (newC < 0 || newC >= numCols) {
                throw runtime_error("Hero trying to move out-of-bounds with an invalid column");
            }
        }
        catch (runtime_error& excpt) {
            cout << excpt.what() << endl;
            // if row is the same, column is different, and there is am empty space
            if (newC < numCols && newC >= 0 && board(HeroRow, newC)->isSpace()) {
                newR = HeroRow;
            }
            else if (newR < numRows && newR >= 0 && board(newR, HeroCol)->isSpace()) {    // else if col is same, row is different and there is no barrier...
                newC = HeroCol;
            }
            else {
                newR = HeroRow;
                newC = HeroCol;
            }
            findHero();
            cout << "Changing Hero position to stay in-bounds" << endl;
        }

        try {
            // attempted move into a barrier: change row &/or col to stay off barrier
            if (board(newR, newC)->isBarrier()) {
                if (board(newR, HeroCol)->isBarrier()) {
                    throw runtime_error("Hero trying to move into a barrier");
                }
                newC = HeroCol;
            }
        }
        catch (runtime_error& excpt) {
            cout << excpt.what() << endl;
            newR = HeroRow;
            newC = HeroCol;
            cout << "Changing Hero position to avoid barrier" << endl;
        }

        try {
            // attempted move into a hole or baddie: remove hero from board, hero did not escape
            if (board(newR, newC)->isHole() || board(newR, newC)->isBaddie()) {
                throw runtime_error("Hero trying to move into a hole or baddie");
            }
        }
        catch (runtime_error& excpt) {
            cout << excpt.what() << endl;
            delete board(HeroRow, HeroCol);
            board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
            cout << "Hero did not escape, removed hero from board" << endl;
            return false;
        }

        // attempted move into the exit: remove hero from board, hero escaped!
        if (board(newR, newC)->isExit()) {
            delete board(HeroRow, HeroCol);
            board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
            wonGame = true;
            findHero();
            return false;
        }

        // attempted move to an empty space: actual move is the attempted move
        if (board(newR, newC)->isSpace()) {
            // Free the cell where Hero currently is.
            delete board(HeroRow, HeroCol);

            // Replace the old Hero cell with a new empty space (Nothing)
            board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
            delete board(newR, newC);

            // Update Hero's position
            HeroRow = static_cast<int>(newR);
            HeroCol = static_cast<int>(newC);

            // Put the Hero to the new position
            setCell(new Hero(HeroRow, HeroCol), HeroRow, HeroCol);
        }
        board(HeroRow, HeroCol)->setMoved(true);

        // TODO: Task V - GameBoard makeMoves() for Baddie movement
        // Loop through each cell in the board and reset the moved flag
        // Loop through each cell in the board
        for (int row = 0; row < numRows; row++) {
            for (int col = 0; col < numCols; col++) {
                // Check if the cell is a Baddie and has not moved
                if (board(row, col)->isBaddie() && !board(row, col)->getMoved()) {
                    // Move the Baddie, catching and resolving any potential collision or illegal move exceptions:
                    /*
                     PASTED FROM ABOVE HERO MOVEMENT, WE NEED TO COVER...

                        a.  move puts the Baddie out-of-bounds (move in bounds)
                        b.  move puts the Baddie on a Wall cell or the EscapeLadder (move away)
                        c.  move puts the Baddie on an Abyss cell/hole (remove from board)
                        d.  move puts the Baddie on a cell occupied by a Hero (remove hero, move baddie, hero loses)
                        e.  move puts the Baddies on a cell occupied by another Baddie (have baddie stay where it was)
                                >>> recommended to loop through rows, then loop through columns nested in the row loop
                        f.  move puts the Baddie on an empty cell with Nothing (move baddie normally)

                     MOST ARE COVERED WITH TWEAKING, EDIT b. TO INCLUDE ESCAPE AS WALL, NEED TO WRITE e. ENTIRELY
                     Split "attempted move into a hole or baddie" for c. and d.
                     Combine "attempted move into the exit" with wall check
                    */
                    // Get the attempted move position for the Baddie using the polymorphic BoardCell function attemptMoveTo()
                    size_t newBadR, newBadC;
                    board(row,col)->attemptMoveTo(newBadR,newBadC,HeroRow,HeroCol);

                    try {
                        // attempted move out-of-bounds in rows
                        if (newBadR < 0 || newBadR >= numRows) {
                            throw runtime_error("Baddie trying to move out-of-bounds with an invalid row");
                        }
                    }
                    catch (runtime_error& excpt) {
                        cout << excpt.what() << endl;
                        newBadR = row;
                        cout << "Changing Baddie position to stay in-bounds" << endl;
                    }

                    try {
                        if (newBadC < 0 || newBadC >= numCols) {
                            throw runtime_error("Baddie trying to move out-of-bounds with an invalid column");
                        }
                    }
                    catch (runtime_error& excpt) {
                        cout << excpt.what() << endl;
                        // attempted move out-of-bounds in columns
                        newBadC = col;
                        cout << "Changing Baddie position to stay in-bounds" << endl;
                    }

                    try {
                        // attempted move into a barrier: change row &/or col to stay off barrier
                        if (board(newBadR, newBadC)->isBarrier() || board(newBadR, newBadC)->isExit()) {
                            throw runtime_error("Baddie trying to move into a barrier or EscapeLadder");
                        }
                    }
                    catch (runtime_error& excpt) {
                        cout << excpt.what() << endl;
                        // if row isn't changed, column is valid and changed, and change in column leads to empty space...
                        if ( !(board(row, newBadC)->isBarrier()) && !board(row, newBadC)->isExit() ) {
                            newBadR = row;
                        }
                        // else if col isn't changed, row is valid and changed, and change in row leads to empty space...
                        else if ( !(board(newBadR, col)->isBarrier() && !board(newBadR, col)->isExit() ) ) {
                            newBadC = col;
                        }
                        cout << "Changing Baddie position to avoid barrier or EscapeLadder" << endl;
                    }

                    try {
                        // attempted move into a hole
                        if (board(newBadR, newBadC)->isHole()) {
                            throw runtime_error("Baddie trying to move into a hole");
                        }
                    }
                    catch (runtime_error& excpt) {
                        cout << excpt.what() << endl;
                        delete board(row, col);
                        board(row, col) = new Nothing(row, col);
                        cout << "Baddie removed from board" << endl;
                    }

                    try {
                        // attempted move onto another Baddie
                        if (board(newBadR, newBadC)->isBaddie()) {
                            throw runtime_error("Baddie trying to move into a cell occupied by another Baddie");
                        }
                    }
                    catch (runtime_error& excpt) {
                        cout << excpt.what() << endl;
                        newBadR = row;
                        newBadC = col;
                        cout << "Changing Baddie position to avoid another Baddie" << endl;
                    }

                    // attempted move into Hero
                    if (newBadR == HeroRow && newBadC == HeroCol) {
                        // Remove Hero from the board
                        delete board(HeroRow, HeroCol);
                        //board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);

                        // Put the Baddie to the new position
                        if (getCellDisplay(row,col) == 'M' || getCellDisplay(row,col) == 'm') {
                            setCell(new Monster(newBadR, newBadC), newBadR, newBadC);
                            board(newBadR, newBadC)->setPower(board(row, col)->getPower());
                        }
                        else {
                            setCell(new Bat(newBadR, newBadC), newBadR, newBadC);
                        }

                        // Replace the old Baddie cell with a new empty space (Nothing)
                        delete board(row, col);
                        board(row, col) = new Nothing(row, col);
                        wonGame = false;
                    }

                    // attempted move to an empty space: actual move is the attempted move
                    if (board(newBadR, newBadC)->isSpace()) {
                        delete board(newBadR, newBadC);
                        // Put the Baddie to the new position
                        if (getCellDisplay(row,col) == 'M' || getCellDisplay(row,col) == 'm') {
                            setCell(new Monster(newBadR, newBadC), newBadR, newBadC);
                            board(newBadR, newBadC)->setPower(board(row, col)->getPower());
                        }
                        else {
                            setCell(new Bat(newBadR, newBadC), newBadR, newBadC);
                        }

                        // Replace the old Baddie cell with a new empty space (Nothing)
                        delete board(row, col);
                        board(row, col) = new Nothing(row, col);
                    }

                    board(newBadR, newBadC)->setMoved(true);
                }
            }
        }


        // return false if Hero is NOT on board at the end of the round
        findHero();
        return HeroRow != -1 && HeroCol != -1;
    }


};

#endif //_GAMEBOARD_H