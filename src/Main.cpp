#include "SDL_net.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include "MyGame.h"

using namespace std;

const char* IP_NAME = "localhost";
const Uint16 PORT = 55555;

bool is_running = true;

MyGame* game;

static int on_receive(void* socket_ptr) {
    TCPsocket socket = (TCPsocket)socket_ptr;

    const int message_length = 1024;

    char message[message_length];
    int received;

    // TODO: while(), rather than do
    do {
        received = SDLNet_TCP_Recv(socket, message, message_length);
        message[received] = '\0';

        char* pch = strtok(message, ",");

        // get the command, which is the first string in the message
        string cmd(pch);

        // then get the arguments to the command
        vector<string> args;

        while (pch != NULL) {
            pch = strtok(NULL, ",");

            if (pch != NULL) {
                args.push_back(string(pch));
            }
        }

        game->on_receive(cmd, args);

        if (cmd == "exit") {
            break;
        }

    } while (received > 0 && is_running);

    return 0;
}

static int on_send(void* socket_ptr) {
    TCPsocket socket = (TCPsocket)socket_ptr;

    while (is_running) {
        if (game->messages.size() > 0) {
            string message = "CLIENT_DATA";

            for (auto m : game->messages) {
                message += "," + m;
            }

            game->messages.clear();

            cout << "Sending_TCP: " << message << endl;

            SDLNet_TCP_Send(socket, message.c_str(), message.length());
        }

        SDL_Delay(1);
    }

    return 0;
}

void loop(SDL_Renderer* renderer) {
    SDL_Event event;

    while (is_running) {
        // input
        while (SDL_PollEvent(&event)) {
            if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && event.key.repeat == 0) {
                game->input(event);

                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        is_running = false;
                        SDL_Quit();
                        break;

                    default:
                        break;
                }
            }

            if (event.type == SDL_QUIT) {
                is_running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        game->update();

        game->render(renderer);

        SDL_RenderPresent(renderer);

        SDL_Delay(17);
    }
}

