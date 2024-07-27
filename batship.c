#include <stdio.h>
#include <stdbool.h>

#define BOARD_SIZE 10
#define NUM_SHIPS 3

// Function prototypes
void initializeBoard(char board[][BOARD_SIZE]);
void placeShips(char board[][BOARD_SIZE]);
void printBoard(char board[][BOARD_SIZE]);
bool isValidMove(int row, int col);
void fireShot(char board[][BOARD_SIZE], char guessBoard[][BOARD_SIZE], int row, int col);
int calculateScore(char guessBoard[][BOARD_SIZE]);

// A simple linear congruential generator (LCG)
// You can change the values to adjust the randomness
int rand_lcg(int *seed) {
    *seed = (*seed * 1664525) + 1013904223;
    return (*seed % 32768); // Return a value in the range of 0 to 32767
}

// Function to initialize the game board with empty spaces
void initializeBoard(char board[][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = ' ';
        }
    }
}

// Function to place ships randomly on the board
void placeShips(char board[][BOARD_SIZE]) {
    int seed = 1; // Initialize the seed
    int shipCount = 0;
    while (shipCount < NUM_SHIPS) {
        int row = rand_lcg(&seed) % BOARD_SIZE;
        int col = rand_lcg(&seed) % BOARD_SIZE;

        if (board[row][col] == ' ') {
            board[row][col] = 'S';
            shipCount++;
        }
    }
}

// Function to print the game board
void printBoard(char board[][BOARD_SIZE]) {
    printf("   ");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%c ", 'A' + i);
    }
    printf("\n");

    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%2d ", i + 1);
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

// Function to check if a move is valid
bool isValidMove(int row, int col) {
    return (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE);
}

// Function to process a shot (fire at a location)
void fireShot(char board[][BOARD_SIZE], char guessBoard[][BOARD_SIZE], int row, int col) {
    if (board[row][col] == 'S') {
        guessBoard[row][col] = 'X';
    } else {
        guessBoard[row][col] = 'O';
    }
}

// Function to calculate the score based on the number of hits
int calculateScore(char guessBoard[][BOARD_SIZE]) {
    int score = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (guessBoard[i][j] == 'X') {
                score++;
            }
        }
    }
    return score;
}

int main() {
    char board[BOARD_SIZE][BOARD_SIZE];
    char guessBoard[BOARD_SIZE][BOARD_SIZE];
    int row, col;
    char input[3];
    bool gameOver = false;

    while (1) {
        initializeBoard(board);
        initializeBoard(guessBoard);
        placeShips(board);

        printf("Welcome to Batship!\n");
        printf("Fire away! Enter your move in the format 'A1', 'B2', etc.\n");

        while (!gameOver) {
            printBoard(guessBoard);

            // Get user input and convert to row and column
            printf("Enter your move: ");
            scanf("%s", input);
            row = input[1] - '1';
            col = input[0] - 'A';

            // Validate move
            while (!isValidMove(row, col)) {
                printf("Invalid move. Enter your move again: ");
                scanf("%s", input);
                row = input[1] - '1';
                col = input[0] - 'A';
            }

            // Fire shot and update guessBoard
            fireShot(board, guessBoard, row, col);

            // Check if the game is over
            gameOver = true;
            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    if (board[i][j] == 'S' && guessBoard[i][j] != 'X') {
                        gameOver = false;
                        break;
                    }
                }
                if (!gameOver) {
                    break; // Exit the outer loop if gameOver is false
                }
            }
        }

        // Game over - display score and prompt to play again
        printBoard(guessBoard);
        printf("You sunk all the ships!\n");
        printf("Your score: %d\n", calculateScore(guessBoard));

        printf("Play again? (Y/N): ");
        scanf(" %c", input);
        if (input[0] != 'Y' && input[0] != 'y') {
            break;
        }
        gameOver = false;
    }

    return 0;
}
