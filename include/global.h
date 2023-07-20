#ifndef GLOBAL_H
#define GLOBAL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <define.h>

extern uint32_t pixels[WINDOW_WIDTH/2];
extern double input[NUMINPUTS];
extern int input_matrix[28][28];

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *texture;
extern SDL_Rect Rect;
extern TTF_Font *tektur;
extern SDL_Color white;
extern SDL_Texture *commands_texture;
extern SDL_Rect commands_rect;
extern char text[58];

extern double training_inputs[NUMTRAININGSETS][NUMINPUTS];
extern double testing_inputs[NUMTESTINGSETS][NUMINPUTS];

extern double hidden_layer[NUMHIDDEN];
extern double output_layer[NUMOUTPUTS];

extern int training_outputs[NUMTRAININGSETS];
extern int testing_outputs[NUMTESTINGSETS];

extern double hidden_layer_bias[NUMHIDDEN];
extern double output_layer_bias[NUMOUTPUTS];

extern double hidden_layer_weights[NUMHIDDEN][NUMINPUTS];
extern double output_layer_weights[NUMOUTPUTS][NUMHIDDEN];

extern double LR;
extern int NUMEPOCHS;

#endif
