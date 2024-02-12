# Outlast-the-Baddies
1. Setting up the board
   - Defines a template class called Grid. The purpose of this class is to represent a 2D grid or matrix with elements of type T. The class provides functionality for creating, manipulating, and managing a grid of arbitrary size.
   - Implements smaller helper functions to return the number of columns, total elements, etc.

2. Handling movement and object places
   Defines a set of classes representing different types of cells on a game board for a maze-like game, each handling its specific behavior, such as movement, display, and categorization on the game board
   
   BoardCell (Abstract Base Class):
      - Represents a generic cell on the game board.
      - Provides virtual functions for displaying the cell, attempting to move to a new position, and checking the type of the cell.
      - Contains functions for setting and getting the position of the cell.
    
    Hero (Derived from BoardCell):
      - Represents the hero character on the game board.
      - Overrides the display function to return 'H'.
      - Implements attemptMoveTo to handle hero movement based on the next move input.

    Monster (Derived from BoardCell):
      - Represents a monster character on the game board.
      - Overrides the display function to return 'M' or 'm' based on power.
      - Implements attemptMoveTo to make monsters move closer to the hero.
    
    Bat (Derived from BoardCell):
      - Represents a bat character on the game board.
      - Overrides the display function to return '~'.
      - Implements attemptMoveTo to navigate towards the hero's column without changing rows.
        
     Abyss (Derived from BoardCell):
      - Represents an abyss cell on the game board.
      - Overrides the display function to return '#'.
  
    Wall (Derived from BoardCell):
      - Represents a wall cell on the game board.
      - Overrides the display function to return '+'.
        
    EscapeLadder (Derived from BoardCell):
      - Represents an escape ladder cell on the game board
