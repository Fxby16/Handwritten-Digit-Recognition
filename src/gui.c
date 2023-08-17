#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <global.h>
#include <nn.h>
#include <define.h>

void clear(){
    memset(pixels,0xFFFFFFFF,400*sizeof(pixels[0])); //clear the texture
    for(int i=0;i<28;i++) //clear the matrix that contains the input with padding
        for(int j=0;j<28;j++)
            input_matrix[i][j]=1;
}

void make_array(){
    for(int i=0;i<28;i++)   //convert the matrix into an array that will be used as input for the nn
        for(int j=0;j<28;j++){
            if(input_matrix[i][j]==0)
                input[(i*28)+j]=0.0;
            else
                input[(i*28)+j]=1.0;
        }
}

void get_text(const char *text){
SDL_Surface *commands_surface=NULL;
    commands_surface=TTF_RenderUTF8_Blended_Wrapped(tektur,text,white,0); //render text on a temp surface
    commands_texture=SDL_CreateTextureFromSurface(renderer,commands_surface); //create a texture from the temp surface
    commands_rect.w=commands_surface->w; //set where the text should be displayed
    commands_rect.h=commands_surface->h;
    commands_rect.y=(WINDOW_HEIGHT-commands_rect.h)/2;
    commands_rect.x=(WINDOW_WIDTH/2)+((WINDOW_WIDTH/2)-commands_rect.w)/2;
    SDL_FreeSurface(commands_surface); 
}

void init_sdl(){
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    window=SDL_CreateWindow("Handwritten Digit Recognition",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WINDOW_WIDTH,WINDOW_HEIGHT,SDL_WINDOW_SHOWN);
    renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    texture=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,DRAW_WIDTH,DRAW_HEIGHT);
    Rect.w=400; //create a rect that covers half of the window (the rect where you can draw)
    Rect.h=400;
    Rect.x=0;
    Rect.y=0;

    tektur=TTF_OpenFont("fonts/Tektur-Regular.ttf",22); 

    get_text(text); //initialize the texture for the text
}

void test_with_custom_draw(){
bool quit=false;
bool leftMouseButtonDown=false;
bool draw=true;
SDL_Event event;
    init_sdl();
    clear();

    while(!quit){
        SDL_WaitEvent(&event);
 
        switch(event.type){
            case SDL_QUIT:
                quit=true;
                break;
            case SDL_MOUSEBUTTONUP:
                if(event.button.button==SDL_BUTTON_LEFT)
                    leftMouseButtonDown=false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button==SDL_BUTTON_LEFT)
                    leftMouseButtonDown=true;
            case SDL_MOUSEMOTION:
                if(leftMouseButtonDown){
                    int mouseX=event.motion.x/20; //since the part of the screen where you can draw is 20 times bigger, i have to divide the x and y coordinates by 20
                    int mouseY=event.motion.y/20;
                    if((mouseX>=0 && mouseX<20) && (mouseY>=0 && mouseY<20)){ //if you click in the part where you can draw
                        if(draw){ //if you are in draw mode
                            pixels[(mouseY*20)+mouseX]=0x00000000; //color this pixel black
                            input_matrix[mouseY+4][mouseX+4]=0; //color this pixel black also in the matrix that will be the input
                        }
                        else{ //if you are in eraser mode
                            pixels[(mouseY*20)+mouseX]=0xFFFFFFFF; //color this pixel white
                            input_matrix[mouseY+4][mouseX+4]=1; //color this pixel white also in the matrix that will be the input
                        }
                    }
                }
                break;
            case SDL_KEYDOWN:
                SDL_Keycode key=event.key.keysym.sym;
                if(key==SDLK_q)
                    quit=true;
                if(key==SDLK_0){
                    clear();
                    text[sizeof(text)/sizeof(text[0])-2]=' '; //since there is no output, the text has to be cleared
                    get_text(text);
                }
                if(key==SDLK_1){
                    make_array(); //convert the matrix into an array
                    ForwardPropagation(input); 
                    text[sizeof(text)/sizeof(text[0])-2]=max_output(output_layer)+'0'; //update the text with the output
                    get_text(text);
                }
                if(key==SDLK_2)
                    draw=true;

                if(key==SDLK_3)
                    draw=false;
                break;
        }
        SDL_UpdateTexture(texture,NULL,pixels,20*sizeof(pixels[0])); //update the texture for the drawing
        SDL_RenderClear(renderer); //clear the screen
        SDL_RenderCopy(renderer,texture,NULL,&Rect); //render the texture with the drawing
        SDL_RenderCopy(renderer,commands_texture,NULL,&commands_rect); //render the texture with the text
        SDL_RenderPresent(renderer); //show the new frame
    }
 
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(commands_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}