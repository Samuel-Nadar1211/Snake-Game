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
    void obstacle1()
    {
        obstacle.push_back({Point(1, 5), Point(15, 25)});
        obstacle.push_back({Point(30, 10), Point(40, 20)});
        obstacle.push_back({Point(35, 1), Point(60, 6)});
        obstacle.push_back({Point(50, 15), Point(70, 25)});
        obstacle.push_back({Point(70, 10), Point(90, 15)});
        obstacle.push_back({Point(100, 10), Point(WIDTH, HEIGHT)});
    }

    void obstacle2()
    {
        obstacle.push_back({Point(1, 1), Point(10, 20)});
        obstacle.push_back({Point(1, 1), Point(20, 10)});
        obstacle.push_back({Point(40, 15), Point(80, 20)});
        obstacle.push_back({Point(55, 5), Point(65, 25)});
        obstacle.push_back({Point(90, 3), Point(110, 15)});
        obstacle.push_back({Point(100, 20), Point(WIDTH, 25)});
    }

    void obstacle3()
    {
        obstacle.push_back({Point(5, 5), Point(25, 15)});
        obstacle.push_back({Point(35, 15), Point(40, 20)});
        obstacle.push_back({Point(45, 15), Point(60, HEIGHT)});
        obstacle.push_back({Point(65, 1), Point(85, 10)});
        obstacle.push_back({Point(70, 15), Point(100, 25)});
    }

protected:
    vector<vector<Point>> obstacle;
    
    Obstacle()
    {
        initialiseObstacle();
    }

    void initialiseObstacle()
    {
        obstacle.erase(obstacle.begin(), obstacle.end());

        switch(rand() % 3)
        {
            case 0: obstacle1();    break;
            case 1: obstacle2();    break;
            case 2: obstacle3();    break;
        }
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


//class 


class PlayGame : protected GameOver
{
    void gameLogo()
    {
        SetConsoleTextAttribute(console, 245);
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
        cout << "\n|          SAMUEL's SNAKE GAME!!         |";
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
        snake[0].draw('O');
        for(int i = 1; i < snake.size(); i++)
			snake[i].draw('#');

		SetConsoleTextAttribute(console, 78);  //red apple        
        fruit.draw('@');

        SetConsoleTextAttribute(console, 180);
    }

    void reset()
    {
        is_alive = true;
        snake.erase(snake.begin(), snake.end());
        snake.push_back(Point(20, 20));
        initialiseObstacle();
        fruit.setPoint(50, 10);
    }

    void runGame()
    {
        system("cls");
        
        if(!is_alive)
            if(!started)
            {
                gameLogo();
                cout << "\n\nWelcome to the Game!!\n";
                cout << "\n\nPress any key to start\t";
                getch();
                is_alive = started = true;
            }
            else
            {
                gameLogo();
                cout << "\n\nGame Over :-(";
                cout << "\nPress any key to restart\t";
                getch();
                reset();
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

        system("cls");
        gameLogo();
        cout << "\n\nThank you for Playing";
        Sleep(2000);
    }
};


int main()
{
	PlayGame snake;
	return 0;
}
