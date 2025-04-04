// THIS IS THE HEADER INCLUDES SECTION
#include "SlidingPuzzle.h"  // MAIN GAME HEADER
#include <iostream>         // INPUT/OUTPUT STREAM HEADER
#include <iomanip>          // INPUT/OUTPUT MANIPULATION HEADER
#include <conio.h>          // CONSOLE INPUT/OUTPUT HEADER
#include <windows.h>        // WINDOWS API HEADER
#include <stdlib.h>         // STANDARD LIBRARY HEADER
#include <ctime>            // TIME FUNCTIONS HEADER

// DISABLE WARNING FOR _GETCH() USAGE
#pragma warning(disable:6031)

// USE STANDARD NAMESPACE 
using namespace std;

// UTILITY FUNCTION TO GET AND VALIDATE USER INPUT WITHIN A SPECIFIED RANGE
int SlidingPuzzle::getValidInput(int min, int max) {
    int input = 0; // INITIALIZE FOR SAFETY (?)
    while (INPUT_VALIDATE_LOOP) {
        cout << "(" << min << "-" << max << "): ";
        cin >> input;

        // CHECK IF INPUT IS WITHIN VALID RANGE
        while (input >= min && input <= max) {
            return input;
        }

        // CLEAR INPUT BUFFER AND SHOW ERROR MESSAGE IF INPUT IS INVALID
        cout << "Invalid input. Try again." << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

// CONSTRUCTOR: INITIALIZES THE SLIDING PUZZLE GAME
SlidingPuzzle::SlidingPuzzle() {

    // GET BOARD DIMENSIONS FROM USER
    cout << "Enter the number of rows for the puzzle: ";
    rows = getValidInput(3, 10);

    cout << "Enter the number of columns for the puzzle: ";
    cols = getValidInput(3, 10);

    // DYNAMICALLY ALLOCATE 2D ARRAY FOR THE GAME BOARD
    board = new int* [rows];
    for (int i = 0; i < rows; i++) {
        board[i] = new int[cols];
    }

    // INITIALIZE RANDOM NUMBER GENERATOR
    srand(time(0));

    // SET UP INITIAL SOLVED BOARD STATE
    initializeBoard();
    printBoard();

    // WAIT FOR USER INPUT BEFORE SHUFFLING
    cout << "Press any key to shuffle the board." << endl;
    _getch();

    // SHUFFLE THE BOARD AND WAIT FOR USER TO START
    cout << "Shuffling board..." << endl;
    shuffleBoard();

    cout << "Shuffling complete. Press any key to begin the game." << endl;
    _getch();
}

// DESTRUCTOR: CLEAN UP DYNAMICALLY ALLOCATED MEMORY
SlidingPuzzle::~SlidingPuzzle() {
    // DELETE EACH ROW OF THE BOARD
    for (int i = 0; i < rows; i++) {
        delete[] board[i];
    }
    // DELETE THE ARRAY OF POINTERS
    delete[] board;
}

// INITIALIZE THE BOARD IN SOLVED STATE
void SlidingPuzzle::initializeBoard() {
    int currentTile = 1;
    // FILL BOARD WITH SEQUENTIAL NUMBERS, LEAVING BOTTOM-RIGHT EMPTY
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i == rows - 1 && j == cols - 1) {
                board[i][j] = SPACE_EMPTY;
            }
            else {
                board[i][j] = currentTile++;
            }
        }
    }
}

// PRINT THE CURRENT STATE OF THE BOARD WITH COLORED TILES
void SlidingPuzzle::printBoard() const {
    // GET HANDLE FOR CONSOLE COLOR MANIPULATION
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // PRINT TOP BORDER
    cout << "+";
    for (int j = 0; j < cols; j++) {
        cout << "---+";
    }
    cout << endl;

    // PRINT BOARD CONTENTS WITH COLOR-CODING
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << "|";

            // PRINT EMPTY SPACE IN BLUE
            if (board[i][j] == SPACE_EMPTY) {
                SetConsoleTextAttribute(hConsole, COLOR_BLUE);
                cout << " " << EMPTY_TILE_SYMBOL << " ";
            }
            else {
                // COLOR TILES GREEN IF IN CORRECT POSITION, RED IF NOT
                int expectedValue = i * cols + j + 1;
                if (board[i][j] == expectedValue) {
                    SetConsoleTextAttribute(hConsole, COLOR_GREEN);
                }
                else {
                    SetConsoleTextAttribute(hConsole, COLOR_RED);
                }
                cout << setw(3) << board[i][j];
            }

            // RESET COLOR TO DEFAULT
            SetConsoleTextAttribute(hConsole, COLOR_DEFAULT);
        }

        // PRINT RIGHT BORDER AND ROW SEPARATOR
        cout << "|" << endl;
        cout << "+";
        for (int j = 0; j < cols; j++) {
            cout << "---+";
        }
        cout << endl;
    }
}

