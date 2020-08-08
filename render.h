#pragma once
#define render_width 80
#define render_height 25

#include <iostream>
#include <conio.h>
#include <windows.h>
#include "colors.h"

COORD position;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


struct dots {

    char symbol;
    color sycolor;
    color bgcolor;

};

class RENDER {
private:

    dots back_buffer[render_height][render_width];
    dots screen_buffer[render_height][render_width];

    int display_x, display_y;

    bool active_draw;

public:

    RENDER(bool active_draw) {
        this->active_draw = active_draw;
    }

	void writeLine(int x, int y, char *text, color textColor, color bgColor) {
		int i = 0;
		while(text[i] != '\0') {
			addSymbol(x + i, y, text[i], textColor, bgColor);
			i++;
		}
	}

	void resetColor() {
		SetConsoleTextAttribute(hConsole, (WORD) ((Black << 4) | White));
	}

	void setColor(color syColor, color bgColor) {
		SetConsoleTextAttribute(hConsole, (WORD) ((bgColor << 4) | syColor));
	}

	void setCursor(bool visible) {
		CONSOLE_CURSOR_INFO CCI;
		CCI.bVisible = visible;
		CCI.dwSize=1;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CCI);
	}

    void clear() {
        for(int i = 0; i < render_height; i++) {
            for(int j = 0; j < render_width; j++) {
                back_buffer[i][j].symbol = char(0);
                back_buffer[i][j].sycolor = Black;
                back_buffer[i][j].bgcolor = Black;
            }
        }
    }

	void clear(char symbol, color syColor, color bgColor) {
		for(int i = 0; i < render_height; i++) {
            for(int j = 0; j < render_width; j++) {
                back_buffer[i][j].symbol = symbol;
                back_buffer[i][j].sycolor = syColor;
                back_buffer[i][j].bgcolor = bgColor;
            }
        }
	}

	void clear(color bgColor) {
        for(int i = 0; i < render_height; i++) {
            for(int j = 0; j < render_width; j++) {
                back_buffer[i][j].symbol = char(176);
                back_buffer[i][j].sycolor = Black;
                back_buffer[i][j].bgcolor = bgColor;
            }
        }
    }

    void drawHLine(int start_y, int length, char symbol, color cs, color bg) {
        for(int i = 0; i < length; i++) {
            back_buffer[start_y][i].symbol = symbol;
            back_buffer[start_y][i].sycolor = cs;
            back_buffer[start_y][i].bgcolor = bg;
        }
    }

    void addSymbol(int x, int y, char s, color cs, color bg) {
        back_buffer[y][x].symbol = s;
        back_buffer[y][x].sycolor = cs;
        back_buffer[y][x].bgcolor = bg;
    }

	void addSymbolOnScreenBuffer(int x, int y, char s, color cs, color bg) {
		screen_buffer[y][x].symbol = s;
        screen_buffer[y][x].sycolor = cs;
        screen_buffer[y][x].bgcolor = bg;
	}

	void delSymbol(int x, int y) {
		addSymbol(x, y, char(176), Black, LightMagenta);
		addSymbolOnScreenBuffer(x, y, char(176), Black, LightMagenta);
	}

    void gotoXY(int x, int y) {
        position.X = x;
        position.Y = y;
        SetConsoleCursorPosition(hConsole, position);
    }

    void gotoxyf(int x, int y, char symbol, color color_text, color bg_color) {
        position.X = x;
        position.Y = y;
        SetConsoleTextAttribute(hConsole, (WORD) ((bg_color << 4) | color_text));
        SetConsoleCursorPosition(hConsole, position);
        std::cout << symbol;
    }

    void draw() {
        for(int i = 0; i < render_height; i++) {
            for(int j = 0; j < render_width; j++) {
                if(screen_buffer[i][j].symbol != back_buffer[i][j].symbol ||
                    screen_buffer[i][j].sycolor != back_buffer[i][j].sycolor ||
                    screen_buffer[i][j].bgcolor != back_buffer[i][j].bgcolor) {
                    screen_buffer[i][j].symbol = back_buffer[i][j].symbol;
                    screen_buffer[i][j].sycolor = back_buffer[i][j].sycolor;
                    screen_buffer[i][j].bgcolor = back_buffer[i][j].bgcolor;
                    gotoxyf(j, i, screen_buffer[i][j].symbol, screen_buffer[i][j].sycolor, screen_buffer[i][j].bgcolor);
                }
            }
        }
		gotoXY(0, 0);
    }
};
