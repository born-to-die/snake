#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <ctime>
#include <Windows.h>
#include <cmath>
#include <string>
#include <fstream>

#include "colors.h"
#include "render.h"

using namespace std;

class Snake {
public:

    int tail[2000][2];

    int head_x,
        head_y,
        direction,
        length_tail,
        last_tail_x,
        last_tail_y,
        max_length,
        speed,
        max_speed,
        score,
        steps,
        add_speed;

    bool live;

    Snake(int x, int y) {
        live = true;
        head_x = x;
        head_y = y;
        direction = 1;
        length_tail = 3;
        max_length = 500;
        speed = 200;
        max_speed = 100;
        score = 0;
        steps = 0;
        add_speed = 1;

        tail[0][0] = head_x - 1;
        tail[0][1] = head_y;
        tail[1][0] = head_x - 2;
        tail[1][1] = head_y;
    }

    bool checkBorders() {
        if(head_y < 1 || head_y > 24 || head_x < 0 || head_x > 79) {
            live = false;
        }
    }

    void update(RENDER &Render) {

        last_tail_x = head_x;
        last_tail_y = head_y;

        switch(direction) {
            case 1: {
                head_x++;
                break;
            }
            case 0: {
                head_y--;
                break;
            }
            case 2: {
                head_y++;
                break;
            }
            case 3: {
                head_x--;
                break;
            }
        }

        Render.addSymbol(head_x, head_y, char(2), Yellow, Black);

        int tx, ty;

        for(int i = 0; i < length_tail; i++) {
            if(tail[i][0] > -1) {

                tx = tail[i][0];
                ty = tail[i][1];

                tail[i][0] = last_tail_x;
                tail[i][1] = last_tail_y;

                last_tail_x = tx;
                last_tail_y = ty;

                position.X = tail[i][0];
                position.Y = tail[i][1];
                Render.addSymbol(tail[i][0], tail[i][1], char(15), Green, Black);
            }
        }

        for(int i = 0; i < length_tail; i++) {
            if(head_x == tail[i][0] && head_y == tail[i][1]) {
                live = false;
                // for debug tail !!!!
                //Render.writeLine(head_x, head_y, "YOU DEAD", White, Black);
                //getch();
            }
        }

        //position.X = 0;
        //position.Y = 0;
       // SetConsoleCursorPosition(hConsole, position);
        //cout << "LENGTH: " << length_tail << "    SPEED: " << 1000 / speed;

        char buffer[5];

        Render.writeLine(0, 0, "LENGHT: ", Black, Red);
        Render.writeLine(8, 0, itoa(length_tail,buffer, 10) , DarkGray, Red);
        Render.writeLine(12, 0, "SPEED: ", Black, Red);
        Render.writeLine(19, 0, itoa(1000 / speed,buffer, 10) , DarkGray, Red);
        Render.writeLine(23, 0, "SCORE: ", Black, Red);
        Render.writeLine(30, 0, itoa(score,buffer, 10) , LightGray, Red);
        Render.writeLine(36, 0, "STEPS: ", Black, Red);
        Render.writeLine(43, 0, itoa(++steps,buffer, 10) , Black, Red);

        checkBorders();
    }

    void addTail() {
        if(length_tail < max_length) {
            length_tail++;
        }
    }

    void addSpeed() {
        if(speed > max_speed)
            speed -= add_speed;
    }
};

class Apple {
public:
    int x, y, step, need, type;

    void drop(Snake snake) {
        step = 0;
        type = 2;
        x = rand() % 80;
        y = 1 + rand() % 24;
        need = sqrt(pow(snake.head_x - x, 2) + pow(snake.head_y - y, 2)) * 2;
    }

    void update(RENDER &Render, Snake &snake) {

        if(step < need) {
            Render.addSymbol(x, y, char(15), LightRed, Black);
        }
        else if(step < need + 10) {
            Render.addSymbol(x, y, char(43), Red, Black);
            type = 1;
        }
        else if(step < need + 20){
            Render.addSymbol(x, y, char(46), DarkGray, Black);
            type = 0;
        }
        else {
            drop(snake);
        }

        if(snake.head_x == x && snake.head_y == y) {
            if(type == 2)
                snake.score += 10;
            else if(type == 1)
                snake.score += 5;
            else
                snake.score += 1;

            snake.addTail();
            snake.addSpeed();
            drop(snake);
        }

        step++;
    }


};