// MOVE A TILE IN THE SPECIFIED DIRECTION
bool SlidingPuzzle::moveTile(int direction) {
    int emptyRow = -1, emptyCol = -1;

    // FIND THE EMPTY SPACE ON THE BOARD
    for (int i = 0; i < rows && emptyRow == -1; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j] == SPACE_EMPTY) {
                emptyRow = i;
                emptyCol = j;
                break;
            }
        }
    }

    // RETURN FALSE IF EMPTY SPACE NOT FOUND (SHOULD NEVER HAPPEN)
    if (emptyRow == -1 || emptyCol == -1) {
        return INVALID_MOVE;
    }

    // HANDLE MOVES IN EACH DIRECTION IF VALID
    if (direction == MOVE_TILE_UP && emptyRow > 0) {
        board[emptyRow][emptyCol] = board[emptyRow - 1][emptyCol];
        board[emptyRow - 1][emptyCol] = SPACE_EMPTY;
        return VALID_MOVE;
    }
    else if (direction == MOVE_TILE_DOWN && emptyRow < rows - 1) {
        board[emptyRow][emptyCol] = board[emptyRow + 1][emptyCol];
        board[emptyRow + 1][emptyCol] = SPACE_EMPTY;
        return VALID_MOVE;
    }
    else if (direction == MOVE_TILE_LEFT && emptyCol > 0) {
        board[emptyRow][emptyCol] = board[emptyRow][emptyCol - 1];
        board[emptyRow][emptyCol - 1] = SPACE_EMPTY;
        return VALID_MOVE;
    }
    else if (direction == MOVE_TILE_RIGHT && emptyCol < cols - 1) {
        board[emptyRow][emptyCol] = board[emptyRow][emptyCol + 1];
        board[emptyRow][emptyCol + 1] = SPACE_EMPTY;
        return VALID_MOVE;
    }

    // RETURN FALSE IF MOVE IS INVALID
    return INVALID_MOVE;
}

// RANDOMLY SHUFFLE THE BOARD BY MAKING MANY RANDOM VALID MOVES
void SlidingPuzzle::shuffleBoard() {
    // NUMBER OF RANDOM MOVES = BOARD SIZE * 100
    int moves = rows * cols * 100;
    for (int i = 0; i < moves; i++) {
        int randomDirection = rand() % 4 + 1;
        moveTile(randomDirection);
    }
}

// CONVERT KEYBOARD INPUT TO MOVE DIRECTION AND EXECUTE MOVE
bool SlidingPuzzle::makeMove(char keyStroke) {
    int direction;
    // CONVERT WASD KEYS TO MOVE DIRECTIONS 
    // (ADDED SUPPORT FOR CAPS LOCK)
    switch (keyStroke) {
    case 'w':
    case 'W':
        direction = MOVE_TILE_UP;
        break;
    case 's':
    case 'S':
        direction = MOVE_TILE_DOWN;
        break;
    case 'a':
    case 'A':
        direction = MOVE_TILE_LEFT;
        break;
    case 'd':
    case 'D':
        direction = MOVE_TILE_RIGHT;
        break;
    default:
        return INVALID_MOVE;
    }
    return moveTile(direction);
}

// CHECK IF THE PUZZLE IS SOLVED (ALL TILES IN CORRECT POSITION)
bool SlidingPuzzle::isSolved() const {
    int expectedValue = 1;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // CHECK IF EMPTY SPACE IS IN BOTTOM-RIGHT CORNER
            if (i == rows - 1 && j == cols - 1) {
                return board[i][j] == SPACE_EMPTY;
            }
            // CHECK IF CURRENT TILE MATCHES EXPECTED VALUE
            else if (board[i][j] != expectedValue) {
                return PUZZLE_UNSOLVED;
            }
            expectedValue++;
        }
    }
    return PUZZLE_SOLVED;
}

// MAIN GAME LOOP
void SlidingPuzzle::play() {
    char keyStroke;
    while (GAME_LOOP) {
        // CLEAR SCREEN AND DISPLAY CURRENT BOARD STATE
        system("cls");
        printBoard();
        cout << "Move with WASD" << endl;
        cout << "Press 'Q' to quit the game" << endl;
        cout << "Enter your move: ";
        keyStroke = _getche();

        // CHECK FOR QUIT COMMAND
        if (keyStroke == 'q' || keyStroke == 'Q') {
            cout << endl << "Quitting game!" << endl;
            return;
        }

        // HANDLE INVALID MOVES
        if (!makeMove(keyStroke)) {
            cout << endl << "Invalid move! Try again." << endl;
            _getch();
        }

        // CHECK FOR WIN CONDITION
        if (isSolved()) {
            system("cls");
            printBoard();
            cout << "Congratulations! You solved the puzzle!" << endl;
            _getch();
            return;
        }
    }
}