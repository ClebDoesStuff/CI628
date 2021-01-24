#include "MyGame.h"
using namespace std;

void MyGame::on_receive(std::string cmd, std::vector<std::string>& args) {
    if (cmd == "GAME_DATA") {
        // we should have exactly 4 arguments
        if (args.size() == 4) {
            game_data.player1Y = stoi(args.at(0));
            game_data.player2Y = stoi(args.at(1));
            game_data.ballX = stoi(args.at(2));
            game_data.ballY = stoi(args.at(3));
        }
    } 
    else {
        std::cout << "Received: " << cmd << std::endl;
    }
    if (cmd == "SCORES") {
        if (args.size() == 2) {
            scores.player1score = stoi(args.at(0));
            scores.player2score = stoi(args.at(1));
        }
    } 
}

void MyGame::send(std::string message) {
    messages.push_back(message);
}

void MyGame::input(SDL_Event& event) {
    switch (event.key.keysym.sym) {
        case SDLK_w:
            send(event.type == SDL_KEYDOWN ? "W_DOWN" : "W_UP");
            break;
        case SDLK_s:
            send(event.type == SDL_KEYDOWN ? "S_DOWN" : "S_UP");
            break;
        case SDLK_i:
            send(event.type == SDL_KEYDOWN ? "I_DOWN" : "I_UP");
            break;
        case SDLK_k:
            send(event.type == SDL_KEYDOWN ? "K_DOWN" : "K_UP");
            break;
    }
}

void MyGame::update() {
    player1.y = game_data.player1Y;
    player2.y = game_data.player2Y;
    ball.y = game_data.ballY;
    ball.x = game_data.ballX;
    //std::cout << "player1.y = " << player1.y << std::endl;
    //std::cout << "player2.y = " << player2.y << std::endl;
    //std::cout << "ball.y = " << ball.y << std::endl;
    //std::cout << "ball.x = " << ball.x << std::endl;
    p1score = scores.player1score;
    p2score = scores.player2score;
    //cout << "P1: " << p1score << std::endl;
    //cout << "P2: " << p2score << std::endl;
}


// This was obtained at https://stackoverflow.com/questions/36449616/sdl2-how-to-draw-dotted-line
// it uses the Bresenham Algoithm to create a dotted line
void DrawDottedLine(SDL_Renderer* renderer, int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;
    int count = 0;
    while (1) {
        if (count < 8) { SDL_RenderDrawPoint(renderer, x0, y0); }
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 > dy) { err += dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
        count = (count + 1) % 20;
    }
}

void MyGame::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &player1);
    SDL_RenderFillRect(renderer, &player2);
    SDL_RenderFillRect(renderer, &ball);
    DrawDottedLine(renderer, 400, 0, 400, 600);
 }


