#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define PROGRAM_TITLE "test"

typedef struct {
    SDL_Window * window;
    SDL_Renderer * renderer;
}Window_t;

//template variables 
TTF_Font *freeMono;
static Window_t windowRend = {NULL, NULL};
int quitFlag = 1;
char* buffer;
char* programName;
int count;

int view_initWindow(void) {

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initialising SDL.\n");
        return 0;
    }
    windowRend.window = SDL_CreateWindow(  
        programName,
        100,
        100,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );
    if(!windowRend.window){
        fprintf(stderr, "Error creating SDL Window.\n");
        return 0;
    }
    windowRend.renderer = SDL_CreateRenderer(windowRend.window, -1, SDL_RENDERER_ACCELERATED);
    if(!windowRend.renderer){
        fprintf(stderr, "Error Creating SDL Renderer.\n");
        return 0;
    }

    if(TTF_Init() != 0){
        fprintf(stderr, "Error initialising SDL_TTF.\n");
        return 0;
    }
    freeMono = TTF_OpenFont("res/FreeMono.ttf", 24);

    return 1;
}

void view_title(){
    if(freeMono == NULL){
        printf("error printing TTF\n");
    }else{
        SDL_Color White = {255, 255, 255};        
        SDL_Surface *surfaceMessage = TTF_RenderText_Blended(freeMono, buffer, White);
        SDL_Texture *Message = SDL_CreateTextureFromSurface(windowRend.renderer, surfaceMessage);

        SDL_Rect Message_rect;
        Message_rect.x = 0;
        Message_rect.y = 0;
        Message_rect.w = surfaceMessage->w;
        Message_rect.h = surfaceMessage->h;

        SDL_RenderCopy(windowRend.renderer, Message, NULL, &Message_rect);
        SDL_FreeSurface(surfaceMessage);
    }    
}

void do_the_thing(){
    // crack on..
}

void view_render(){
    SDL_SetRenderDrawColor(windowRend.renderer, 0,0,0,255);
    SDL_RenderClear(windowRend.renderer);

    
    view_title();//optional render text

    do_the_thing(); // do your stuff


    SDL_RenderPresent(windowRend.renderer);
}

void view_quit(void) {
    SDL_DestroyRenderer(windowRend.renderer);
    SDL_DestroyWindow(windowRend.window);
    SDL_Quit();
}

void ctrl_processInput() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch(event.type){
        case SDL_QUIT:
        quitFlag = 0;
        break;
    }
}

void init_vars(){
    count = 0;
    buffer = malloc(256);
    programName = PROGRAM_TITLE;
}


void update() {
    int offset = sprintf(buffer, "Ticks: ");
    offset += sprintf(buffer + offset, "%d", count++);
}


int main(){
    init_vars();
    view_initWindow();
    printf("%s\n", programName);
    
    while(quitFlag){
        update();
        view_render();
        ctrl_processInput();        
        // SDL_Delay(50);
    }

    view_quit();
    return 0;
}
