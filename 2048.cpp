#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <curses.h>
#include <fstream>
 
using namespace std;
 
// grid number
#define N 4
 // Character length of each grid
#define WIDTH 5
 // Victory conditions
#define TARGET 2048
 
 // game state
#define S_FAIL 0
#define S_WIN 1
#define S_NORMAL 2
#define S_QUIT 3
 
 
class Game2048
{
public:
	Game2048():status(S_NORMAL)
	{
		myScore = 0;
		setTestData();
	}
	int getStatus()
	{
		return status;
	}
 
	 //Handle keys
	void processInput()
	{
		char ch = getch();
		bool updated = false;
		if( 'a' <= ch <= 'z' )
		{
			ch = ch -32;
		}
		if(ch == 'Q')
		{
			status = S_QUIT;
		}
		else if (ch == 'R')
		{
			restart();
		}
		else if (ch == 'A')
		 {//Left
			updated = moveLeft();
		}
		else if (ch == 'W')
		 {//Up
			rotate();
			updated = moveLeft();
			rotate();
			rotate();
			rotate();
		}
		else if (ch == 'D')
		 		{//To the right 
			rotate();
			rotate();
			updated = moveLeft();
			rotate();
			rotate();
		}
		else if (ch == 'S')
		 {//Down
			rotate();
			rotate();
			rotate();
			updated = moveLeft();
			rotate();
		}
 
		if (updated)
		{
			 randNew(); // randomly generate a new element
			if (isOver())
			{
				status = S_FAIL;
			}
		}
 
	}
 
         // Draw the game interface
    void draw() 
    {
                 // clean the screen
        clear();
                 // Center offset
        const int offset = 12;
        for (int i = 0; i <= N; ++i) 
        {
            for (int j = 0; j <= N; ++j) 
            {
                                 // intersection
                drawItem(i * 2, 1 + j * WIDTH + offset, '+');
 
                                 // vertical bar
                if (i != N) 
                {
                    drawItem(i * 2 + 1, 1 + j * WIDTH + offset, '|');
                }
 
                                 // horizontal line
                for (int k = 1; j != N && k < WIDTH; ++k) 
                {
                    drawItem(i * 2, 1 + j * WIDTH + k + offset, '-');
                }
 
                                 // the number in each grid
                if (i != N && j != N) 
                {
                    drawNum(i * 2 + 1, (j + 1) * WIDTH + offset, data[i][j]);
                }
            }
        }
        drawString(1,offset-WIDTH,"highest:");
        drawNum(1,offset,readDateToFile("score.txt"));
 
        drawString(2,offset-WIDTH,"score:");
        drawNum(2,offset,myScore);
        
 
                 // prompt text
        mvprintw(2 * N + 2, (5 * (N - 4) - 1) / 2, "W(UP),S(DOWN),A(LEFT),D(RIGHT),R(RESTART),Q(QUIT)");
        mvprintw(2 * N + 3, 5 + 5 * (N - 4) / 2, "My Blog : https://blog.csdn.net/feit2417");
 
        if (status == S_WIN) 
        {
        	
            mvprintw( N, 5 * N / 2 - 1, " YOU WIN,PRESS R TO CONTINUE ");
        } else if (status == S_FAIL) 
        {
            mvprintw( N, 5 * N / 2 - 1, " YOU LOSE,PRESS R TO CONTINUE ");
        }
 
    	if (myScore > readDateToFile("score.txt"))
    	{
    		     writeDateToFile("score.txt",myScore); //Record score to file
    	}
    }
 
         // Easy to debug, set data at any time
    void setTestData() 
    {
        for (int i = 0; i < N; ++i) 
        {
            for (int j = 0; j < N; ++j) 
            {
                data[i][j] = 16 << (i + j);
                // data[i][j] = 2 << (1 + rand() % 7);
                /*
                data[i][0] = 2;
                data[i][1] = 4;
                data[i][2] = 8;
                data[i][3] = 16;*/
            }
        }
    }
 
private:
         // judge the game is over
    bool isOver() 
    {
        for (int i = 0; i < N; ++i) 
        {
            for (int j = 0; j < N; ++j) 
            {
                                 // If there are vacancies or the neighbors have the same, you can continue
                if ((j + 1 < N) && (data[i][j] * data[i][j+1] == 0 || data[i][j] == data[i][j+1])) return false;
                if ((i + 1 < N) && (data[i][j] * data[i+1][j] == 0 || data[i][j] == data[i+1][j])) return false;
            }
        }
        return true;
    }
 
