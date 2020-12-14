// CS300 HW2 Conway's Game of Life
// Adam Shih 2020
#include <ncurses.h>
#include <unistd.h>

void step(int** board);
int count_neighbors(int** board, int row, int col);
void display(int** board);

int main(void)
{
    int** board;
    bool paused = true;
    bool quit = false;
    int ch, y, x;

    initscr();              //start curses mode
    keypad(stdscr, true);   //detect arrow key presses
    noecho();               //don't echo key presses
    nodelay(stdscr, true);  // don't wait for input to continue

    //initialize board
    board = new int*[COLS];
    for(int i = 0; i < COLS; ++i)
    {
        board[i] = new int[LINES];
    }

    while(!quit)  // wait for 'q' input
    {
        while(paused && !quit)
        {
            getyx(stdscr, y, x);
            ch = getch();

            if(ch == KEY_UP)
                move(y - 1, x);
            else if(ch == KEY_DOWN)
                move(y + 1, x);
            else if(ch == KEY_LEFT)
                move(y, x - 1);
            else if(ch == KEY_RIGHT)
                move(y, x + 1);
            else if(ch == 'x')
                if(board[x][y])
                    board[x][y] = 0;
                else
                    board[x][y] = 1;
            else if(ch == 'n')
                step(board);
            else if(ch == 'p')
                paused = !paused;
            else if(ch == 'q')
                quit = true;

            display(board);
        }

        while(!paused && !quit)
        {
            getyx(stdscr, y, x);
            ch = getch();

            if(ch == 'p')
                paused = !paused;
            else if(ch == 'q')
                quit = true;

            step(board);
            display(board);
            usleep(500000); //wait 0.5s in between steps
        }
    }

    // deallocate memory used for the board
    for(int i = 0; i < COLS; ++i)
    {
        delete[] board[i];
    }
    delete[] board;

    endwin();
    return 0;
}

//move the board one step forward in time
void step(int** board)
{
    int neighbors;
    int** copy;
    
    copy = new int*[COLS];
    for(int i = 0; i < COLS; ++i)
    {
        copy[i] = new int[LINES];
    }

    for(int i = 0; i < COLS; ++i)
    {
        for(int j = 0; j < LINES; ++j)
        {
            copy[i][j] = board[i][j];
        }
    }

    for(int i = 0; i < COLS; ++i)
    {
        for(int j = 0; j < LINES; ++j)
        {
            neighbors = count_neighbors(copy, i, j);           

            if(copy[i][j] && (neighbors < 2 || neighbors > 3))
                board[i][j] = 0;
            
            if(!copy[i][j] && neighbors == 3)
                board[i][j] = 1;
        }   
    }

    for(int i = 0; i < COLS; ++i)
    {
        delete[] copy[i];
    }
    delete[] copy;
}

//return the number of neighbors an array index has as an int
int count_neighbors(int** board, int row, int col) 
{
    int count = 0;
    int operands[] = { 0, 1, -1 }; //cases for neighbors
    int x, y;

    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            // if the cell is not the current cell
            if(!(operands[i] == 0 && operands[j] == 0))
            {
                x = row + operands[i];
                y = col + operands[j];

                //check to see if it is in bounds
                if((x < COLS && x >= 0) && (y < LINES && y >= 0) && board[x][y])
                    ++count;
            }
        }
    }

    return count;
}

//display the board to the screen
void display(int** board)
{
    int x, y;
    getyx(stdscr, y, x);

    erase();
    
    for(int i = 0; i < COLS; ++i)
    {
        for(int j = 0; j < LINES; ++j)
        {
            if(board[i][j] == 1)
            {
                mvaddch(j, i, 'X');
                //move(j, i);
                //printw("X");
            }
        }
    }

    move(y, x);
    refresh();
}
