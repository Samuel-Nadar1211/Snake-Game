//C++ Progrm to implement Snake Game

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <direct.h>
#include <ctime>
#include <vector>

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


class Point : public Console
{
public:
    int x, y;

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
        if (y <= 0)  y = HEIGHT - 1;
    }
    
    void moveDown()
    {
        y++;
        if (y >= HEIGHT)  y = 1;
    }
    
    void moveLeft()
    {
        x--;
        if (x <= 0)  x = WIDTH - 1;
    }
    
    void moveRight()
    {
        x++;
        if (x >= WIDTH)  x = 1;
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


class Snake : virtual protected Console
{
protected:
    vector<Point> snake;
    char direction;
    Point fruit;
    bool is_alive, started;

    Snake()
    {
        setCursorInfo(false, 0);
        snake.push_back(Point(20, 20));
        fruit.setPoint(50, 10);
        is_alive = started = false;
    }

    void grow(int x = 0, int y = 0)
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

    void move()
    {
        for(int i = snake.size() - 1; i > 0; i--)   //Snake moves
            snake[i] = snake[i-1];
        
        switch(direction)   //Change direction of head
        {
            case 'w':   snake[0].moveUp();      break;
            case 's':   snake[0].moveDown();    break;
            case 'a':   snake[0].moveLeft();    break;
            case 'd':   snake[0].moveRight();   break;
        }
    }    
};


class Obstacle : virtual public Console
{
protected:
    vector<vector<Point>> obstacle;
    
    Obstacle()
    {
        obstacle.push_back({Point(30, 10), Point(40, 20)});
    }

    void drawFrame()
    {
        SetConsoleTextAttribute(console, 86);
        Point temp;
        for (int j = 0; j <= WIDTH; j++)
        {
            temp = Point(j, 0);
            temp.draw('*');
            temp = Point(j, HEIGHT);
            temp.draw('*');
        }
        for (int j = 1; j < HEIGHT; j++)
        {
            temp = Point(0, j);
            temp.draw('*');
            temp = Point(WIDTH, j);
            temp.draw('*');
        }
    }

    void drawObstacle()
    {
        SetConsoleTextAttribute(console, 108);
        Point temp;
        for (int i = 0; i < obstacle.size(); i++)
            for (int j = obstacle[i][0].x; j < obstacle[i][1].x; j++)
                for (int k = obstacle[i][0].y; k < obstacle[i][1].y; k++)
                {
                    temp = Point(j, k);
                    temp.draw('+');
                }
    }
};


class GameOver : protected Snake, protected Obstacle
{
protected:
    bool selfCollision()
    {
        for(int i = 1; i < snake.size() - 1; i++)
            if (snake[0] == snake[i])   return true;
        return false;
    }

    bool obstacleCollision()
    {
        for (int i = 0; i < obstacle.size(); i++)
            if (obstacle[i][0].x <= snake[0].x && obstacle[i][1].x > snake[0].x && obstacle[i][0].y <= snake[0].y && obstacle[i][1].y > snake[0].y)
                return true;
        return false;
    }
};


class PlayGame : protected GameOver
{
    void welcomeScreen()
    {
        SetConsoleTextAttribute(console, 140);
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
    
    void setNewFruit()
    {
        int x = rand() % (WIDTH - 1) + 1;
        int y = rand() % (HEIGHT - 1) + 1;
        
        bool valid = true;
        for (int i = 0; i < obstacle.size(); i++)
            if (obstacle[i][0].x <= x && obstacle[i][1].x > x && obstacle[i][0].y <= y && obstacle[i][1].y > y)
            {
                valid = false;
                break;
            }
        if (valid)  fruit.setPoint(x, y);
        else    setNewFruit();
    }

    void display()
    {
        drawFrame();
        drawObstacle();

        SetConsoleTextAttribute(console, 37);  //green snake
        for(int i = 0; i < snake.size(); i++)
			snake[i].draw('#');

		SetConsoleTextAttribute(console, 78);  //red apple        
        fruit.draw('@');

        SetConsoleTextAttribute(console, 180);
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
                snake.erase(snake.begin(), snake.end());
                snake.push_back(Point(20, 20));
            }
        
        move();

        if (selfCollision() || obstacleCollision()) is_alive = false;

        if (fruit == snake[0])      //eat fruit
        {
            grow();
            setNewFruit();
        }

        display();
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
        while(op != 'e' && op != 'E');    
    }
};


int main()
{
	PlayGame snake;
	return 0;
}
