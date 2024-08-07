// C++ Progrm to implement Snake Game

#include <iostream>
#include <iomanip>
#include <conio.h>
#include <windows.h>
#include <direct.h>
#include <ctime>
#include <fstream>
#include <cstring>
#include <vector>
#include <utility>

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

    void setCursorPosition(int x, int y)
    {
        cursor_position.X = x;
        cursor_position.Y = y;
        SetConsoleCursorPosition(console, cursor_position);
    }

    void setCursorInfo(bool visible, DWORD size)
    {
        if (size == 0)
            size = 20;
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
        this->x = x;
        this->y = y;
    }

    void moveUp()
    {
        y--;
        if (y <= 0)
            y = HEIGHT - 1;
    }

    void moveDown()
    {
        y++;
        if (y >= HEIGHT)
            y = 1;
    }

    void moveLeft()
    {
        x--;
        if (x <= 0)
            x = WIDTH - 1;
    }

    void moveRight()
    {
        x++;
        if (x >= WIDTH)
            x = 1;
    }

    void draw(char ch)
    {
        setCursorPosition(x, y);
        cout << ch;
    }

    void erase()
    {
        setCursorPosition(x, y);
        cout << ' ';
    }

    bool operator==(Point p) // Collision
    {
        return x == p.x && y == p.y;
    }
};

class Snake : virtual protected Console
{
protected:
    vector<Point> snake;
    char direction = 'w';
    Point fruit, tail;
    bool is_alive, started;

    Snake()
    {
        setCursorInfo(false, 0);
        snake.push_back(Point(20, 20));
        fruit.setPoint(50, 10);
        is_alive = started = false;
    }

    void grow()
    {
        snake.push_back(Point(20, 20));
    }

    void turnUp()
    {
        if (direction != 's')
            direction = 'w';
    }

    void turnDown()
    {
        if (direction != 'w')
            direction = 's';
    }

    void turnLeft()
    {
        if (direction != 'd')
            direction = 'a';
    }

    void turnRight()
    {
        if (direction != 'a')
            direction = 'd';
    }

    void move()
    {
        for (int i = snake.size() - 1; i > 0; i--) // Snake moves
            snake[i] = snake[i - 1];

        switch (direction) // Change direction of head
        {
        case 'w':
            snake[0].moveUp();
            break;
        case 's':
            snake[0].moveDown();
            break;
        case 'a':
            snake[0].moveLeft();
            break;
        case 'd':
            snake[0].moveRight();
            break;
        }
    }

    void drawSnake()
    {
        SetConsoleTextAttribute(console, 37);
        snake[0].draw('O');
        for (int i = 1; i < snake.size(); i++)
            snake[i].draw('#');
    }
};

