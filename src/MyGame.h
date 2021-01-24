#ifndef __MY_GAME_H__
#define __MY_GAME_H__

#include <iostream>
#include <vector>
#include <string>

#include "SDL_image.h"
#include "SDL_ttf.h"

#include "SDL.h"

static struct GameData {
    int player1Y = 0;
    int player2Y = 0;
    int ballX = 0;
    int ballY = 0;
} game_data;

static struct Scoring {
    int player1score;
    int player2score;
} scores;

class MyGame {

    private:
        SDL_Rect player1 = { 200, 300, 20, 60 };
        SDL_Rect player2 = { 580, 300, 20, 60 };
        SDL_Rect ball = { 400, 300, 15, 15 };
        int p1score = 0;
        int p2score = 0;
        TTF_Font* font;
        

    public:
        std::vector<std::string> messages;

        void on_receive(std::string message, std::vector<std::string>& args);
        void send(std::string message);
        void input(SDL_Event& event);
        void update();
        void DrawText(SDL_Renderer* renderer, const char* location, int fontsize, int score, int x, int y, int w, int h);
        void render(SDL_Renderer* renderer);
};

#endif