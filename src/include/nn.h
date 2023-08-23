#ifndef NN_H
#define NN_H

double sigmoid(double);
double dSigmoid(double);
void read_input();
double init_value();
int max_output(double *);
void ForwardPropagation(double *);
void BackPropagation(double *,int);
void init();
void save_training();
void load();
void test();
void train();

#endif