int run_game() {
    SDL_Window* window = SDL_CreateWindow(
        "Multiplayer Pong Client",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    if (nullptr == window) {
        std::cout << "Failed to create window" << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (nullptr == renderer) {
        std::cout << "Failed to create renderer" << SDL_GetError() << std::endl;
        return -1;
    }

    loop(renderer);

    return 0;
}

int main(int argc, char** argv) {
   
    // Initialize SDL
    if (SDL_Init(0) == -1) {
        printf("SDL_Init: %s\n", SDL_GetError());
        exit(1);
    }

    // Initialize SDL_net
    if (SDLNet_Init() == -1) {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(2);
    }

    IPaddress ip;

    // Resolve host (ip name + port) into an IPaddress type
    if (SDLNet_ResolveHost(&ip, IP_NAME, PORT) == -1) {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(3);
    }

    // Open the connection to the server
    TCPsocket socket = SDLNet_TCP_Open(&ip);

    if (!socket) {
        printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(4);
    }

    // Initialize TTF
    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(5);
    }
    
    // Initialize IMG
    int loadImg = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP;
    int initialImg = IMG_Init(loadImg);
    if ((loadImg & initialImg) != loadImg) {
        printf("IMG_Init: Initialising image process failed, images unsupported!\n");
        printf("IMG_Init: %s\n", IMG_GetError());
    }
    if (IMG_Init(0) == -1) {
        printf("IMG_Init: %s\n", IMG_GetError());
        exit(6);
    }

    // Initialize Mix
    int loadMIX = MIX_INIT_FLAC | MIX_INIT_MID | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_OPUS;
    int initialMIX = Mix_Init(loadMIX);
    if ((loadMIX & initialMIX) != loadMIX) {
        printf("Mix_Init: Initialising image process failed, music unsupported!\n");
        printf("Mix_Init: %s\n", Mix_GetError());
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
        exit(7);
    }
    else {
        std::cout << "Audio is opened" << std::endl;
    }

    //Load Font
    TTF_Font* font = TTF_OpenFont("assets/Peepo.ttf", 64);
    if (font != nullptr) {
        cout << "font was loaded" << endl;
    }
    else {
        cout << "font is not loaded" << endl;
        printf("TTF_Load: %s\n", TTF_GetError());
    }

    //Load Images
    SDL_Surface* background = IMG_Load("assets/background.png");
    
    SDL_Surface* batleft = IMG_Load("assets/batleft.png");
    
    SDL_Surface* batright = IMG_Load("assets/batright.png");

    SDL_Surface* starball = IMG_Load("assets/starball.png");

    //Check Images aren't nullptr
    if (background != nullptr) {
        cout << "background is loaded" << endl;
    }
    else {
        cout << "background is not loaded" << endl;
        printf("TMG_Load: %s\n", IMG_GetError());
    }
    if (batleft != nullptr) {
        cout << "batleft is loaded" << endl;
    }
    else {
        cout << "batleft is not loaded" << endl;
        printf("TMG_Load: %s\n", IMG_GetError());
    }
    if (batright != nullptr) {
        cout << "batright is loaded" << endl;
    }
    else {
        cout << "batright is not loaded" << endl;
        printf("TMG_Load: %s\n", IMG_GetError());
    }
    if (starball != nullptr) {
        cout << "starball is loaded" << endl;
    }
    else {
        cout << "starball is not loaded" << endl;
        printf("TMG_Load: %s\n", IMG_GetError());
    }

    //Load audio
    Mix_Chunk* hitbat = Mix_LoadWAV("assets/hitbat.wav");
    
    Mix_Chunk* hitwall = Mix_LoadWAV("assets/hitwall.wav");

    Mix_Chunk* hitside = Mix_LoadWAV("assets/hitside.wav");

    Mix_Chunk* tbgm = Mix_LoadWAV("assets/bgm.wav");

    Mix_Music* bgm = Mix_LoadMUS("assets/bgm2.mp3");

    //Check audios aren't nullptr
    if (bgm != nullptr) {
        cout << "bgm is loaded" << endl;
    }else{
        printf("Mix_LoadMUS(\"bgm2.mp3\"): %s\n", Mix_GetError());
    }

    if (hitbat != nullptr) {
        cout << "hitbat is loaded" << endl;
    }
    else {
        cout << "hitbat is not loaded" << endl;
        printf("Mix_LoadWAV: %s\n", Mix_GetError());
    }
    
    if (hitwall != nullptr) {
        cout << "hitwall is loaded" << endl;
    }
    else {
        cout << "hitwall is not loaded" << endl;
        printf("Mix_LoadWAV: %s\n", Mix_GetError());
    }

    if (hitside != nullptr) {
        cout << "hitside is loaded" << endl;
    }
    else {
        cout << "hitside is not loaded" << endl;
        printf("Mix_LoadWAV: %s\n", Mix_GetError());
    }

    if (bgm != nullptr) {
        cout << "bgm is loaded" << endl;
    }
    else {
        cout << "bgm is not loaded" << endl;
        printf("Mix_LoadWAV: %s\n", Mix_GetError());
    }

    //Change volume of sounds
    Mix_VolumeChunk(hitbat, 1.5);
    Mix_VolumeChunk(hitwall, 4);
    Mix_VolumeChunk(hitside, 3);
    
    //Change volume of music
    Mix_VolumeMusic(6);

    //Play music
    Mix_PlayMusic(bgm, -1);

    //send variables to MyGame
    game = new MyGame(font, background, batleft, batright, starball, hitbat, hitwall, hitside);

    SDL_CreateThread(on_receive, "ConnectionReceiveThread", (void*)socket);
    SDL_CreateThread(on_send, "ConnectionSendThread", (void*)socket);

    run_game();

    delete game;

    // Close connection to the server
    SDLNet_TCP_Close(socket);

    // Shutdown SDL_net
    SDLNet_Quit();

    TTF_CloseFont(font);
    SDL_FreeSurface(batleft);
    SDL_FreeSurface(batright);
    SDL_FreeSurface(background);
    SDL_FreeSurface(starball);
    Mix_FreeChunk(hitbat);
    Mix_FreeChunk(hitwall);
    Mix_FreeChunk(hitside);

    // Shutdown SDL
    SDL_Quit();

    return 0;
}