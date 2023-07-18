#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <define.h>

uint32_t pixels[WINDOW_WIDTH/2];
double input[NUMINPUTS];
int input_matrix[28][28];

SDL_Window *window=NULL;
SDL_Renderer *renderer=NULL;
SDL_Texture *texture=NULL;
SDL_Rect Rect;
TTF_Font *tektur=NULL;
SDL_Color white={255,255,255};
SDL_Texture *commands_texture=NULL;
SDL_Rect commands_rect;
char text[]="q: Quit\n0: Clear\n1: Submit\n2: Pencil\n3: Eraser\n\nOutput:  ";


double training_inputs[NUMTRAININGSETS][NUMINPUTS];
double testing_inputs[NUMTESTINGSETS][NUMINPUTS];

double hidden_layer[NUMHIDDEN];
double output_layer[NUMOUTPUTS];

double training_outputs[NUMTRAININGSETS][NUMOUTPUTS];
int testing_outputs[NUMTESTINGSETS];

double hidden_layer_bias[NUMHIDDEN];
double output_layer_bias[NUMOUTPUTS];

double hidden_layer_weights[NUMHIDDEN][NUMINPUTS];
double output_layer_weights[NUMOUTPUTS][NUMHIDDEN];

int outputs[NUMTRAININGSETS];

double LR=0;
int NUMEPOCHS=0;