void setParams(RENDER &Render, Snake &snake) {

    int key = 0, cursor = 1, sum, max, min;
    int *point;
    char buffer[5];
    bool end_set = false;

    while(!end_set) {

        Render.clear();

        switch(cursor) {
            case 1: {
                point = &snake.max_speed;
                sum = 5;
                max = 9995;
                min = 0;
                break;
            }
            case 2: {
                point = &snake.speed;
                sum = 5;
                max = 400;
                min = 0;
                break;
            }
            case 3: {
                point = &snake.max_length;
                sum = 5;
                max = 2000;
                min = 3;
                break;
            }
            case 4: {
                point = &snake.add_speed;
                sum = 1;
                max = 10;
                min = 0;
                break;
            }
        }

        Render.writeLine(23, 1, "!!! THE OPTIMUM SETTINGS SET UP !!!", Red, Black);

        Render.writeLine(12, 3, "MAXIMUM SPEED:      (The less the faster)", LightGray, Black);
        Render.writeLine(28, 3, itoa(snake.max_speed, buffer, 10), White, Black);

        Render.writeLine(12, 6, "SPEED AT START:     (The less the faster)", LightGray, Black);
        Render.writeLine(28, 6, itoa(snake.speed, buffer, 10), White, Black);

        Render.writeLine(12, 9, "Maximum length: ", LightGray, Black);
        Render.writeLine(28, 9, itoa(snake.max_length, buffer, 10), White, Black);

        Render.writeLine(12, 12, "Add to speed: ", LightGray, Black);
        Render.writeLine(28, 12, itoa(snake.add_speed, buffer, 10), White, Black);

        Render.addSymbol(10, cursor * 3, char(15), Green, Black);

        Render.writeLine(35, 20, "PRESS ENTER", White, Black);

        Render.draw();

        key = getch();

        if(key == 80 && cursor < 5) // down
            cursor++;
        if(key == 72 && cursor > 1) // up
            cursor--;
        if(key == 77 && *point < max)
            *point += sum;
        if(key == 75 && *point > min)
            *point -= sum;
        if(key == 13)
            end_set = true;
    }
}

void getName(RENDER &Render, string &name) {
    Render.clear();
    Render.writeLine(10, 11, "Please, enter your name:", White, Black);
    Render.draw();
    Render.setColor(White, Black);
    Render.gotoXY(36, 11);
    cin >> name;
}

void lobby(RENDER &Render) {
    Render.clear();
    Render.writeLine(35, 1, "Soul Eater", LightRed, Black);
    Render.writeLine(0, 4, "You are the essence, and you are the essence of the devourer of souls. The more souls the bigger your size, but the higher you fall. The space in which you are has no dimension. You are in timelessness from nothing. In such a world, one    rule - to be, it is necessary that there is something in you. Eat your souls, if you do not eat them, then in time they go out and disappear.You are the essence, and you are the essence of the devourer of souls. The more souls the bigger   your size, but the higher you fall. The space in which you are has no dimension. You are in timelessness from nothing. In such a world, one rule - to be, it is necessary that there is something in you. Eat your souls, if you do not eat them, then in time they go out and disappear.", Red, Black);

    Render.addSymbol(2, 15, char(15), LightRed, Black);
    Render.writeLine(4, 15, "- 10 score", DarkGray, Black);
    Render.addSymbol(2, 17, char(43), Red, Black);
    Render.writeLine(4, 17, "- 5 score", DarkGray, Black);
    Render.addSymbol(2, 19, char(46), DarkGray, Black);
    Render.writeLine(4, 19, "- 1 score", DarkGray, Black);

    Render.writeLine(47, 18, "Collapsar Games", LightRed, Black);
    Render.writeLine(35, 19, "programmer intbad everything else artbsd", White, Black);
    Render.draw();
    getch();
}

void death(RENDER &Render, string name, Snake snake) {

    fstream file("scorex.txt", std::fstream::app);
    file << name << " -\t" << snake.score << " points\t" << snake.length_tail << " length\t" << snake.speed << " speed\t" << snake.steps << " steps\n";
    file.close();

    Render.clear();

    Render.writeLine(37, 11, "YOU DEAD", Red, Black);

    Render.draw();

    getch();

}

int main() {

    RENDER Render(true);

    srand(time(0));

    RESET:

    Snake snake(10, 10);
    Apple apple[20];
    string name;

    getName(Render, name);
    Render.setCursor(false);
    lobby(Render);

    setParams(Render, snake);

    for(int i = 0; i < 20; i++)
        apple[i].drop(snake);

    int key;
    bool game_active = true;

    while(game_active) {

        while(!kbhit()) {
            Render.clear();

            Render.drawHLine(0, 80, '#', Red, Red);

            for(int i = 0; i < 20; i++)
                apple[i].update(Render, snake);

            snake.update(Render);

            Render.draw();

            Sleep(snake.speed);

            if(snake.live == false) {
                game_active = false;
                break;
            }
        }

        if(game_active == false) {
            death(Render, name, snake);
            goto RESET;
        }

        key = getch();

        if(key == 80 && snake.direction != 0) // down
            snake.direction = 2;
        if(key == 77 && snake.direction != 3) // right
            snake.direction = 1;
        if(key == 72 && snake.direction != 2) // up
            snake.direction = 0;
        if(key == 75 && snake.direction != 1) // left
            snake.direction = 3;
        if(key == 27)
            getch();
        if(key == 71)
            snake.speed -= 25;
    }
}