class Obstacle : virtual protected Console
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

        switch (rand() % 3)
        {
        case 0:
            obstacle1();
            break;
        case 1:
            obstacle2();
            break;
        case 2:
            obstacle3();
            break;
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

class Collision : protected Snake, protected Obstacle
{
protected:
    bool selfCollision()
    {
        for (int i = 1; i < snake.size() - 1; i++)
            if (snake[0] == snake[i])
                return true;
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

class Score
{
    pair<char[20], int> high_score[5];
    void updateFile()
    {
        ofstream fout("HighScore.dat", ios::out | ios::binary);

        for (int i = 0; i < 5; i++)
            fout.write(reinterpret_cast<char *>(&high_score[i]), sizeof(pair<char[20], int>));
        fout.close();
    }

protected:
    int score = 1001;

    Score()
    {
        ifstream fin("HighScore.dat", ios::in | ios::binary);

        if (fin.is_open())
        {
            for (int i = 0; i < 5; i++)
                fin.read(reinterpret_cast<char *>(&high_score[i]), sizeof(pair<char[20], int>));
            fin.close();
        }
        else
        {
            strcpy(high_score[0].first, "Kavitha");
            strcpy(high_score[1].first, "Rexlin");
            strcpy(high_score[2].first, "George");
            strcpy(high_score[3].first, "Nivetha");
            strcpy(high_score[4].first, "Samuel");
            high_score[0].second = 50000;
            high_score[1].second = 25000;
            high_score[2].second = 10000;
            high_score[3].second = 7500;
            high_score[4].second = 5000;
            updateFile();
        }
    }

    void printHighScore()
    {
        cout << "\n\t\tLeaderboard-\n";
        for (int i = 0; i < 5; i++)
            cout << "\t\t" << i + 1 << ". " << setw(20) << left << high_score[i].first << " - " << high_score[i].second << endl;
    }

    void updateHighScore()
    {
        cout << "\n\t\tYour score: " << score << endl;

        if (high_score[4].second >= score)
            return;

        char name[20];
        cout << "Enter your name: ";
        cin.getline(name, 20);

        int i;
        for (i = 3; i >= 0; i--)
            if (high_score[i].second >= score)
            {
                strcpy(high_score[i + 1].first, name);
                high_score[i + 1].second = score;
                break;
            }
            else
            {
                strcpy(high_score[i + 1].first, high_score[i].first);
                high_score[i + 1].second = high_score[i].second;
            }
        if (i < 0)
        {
            strcpy(high_score[0].first, name);
            high_score[0].second = score;
        }

        cout << "\n\t\tCongrats!! You are in the leaderboard\n";
        updateFile();
    }
};

class GameManager : protected Collision, protected Score
{
protected:
    void gameLogo()
    {
        SetConsoleTextAttribute(console, 245);
        cout << "\n\t\t\t\t\t|         /\\/\\/\\                         |";
        cout << "\n\t\t\t\t\t|        | ___ |                         |";
        cout << "\n\t\t\t\t\t|        \\/   \\/                         |";
        cout << "\n\t\t\t\t\t|        /     \\                         |";
        cout << "\n\t\t\t\t\t|       / /\\ /\\ \\                        |";
        cout << "\n\t\t\t\t\t|       | \\/ \\/ |                        |";
        cout << "\n\t\t\t\t\t|       \\\\  _   /                        |";
        cout << "\n\t\t\t\t\t|        \\\\    /                         |";
        cout << "\n\t\t\t\t\t|         |    |       _...._            |";
        cout << "\n\t\t\t\t\t|         |    \\\\    /       \\           |";
        cout << "\n\t\t\t\t\t|         \\\\    \\\\__/    __   |          |";
        cout << "\n\t\t\t\t\t|          \\\\          /  \\\\  |          |";
        cout << "\n\t\t\t\t\t|           \\\\________/    \\\\/           |";
        cout << "\n\t\t\t\t\t|                                        |";
        cout << "\n\t\t\t\t\t|          SAMUEL's SNAKE GAME!!         |";
    }

    void printRules()
    {
        cout << "\n\t\tRules:\n";
        cout << "\n\t\t\t1. A -> Left\tS -> Down\tW -> Up  \tD -> Right\tP -> Pause\t\n";
        cout << "\t\t\t2. Press E to Exit\n";
        cout << "\t\t\t3. Don't hit the block or yourself\n";
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

        if (valid)
            fruit.setPoint(x, y);
        else
            setNewFruit();
    }

    void display()
    {
        SetConsoleTextAttribute(console, 180);
        tail.erase();
        tail = snake[snake.size() - 1];

        SetConsoleTextAttribute(console, 37); // green snake
        snake[0].draw('O');
        if (snake.size() > 1)
            snake[1].draw('#');

        SetConsoleTextAttribute(console, 78); // red apple
        fruit.draw('@');
    }

    void reset()
    {
        is_alive = true;
        score = 1001;

        snake.erase(snake.begin(), snake.end());
        snake.push_back(Point(20, 20));

        initialiseObstacle();
        fruit.setPoint(50, 10);
    }
};

class SwitchScreen : protected GameManager
{
protected:
    void confirmExit()
    {
        SetConsoleTextAttribute(console, 223);
        system("cls");

        setCursorPosition(WIDTH / 3, HEIGHT / 3);
        cout << "Do you want to exit the game?";
        setCursorPosition(WIDTH / 3, HEIGHT / 3 + 1);
        cout << "Your score will not be saved";
        setCursorPosition(WIDTH / 3, HEIGHT / 3 + 2);
        cout << "Press N to reject and any other key to confirm: ";

        char choice;
        cin >> choice;

        if (choice == 'n' || choice == 'N')
            resume();
        else
        {
            SetConsoleTextAttribute(console, 245);
            system("cls");

            gameLogo();
            cout << "\n\n\t\t\t\t\tThank you for Playing";
            Sleep(2000);

            exit(0);
        }
    }

    void pause()
    {
        SetConsoleTextAttribute(console, 223);
        system("cls");

        setCursorPosition(WIDTH / 3, HEIGHT / 3);
        cout << "Game is paused!";
        setCursorPosition(WIDTH / 3, HEIGHT / 3 + 1);
        cout << "Your Score: " << score;
        setCursorPosition(WIDTH / 3, HEIGHT / 3 + 2);
        cout << "Do you want to resume the game (Press Y): ";

        char choice;
        do
        {
            setCursorPosition(WIDTH / 3 + 43, HEIGHT / 3 + 2);
            cin >> choice;
        } while (choice != 'Y' && choice != 'y');

        resume();
    }

    void resume()
    {
        SetConsoleTextAttribute(console, 180);
        system("cls");

        drawFrame();
        drawObstacle();
        drawSnake();
    }
};

class PlayGame : protected SwitchScreen
{
    bool runGame()
    {
        if (!is_alive)
        {
            SetConsoleTextAttribute(console, 245);
            system("cls");
            gameLogo();

            if (!started)
            {
                cout << "\n\n\t\tWelcome to the Game!!\n";
                printRules();
                cout << "\n\n\t\tPress any key other than E to start\t";

                char c = getch();
                if (c == 'E' || c == 'e')
                    return false;

                is_alive = started = true;
            }

            else
            {
                cout << "\n\n\t\tGame Over :-(";

                updateHighScore();
                printHighScore();

                cout << "\n\t\tPress any key other than E to restart\t";

                char c = getch();
                if (c == 'E' || c == 'e')
                    return false;

                reset();
            }

            SetConsoleTextAttribute(console, 180);
            system("cls");

            drawFrame();
            drawObstacle();
            tail.setPoint(20, 20);
        }

        move();
        score--;

        setCursorPosition(WIDTH / 2 - 3, HEIGHT);
        SetConsoleTextAttribute(console, 228);
        cout << setfill('0') << setw(6) << right << score << setfill(' ');

        if (selfCollision() || obstacleCollision())
            is_alive = false;

        if (fruit == snake[0]) // eat fruit
        {
            grow();
            score += 1000;
            setNewFruit();
        }

        display();
        switch (direction) // Change direction of head
        {
        case 'w':
        case 's':
            Sleep(120);
            break;
        case 'a':
        case 'd':
            Sleep(85);
            break;
        }
        return true;
    }

public:
    PlayGame()
    {
        srand(time(NULL));

        char op = ' ';
        while (true)
        {
            if (kbhit())
                op = getch();

            switch (op)
            {
            case 'w':
            case 'W':
                turnUp();
                break;
            case 's':
            case 'S':
                turnDown();
                break;
            case 'a':
            case 'A':
                turnLeft();
                break;
            case 'd':
            case 'D':
                turnRight();
                break;
            case 'p':
            case 'P':
                pause();
                op = ' ';
                break;
            case 'e':
            case 'E':
                confirmExit();
                op = ' ';
                break;
            }

            if (!runGame())
                break;
        }

        SetConsoleTextAttribute(console, 245);
        system("cls");

        gameLogo();
        cout << "\n\n\t\t\t\t\tThank you for Playing";
        Sleep(2000);
    }
};

int main()
{
    PlayGame snake;
    return 0;
}
