//C++ Progrm to implement Snake Game

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <direct.h>
#include <ctime>
#include <vector>

#define MAX_SNAKE_SIZE 100
#define HEIGHT 29
#define WIDTH 119

using namespace std;


class Console
{
protected:
    HANDLE console;
    COORD cursor_position;

    Console()
    {
        console = GetStdHandle(STD_OUTPUT_HANDLE);
    }

    void setCursorPosition(int x, int y){
        cursor_position.X = x;
        cursor_position.Y = y;
        SetConsoleCursorPosition(console, cursor_position);
    }

    void setCursorInfo(bool visible, DWORD size)
    {
        if(size == 0)   size = 20;
        CONSOLE_CURSOR_INFO lpCursor;	
        lpCursor.bVisible = visible;
        lpCursor.dwSize = size;
        SetConsoleCursorInfo(console, &lpCursor);
    }
};


class Snake : public Console
{
    class Point : public Console
    {
        int x, y;

    public:
        Point(int x = 10, int y = 10)
        {
            this->x = x;
            this->y = y;
        }

        void setPoint(int x, int y)
        {
            this -> x = x;
            this -> y = y;
        }

        void moveUp()
        {
            y--;
            if (y < 0)  y = HEIGHT;
        }
        
        void moveDown()
        {
            y++;
            if (y > HEIGHT)  y = 0;
        }
        
        void moveLeft()
        {
            x--;
            if (x < 0)  x = WIDTH;
        }
        
        void moveRight()
        {
            x++;
            if (x > WIDTH)  x = 0;
        }
        
        void draw(char ch)
        {
            setCursorPosition(x, y);
            cout << ch;
        }
        
        bool operator ==(Point p)       //Collision
        {
            return x == p.x && y == p.y;
        }
    };

protected:
    vector<Point> snake;
    char direction;
    Point fruit;
    bool is_alive, started;

    Snake()
    {
        setCursorInfo(false, 0);
        snake.push_back(Point(20, 20));
        fruit.setPoint(rand() % WIDTH, rand() % HEIGHT);
        is_alive = started = false;
    }

    void grow(int x, int y)
    {
        snake.push_back(Point(x, y));
    }

    void turnUp()
    {
        if (direction != 's')   direction = 'w';
    }

    void turnDown()
    {
        if (direction != 'w')   direction = 's';
    }

    void turnLeft()
    {
        if (direction != 'd')   direction = 'a';
    }

    void turnRight()
    {
        if (direction != 'a')   direction = 'd';
    }    
    
    bool selfCollision()
    {
        for(int i = 1; i < snake.size() - 1; i++)
            if (snake[0] == snake[i])   return true;
        return false;
    }
};

class PlayGame : public Snake
{
    void welcomeScreen()
    {
        SetConsoleTextAttribute(console, 15);
        cout << "\n|         /\\/\\/\\                         |";
        cout << "\n|        | ___ |                         |";
        cout << "\n|        \\/   \\/                         |";
        cout << "\n|        /     \\                         |";
        cout << "\n|       / /\\ /\\ \\                        |";
        cout << "\n|       | \\/ \\/ |                        |";
        cout << "\n|       \\\\  _   /                        |";
        cout << "\n|        \\\\    /                         |";
        cout << "\n|         |    |       _...._            |";
        cout << "\n|         |    \\\\    /       \\           |";
        cout << "\n|         \\\\    \\\\__/    __   |          |";
        cout << "\n|          \\\\          /  \\\\  |          |";
        cout << "\n|           \\\\________/    \\\\/           |";
        cout << "\n|                                        |";
        cout << "\n|    WELCOME TO SAMUEL's SNAKE GAME!!    |";
    }
    
    void runGame()
    {
        system("cls");
        
        if(!is_alive)
            if(!started)
            {
                welcomeScreen();
                cout << "\n\nPress any key to start\t";
                getch();
                is_alive = started = true; 
            }
            else
            {
                cout << "\nGame Over :-(";
                cout << "\nPress any key to start\t";
                getch();
                is_alive = true;
                snake.erase(snake.begin() + 1, snake.end());
            }
        
        for(int i = snake.size() - 1; i > 0; i--)   //Snake moves
            snake[i] = snake[i-1];
        
        switch(direction)   //Change direction of head
        {
            case 'w':   snake[0].moveUp();      break;
            case 's':   snake[0].moveDown();    break;
            case 'a':   snake[0].moveLeft();    break;
            case 'd':   snake[0].moveRight();   break;
        }

        if (selfCollision()) is_alive = false;

        if (fruit == snake[0])
        {  
            grow(0, 0);
            fruit.setPoint(rand() % WIDTH, rand() % HEIGHT);
        }

        SetConsoleTextAttribute(console, 242);  //green snake
        for(int i = 0; i < snake.size(); i++)
			snake[i].draw('#');

		SetConsoleTextAttribute(console, 252);  //red apple        
        fruit.draw('@');

        Sleep(100);
    }

public:
    PlayGame()
    {
        srand(time(NULL));

        char op = ' ';
        do
        {
            if(kbhit())
                op = getch();
            
            switch(op)
            {
                case 'w':   case 'W':   turnUp();       break;
                case 's':   case 'S':   turnDown();     break;
                case 'a':   case 'A':   turnLeft();     break;
                case 'd':   case 'D':   turnRight();    break;
            }

            runGame();
        }
        while(op != 'e' && op != 'e');    
    }
};


int main()
{
	PlayGame snake;
	return 0;
}