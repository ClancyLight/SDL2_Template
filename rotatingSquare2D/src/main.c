#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define PROGRAM_TITLE "rotatingSquare2D"

#define PI 3.1415926536

typedef struct {
    SDL_Window * window;
    SDL_Renderer * renderer;
}Window_t;

typedef struct {
    float x;
    float y;
    // float z;
}Vector_t;

typedef struct {
    Vector_t a;
    Vector_t b;
    Vector_t c;
    Vector_t d;
    Vector_t center;
}Square_t;

Square_t squareA;

//template variables 
TTF_Font *freeMono;
static Window_t windowRend = {NULL, NULL};
int quitFlag = 1;
char* buffer;
char* programName;
int count;
double theta_g;
Vector_t origin;
Vector_t centerC ;
Vector_t rot = {200, -100};

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

void renderSquare(){
    
    SDL_SetRenderDrawColor(windowRend.renderer, 50, 200, 50, 255);
    //A->B
    SDL_RenderDrawLineF(windowRend.renderer, squareA.a.x, squareA.a.y, squareA.b.x, squareA.b.y);  
    SDL_RenderDrawLineF(windowRend.renderer, squareA.c.x, squareA.c.y, squareA.d.x, squareA.d.y);  
    
    SDL_RenderDrawLineF(windowRend.renderer, squareA.a.x, squareA.a.y, squareA.d.x, squareA.d.y);
    SDL_RenderDrawLineF(windowRend.renderer, squareA.b.x, squareA.b.y, squareA.c.x, squareA.c.y);
    
    SDL_RenderDrawLineF(windowRend.renderer, centerC.x, centerC.y, origin.x, origin.y);


}

void view_render(){
    SDL_SetRenderDrawColor(windowRend.renderer, 0,0,0,255);
    SDL_RenderClear(windowRend.renderer);

    
    view_title();//optional render text

    renderSquare(); // do your stuff


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

double deg2rad(int deg){
    return (deg * (PI / 180));
}

Square_t translate(Square_t s, float x, float y){
    
    s.a.x += x;
    s.a.y += y;
    s.b.x += x;
    s.b.y += y;
    s.c.x += x;
    s.c.y += y;
    s.d.x += x;
    s.d.y += y;
    s.center.x += x;
    s.center.y += y;

    return s;

}
void init_vars(){
    count = 0;
    buffer = malloc(256);
    programName = PROGRAM_TITLE;

    //create a square / clunky as 

    origin.x = WINDOW_WIDTH / 2;
    origin.y = WINDOW_HEIGHT / 2;

    Vector_t pA = {-50, -50};
    Vector_t pB = { 50, -50};
    Vector_t pC = { 50,  50};
    Vector_t pD = {-50,  50};
    
    Square_t square = {pA, pB, pC, pD, origin};
    squareA = square;
    theta_g = deg2rad(1);

    squareA = translate(squareA, origin.x, origin.y); 
    squareA = translate(squareA, rot.x, rot.y);       
    centerC = squareA.center;

}



 Vector_t matrixRotationTheta(Vector_t vector, Vector_t cen , float theta){

    double cosTheta = cos(theta);
    double sinTheta = sin(theta);


    vector.x = (cosTheta * (vector.x - cen.x)) + (-1 * sinTheta * (cen.y - vector.y) + cen.x);
    vector.y = cen.y - ((sinTheta * (vector.x - cen.x)) + (cosTheta * (cen.y - vector.y)));

    return vector;
}




Vector_t translateV(Vector_t v, float x, float y){
    v.x += x;
    v.y += y;

    return v;
}

void update() {
    int offset = sprintf(buffer, "Ticks: ");
    offset += sprintf(buffer + offset, "%d", count++);


    // get center, set center?? 
    Vector_t new = {0, 0};
    rot = matrixRotationTheta(rot, new, theta_g);
    



    Vector_t a, b, c, d;
    a = squareA.a;
    b = squareA.b;
    c = squareA.c;
    d = squareA.d;
    squareA.center.x = ((c.x - a.x) / 2) + a.x;
    squareA.center.y = ((c.y - a.y) / 2) + a.y;
    
    centerC = translateV(rot, origin.x, origin.y);

    squareA = translate(squareA, centerC.x - squareA.center.x, centerC.y - squareA.center.y);        
    
    // squareA.center = centerC;
    // squareA.center = matrixRotationTheta(squareA.center, origin, theta_g);
    
    int fc = 5;
    squareA.a = matrixRotationTheta(squareA.a, squareA.center, theta_g*fc);
    squareA.b = matrixRotationTheta(squareA.b, squareA.center, theta_g*fc);
    squareA.c = matrixRotationTheta(squareA.c, squareA.center, theta_g*fc);
    squareA.d = matrixRotationTheta(squareA.d, squareA.center, theta_g*fc);
    
    centerC = squareA.center;

}


int main(){
    init_vars();
    view_initWindow();
    printf("%s\n", programName);
    
    while(quitFlag){
        update();
        view_render();
        ctrl_processInput();        
        SDL_Delay(5);
    }
    view_quit();
    return 0;
}
