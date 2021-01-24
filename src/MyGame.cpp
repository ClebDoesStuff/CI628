#include "MyGame.h"

//MyGame::MyGame(TTF_Font* font) {
//    this->font = font;
//}

using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;

const int USE_FULL_TEXTURE = NULL;

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

void DrawImage(SDL_Renderer* renderer, const char* location, int x, int y, int w, int h) {
    SDL_Rect dst = { x, y, w , h };
    auto surface = IMG_Load(location);
    if (surface != nullptr) {
        //cout << location << " was loaded" << endl;
    }
    else {
        //cout << location << " has not been loaded" << endl;
    }

    auto texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_RenderCopy(renderer, texture, USE_FULL_TEXTURE, &dst);
    //SDL_RenderPresent(renderer);
}

void MyGame::DrawText(SDL_Renderer* renderer, const char* location, int fontsize, int score, int x, int y, int w, int h) {
    TTF_Font* font = TTF_OpenFont(location, fontsize);
    if (font != nullptr) {
        //cout << "font was loaded" << endl;
    }
    else {
        //cout << "font has not been loaded" << endl;
    }
    
    SDL_Color text_color = { 255, 255, 255, 255 };
    
    string text_score = to_string(score);
    
    SDL_Surface* text_surface = TTF_RenderText_Blended(font, text_score.c_str(), text_color);

    if (text_surface != nullptr) {
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        SDL_FreeSurface(text_surface);
        if (text_texture != nullptr) {
            SDL_QueryTexture(text_texture, NULL, NULL, &w, &h);

            SDL_Rect dst = { x,y,w,h };

            //NULL to draw 
            SDL_RenderCopy(renderer, text_texture, NULL, &dst);
        }
    }
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
    DrawImage(renderer, "assets/background.png", 0, 0, WIDTH, HEIGHT);
    DrawDottedLine(renderer, 410, 0, 410, 600);
    SDL_RenderFillRect(renderer, &player1);
    SDL_RenderFillRect(renderer, &player2);
    SDL_RenderFillRect(renderer, &ball);
    DrawText(renderer, "assets/Peepo.ttf", 64, p1score, 100, 20, 40, 40);
    DrawText(renderer, "assets/Peepo.ttf", 64, p2score, 650, 20, 40, 40);
 }



