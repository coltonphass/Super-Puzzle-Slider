#pragma once
// PRAGMA ONCE ENSURES HEADER IS ONLY INCLUDED ONCE IN COMPILATION

// MAIN SLIDING PUZZLE GAME CLASS DEFINITION
class SlidingPuzzle {
private:
    // BOARD DIMENSIONS
    int rows;                          // NUMBER OF ROWS IN PUZZLE
    int cols;                          // NUMBER OF COLUMNS IN PUZZLE

    // DYNAMIC 2D ARRAY FOR GAME BOARD
    int** board;                       // POINTER TO 2D ARRAY OF INTEGERS

    // CONSTANT DEFINITIONS FOR SPECIAL TILE
    const int SPACE_EMPTY = -1;
    const char EMPTY_TILE_SYMBOL = '*';

    // DIRECTION CONSTANTS FOR TILE MOVEMENT
    const int MOVE_TILE_UP = 1;
    const int MOVE_TILE_DOWN = 2;
    const int MOVE_TILE_LEFT = 3;
    const int MOVE_TILE_RIGHT = 4;

    // BOOLEAN CONSTANTS FOR IMPROVED CODE READABILITY
    const bool VALID_MOVE = true;       // INDICATES SUCCESSFUL MOVE
    const bool INVALID_MOVE = false;      // INDICATES FAILED MOVE
    const bool PUZZLE_SOLVED = true;       // INDICATES WINNING STATE
    const bool PUZZLE_UNSOLVED = false;      // INDICATES ONGOING GAME
    const bool GAME_LOOP = true;       // USED FOR CONTINUOUS GAME LOOP
    const bool INPUT_VALIDATE_LOOP = true;       // USED FOR VALIDATION LOOP

    // CONSOLE COLOR CODES FOR VISUAL FEEDBACK
    const int COLOR_DEFAULT = 7;       // DEFAULT TEXT COLOR
    const int COLOR_RED = 12;      // FOR INCORRECT POSITIONS
    const int COLOR_GREEN = 10;      // FOR CORRECT POSITIONS
    const int COLOR_BLUE = 9;       // FOR EMPTY SPACE

    // PRIVATE MEMBER FUNCTIONS FOR INTERNAL GAME LOGIC
    void initializeBoard();              // SETS UP INITIAL SOLVED BOARD STATE
    bool moveTile(int direction);        // HANDLES INDIVIDUAL TILE MOVEMENTS
    void shuffleBoard();                 // RANDOMIZES BOARD FROM SOLVED STATE
    int getValidInput(int min, int max); // VALIDATES USER INPUT WITHIN RANGE

public:
    // PUBLIC INTERFACE FOR GAME INTERACTION
    SlidingPuzzle();                  // CONSTRUCTOR: CREATES AND INITIALIZES GAME
    ~SlidingPuzzle();                 // DESTRUCTOR: CLEANS UP DYNAMIC MEMORY
    void printBoard() const;          // DISPLAYS CURRENT BOARD STATE
    bool makeMove(char keyStroke);    // PROCESSES USER INPUT FOR MOVES
    bool isSolved() const;            // CHECKS IF PUZZLE IS SOLVED
    void play();                      // MAIN GAME LOOP
};