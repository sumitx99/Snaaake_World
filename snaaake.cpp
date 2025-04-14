#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>

using namespace std;

bool gameOver;
bool paused = false;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
int speed;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
}

void Draw() {
    system("cls");

    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0) cout << "#";

            if (i == y && j == x)
                cout << "O";
            else if (i == fruitY && j == fruitX)
                cout << "F";
            else {
                bool print = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "o";
                        print = true;
                    }
                }
                if (!print) cout << " ";
            }

            if (j == width - 1) cout << "#";
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << endl;

    cout << "Score: " << score << endl;
    if (paused) cout << "** Paused ** Press 'P' to resume" << endl;
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            if (!paused && dir != RIGHT) dir = LEFT;
            break;
        case 'd':
            if (!paused && dir != LEFT) dir = RIGHT;
            break;
        case 'w':
            if (!paused && dir != DOWN) dir = UP;
            break;
        case 's':
            if (!paused && dir != UP) dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        case 'p':
            paused = !paused;
            break;
        }
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    // Wall collision
    if (x < 0 || x >= width || y < 0 || y >= height)
        gameOver = true;

    // Self collision
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;
    }

    // Eat fruit
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
        Beep(523, 100); // Play sound
    }
}

void SaveHighScore() {
    int highscore = 0;
    ifstream inFile("highscore.txt");
    if (inFile.is_open()) {
        inFile >> highscore;
        inFile.close();
    }

    if (score > highscore) {
        ofstream outFile("highscore.txt");
        outFile << score;
        outFile.close();
        cout << "🎉 New High Score: " << score << "!" << endl;
    } else {
        cout << "High Score: " << highscore << endl;
    }
}

int main() {
    cout << "Choose difficulty (1 = Easy, 2 = Medium, 3 = Hard): ";
    int choice;
    cin >> choice;
    switch (choice) {
    case 1: speed = 150; break;
    case 2: speed = 100; break;
    case 3: speed = 50; break;
    default: speed = 100;
    }

    Setup();
    while (!gameOver) {
        Draw();
        Input();
        if (!paused)
            Logic();
        Sleep(speed);
    }

    SaveHighScore();
    cout << "Game Over! Final Score: " << score << endl;

    return 0;
}