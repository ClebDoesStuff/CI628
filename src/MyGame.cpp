#include "MyGame.h"

using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;

const int USE_FULL_TEXTURE = NULL;

MyGame::MyGame(TTF_Font* font, SDL_Surface* background, SDL_Surface* batleft, SDL_Surface* batright, 
    SDL_Surface* starball, Mix_Chunk* hitbat, Mix_Chunk* hitwall, Mix_Chunk* hitside) {
    this->font = font;
    this->background = background;
    this->batleft = batleft;
    this->batright = batright;
    this->starball = starball;
    this->hitbat = hitbat;
    this->hitwall = hitwall;
    this->hitside = hitside;
}

void MyGame::on_receive(string cmd, vector<string>& args) {
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
        cout << "Received: " << cmd << endl;
    }

    if (cmd == "SCORES") {
        if (args.size() == 2) {
            scores.player1score = stoi(args.at(0));
            scores.player2score = stoi(args.at(1));
        }
    }

    if (cmd == "BALL_HIT_BAT1" || cmd == "BALL_HIT_BAT2") {
        PlaySound(hitbat, 0);
    }
    
    if (cmd == "HIT_WALL_LEFTGAME_DATA" || cmd == "HIT_WALL_RIGHTGAME_DATA") {
        PlaySound(hitwall, 0);
    }

    if (cmd == "HIT_WALL_UP" || cmd == "HIT_WALL_DOWN") {
        PlaySound(hitside, 0);
    }
}

void MyGame::send(string message) {
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


void DrawSpecial(SDL_Renderer* renderer, SDL_Surface* surface, const SDL_Rect* rect) {
    auto texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_RenderCopy(renderer, texture, USE_FULL_TEXTURE, rect);
    SDL_DestroyTexture(texture);
}

void DrawImage(SDL_Renderer* renderer, SDL_Surface* surface, int x, int y, int w, int h) {
    
    SDL_Rect dst = { x, y, w, h };
    auto texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_RenderCopy(renderer, texture, USE_FULL_TEXTURE, &dst);
    SDL_DestroyTexture(texture);
}

void MyGame::PlaySound(Mix_Chunk* sound, int loops) {
        Mix_PlayChannel(-1, sound, loops);
}

void MyGame::DrawText(SDL_Renderer* renderer, int score, int x, int y, int w, int h) {
    
    SDL_Color text_color = { 255, 255, 255, 255 };
    
    string text_score = to_string(score);
    
    SDL_Surface* text_surface = TTF_RenderText_Blended(font, text_score.c_str(), text_color);

    if (text_surface != nullptr) {
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        SDL_FreeSurface(text_surface);
        if (text_texture != nullptr) {
            SDL_QueryTexture(text_texture, NULL, NULL, &w, &h);

            SDL_Rect dst = { x, y, w, h };

            //NULL to draw 
            SDL_RenderCopy(renderer, text_texture, NULL, &dst);    
        }
        SDL_DestroyTexture(text_texture);
    }
}

void MyGame::render(SDL_Renderer* renderer) {
    //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    DrawImage(renderer, background, 0, 0, WIDTH, HEIGHT); 
    //Draw Score
    DrawText(renderer, p1score, 100, 20, 40, 40);
    DrawText(renderer, p2score, 650, 20, 40, 40);
    //Draw player bats and ball
    DrawSpecial(renderer, batleft, &player1);
    DrawSpecial(renderer, batright, &player2);
    DrawSpecial(renderer, starball, &ball);
 }