	 	//move to the left 
	bool moveLeft()
	{
		int tmp[N][N];	
		for (int i = 0; i < N; ++i)
		 {//Traverse line by line
			 int compareValue = 0; //Compare value
			 int currentWritePos = 0; //Currently written position
			for (int j = 0; j < N; ++j)
			{
				tmp[i][j] = data[i][j];
				if (0 == data[i][j])
				 {//Element is empty, end this cycle
					continue;
				}
				if (0 == compareValue)
				{
					 compareValue = data[i][j]; //Set comparison value
				}
				else
				{
					if (compareValue == data[i][j])
					 {//Adjacent elements are equal
						 data[i][currentWritePos] = compareValue*2; //The result is written to the current position
						 compareValue = 0; //Prepare the next round of comparison
						++currentWritePos;
						
						 myScore += 10; //follow the new score
					}
					else 
					{
						 data[i][currentWritePos] = compareValue; //Write the comparison value back to the current writing position
						 compareValue = data[i][j]; //Compare with new value
						++currentWritePos;
 
					}
				}
				 data[i][j] = 0; //Clear the record at this position
 
			}
			if (0 != compareValue)
			 {//Considering the rightmost element to ensure that it will not be "cleared"
				data[i][currentWritePos] = compareValue;
			}
		}
 
		for (int i = 0; i <= N; ++i)
		 {//See if the array has changed before and after left shift
			for (int j = 0; j <= N; ++j)
			{
				if (tmp[i][j] != data[i][j]) 
					return true;
			}
		}
		return false;
 
	}
 
         // The matrix rotates 90 degrees counterclockwise
    void rotate() {
        int tmp[N][N] = {0};
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                tmp[i][j] = data[j][N - 1 - i];
            }
        }
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                data[i][j] = tmp[i][j];
            }
        }
    }
 
         // Restart 
    void restart() 
    {
        for (int i = 0; i < N; ++i) 
        {
            for (int j = 0; j < N; ++j) 
            {
                data[i][j] = 0;
            }
        }
        randNew();
        randNew();
        status = S_NORMAL;
    }
 
 
         // Randomly generate a new number
    bool randNew() 
    {
        vector<int> emptyPos;
                 // Store the empty position first
        for (int i = 0; i < N; ++i) 
        {
            for (int j = 0; j < N; ++j) 
            {
                if (data[i][j] == 0) 
                {
                    emptyPos.push_back(i * N + j);
                }
            }
        }
 
        if (emptyPos.size() == 0) 
        {
            return false;
        }
 
                 // Randomly find an empty position
        int value = emptyPos[rand() % emptyPos.size()];
                 // 10% probability produces 4
        data[value / N][value % N] = rand() % 10 == 1 ? 4 : 2;
        return true;
    }
 
 
	 // The upper left corner is (0, 0), draw a character at the specified position
    void drawItem(int row, int col, char c) 
    {
        move(row, col);
        addch(c);
    }
 
         //Write a string at the specified position
    void drawString(int row, int col, string str)
    {
 
		int num = str.size();
		while (num--) 
		{
			drawItem(row, col, str[num]);
			--col;
		}
    }
 
         // The numbers in the game are right aligned, row, col are the position of the last digit
    void drawNum(int row, int col, int num) 
    {
        while (num > 0) {
            drawItem(row, col, num % 10 + '0');
            num /= 10;
            --col;
        }
    }
 
         //Write value to file
    void writeDateToFile(const char *path,int value)
    {
		ofstream of(path);
		of << value;
		of.close();
    }
         //Read the value in the file
    int readDateToFile(const char *path)
    {
		ifstream inf(path);
		int sb;
		inf>>sb;
		inf.close();
 
		return sb;
    }
 
private:
	 int myScore; // player score
	 int status; //Game status
	 int data[N][N]; //Array
 
};
 
void initialize() 
{
         // ncurses initialization
    initscr();
         // Keys do not need to enter Enter to interact directly
    cbreak();
         // Buttons are not displayed
    noecho();
         // hide the cursor
    curs_set(0);
         // random number 
    srand(time(NULL));
}
 
void shutdown() {
         // ncurses cleanup
    endwin();
}
 
int main(int argc, char const *argv[])
{
	initialize();
 
	Game2048 game;
	do 
	{
		game.draw();
		game.processInput();
	}while(S_QUIT != game.getStatus());
 
	shutdown();
	return 0;
}