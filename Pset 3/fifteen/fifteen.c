/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state''' of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    // get the number for the largest tile on the board (total number of tiles minus one)
    int largest_num = (d * d) - 1;
    
    // use two for loops to iterate through the 2D array - rows and columns
    // first loop iterates through the rows one at a time
    for (int row = 0; row < d; row++)
    {
        // second loop iterates through all the columns before preceding to the next row
        for (int column = 0; column < d; column++)
        {
            // insert into the board the value of the tile, then decrement the counter
            // (if board is 4 x 4 = 16 tiles, the number in the top left corner will be 15)
            board[row][column] = largest_num;
            largest_num--;
        }
    }
    
    // if the given dimension for the board is even, swap the position of the 1 and 2 tiles so the game is solveable
    // (swap two tiles in the last row, the second and third from the right)
    if (d % 2 == 0)
    {
        int temp = board[d - 1][d - 2];
        board[d - 1][d - 2] = board[d - 1][d - 3];
        board[d - 1][d - 3] = temp;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    printf("\n");
    
    // use two for loops to iterate through the 2D array - rows and columns
    // first loop iterates through the rows one at a time
    for (int row = 0; row < d; row++)
    {
        // second loop iterates through all the columns before preceding to the next row
        for (int column = 0; column < d; column++)
        {
            // change the 0 tile to blank
            if (board[row][column] == 0)
            {
                printf("   _");
            }
            // print each tile number
            else
            {
                printf("%4i", board[row][column]);
            }
        }
        // print two blank lines after each row to make the grid
        printf("\n\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    int row = 0, column = 0;
    
    // find the tile with the integer the user typed
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == tile)
            {
                row = i;
                column = j;
            }
        }
    }
    
    // check to make sure we are not at the bottom of the board already
    // and if there is an adjacent blank space below
    if (row + 1 < d && board[row + 1][column] == 0)
    {
        // switch tile and blank space
        board[row + 1][column] = board[row][column];
        board[row][column] = 0;
        return true;
    }
    
    // check to make sure we are not at the top of the board already
    // and if there is an adjacent blank space above
    else if (row - 1 >= 0 && board[row - 1][column] == 0)
    {
        // switch tile and blank space
        board[row - 1][column] = board[row][column];
        board[row][column] = 0;
        return true;
    }
    
    // check to make sure we are not at the right of the board already
    // and if there is an adjacent blank space to the right
    else if (column + 1 < d && board[row][column + 1] == 0)
    {
        // switch tile and blank space
        board[row][column + 1] = board[row][column];
        board[row][column] = 0;
        return true;
    }
    
    // check to make sure we are not at the left of the board already
    // and if there is an adjacent blank space to the left
    else if (column - 1 >= 0 && board[row][column - 1] == 0)
    {
        // switch tile and blank space
        board[row][column - 1] = board[row][column];
        board[row][column] = 0;
        return true;
    }
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // set counter
    int n = 1;
    
    // check to see whether the last tile is blank
    // if it is not blank, return false
    if (board[d - 1][d - 1] != 0)
    {
        return false;
    }
    
    // use two for loops to iterate through the 2D array - rows and columns
    // first loop iterates through the rows one at a time
    for (int row = 0; row < d; row++)
    {
        // second loop iterates through all the columns before preceding to the next row
        for (int column = 0; column < d; column++)
        {
            // check the bottom right grid position first, if blank, return true
            if (row == d - 1 && column == d - 1)
            {
                return true;
            }
                
            // check the number on corresponding tile
            // if it doesn't match the counter, return false
            // (ex. the tile at board[0][0] should have a value of 1)
            if (n != board[row][column])
            {
                return false;
            }
            // increment the counter
            n++;                        
        }
    }
    return false;
}
