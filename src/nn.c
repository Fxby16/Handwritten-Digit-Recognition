#include <global.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <define.h>

double sigmoid(double x){ 
    return 1/(1+exp(-x));  //activation function
}
double dSigmoid(double x){ 
    return sigmoid(x)*(1-sigmoid(x)); //derivative of the activation function
}

void read_input(){
    FILE *fp=fopen("MNIST_Dataset/mnist_train.csv","r");
    FILE *fptest=fopen("MNIST_Dataset/mnist_test.csv","r");

    printf("READING THE INPUT\n");

char buffer[5000];
    int row=0;
    while(fgets(buffer,5000,fp)){
        int col=0;
        char *num=strtok(buffer,",");
        int out=atoi(num);
        outputs[row]=out;
        for(int i=0;i<NUMOUTPUTS;i++){
            if(i==out)
                training_outputs[row][i]=1;
            else 
                training_outputs[row][i]=0;
        }
        while(num){
            num=strtok(NULL,",");
            if(num!=NULL)
                training_inputs[row][col++]=(double)(255-atoi(num))/255.0; //i subtract num to 255 because i want black number on white background
        }
        row++;
    }
    fclose(fp);

    row=0;
    while(fgets(buffer,5000,fptest)){
        int col=0;
        char *num=strtok(buffer,",");
        testing_outputs[row]=atoi(num);
        while(num){
            num=strtok(NULL,",");
            if(num!=NULL)
                testing_inputs[row][col++]=(double)(255-atoi(num))/255.0; //i subtract num to 255 because i want black number on white background
        }
        row++;
    }
    fclose(fptest);

    printf("READING ENDED\n");
}

double init_value(){
    return ((double)rand()/(double)RAND_MAX)-0.5; //generating values between -0.5 and +0.5
}

void init(){
    printf("INITIALIZING VALUES\n");
    
    for(int i=0;i<NUMHIDDEN;i++){ //initialize weights and biases for the hidden layer
        hidden_layer_bias[i]=init_value();
        for(int j=0;j<NUMINPUTS;j++)
            hidden_layer_weights[i][j]=init_value();
    }

    for(int i=0;i<NUMOUTPUTS;i++){ //initialize weights and biases for the output layer
        output_layer_bias[i]=init_value();
        for(int j=0;j<NUMHIDDEN;j++)
            output_layer_weights[i][j]=init_value();
    }
    printf("INITIALIZATION ENDED\n");
}   

int max_output(double *output){ //get the most probable output
int maxindex=0;
    for(int i=0;i<NUMOUTPUTS;i++)
        if(output[i]>output[maxindex])
            maxindex=i;
    return maxindex;
}

void ForwardPropagation(double *input){
    for(int i=0;i<NUMHIDDEN;i++)
        hidden_layer[i]=hidden_layer_bias[i];
    
    for(int i=0;i<NUMHIDDEN;i++)
        for(int j=0;j<NUMINPUTS;j++)
            hidden_layer[i]+=input[j]*hidden_layer_weights[i][j];

    for(int i=0;i<NUMOUTPUTS;i++)
        output_layer[i]=output_layer_bias[i];

    for(int i=0;i<NUMOUTPUTS;i++)
        for(int j=0;j<NUMHIDDEN;j++)
            output_layer[i]+=sigmoid(hidden_layer[j])*output_layer_weights[i][j];
}

void BackPropagation(double *input,double *output){
double deltaHidden[NUMHIDDEN];
double deltaOutput[NUMOUTPUTS];
    
    for(int i=0;i<NUMOUTPUTS;i++)
        deltaOutput[i]=LR*(sigmoid(output_layer[i])-output[i])*dSigmoid(output_layer[i]);

    for(int i=0;i<NUMHIDDEN;i++)
        deltaHidden[i]=0;

    for(int i=0;i<NUMOUTPUTS;i++)
        for(int j=0;j<NUMHIDDEN;j++)
            deltaHidden[j]+=deltaOutput[i]*output_layer_weights[i][j];

    for(int i=0;i<NUMHIDDEN;i++)
        deltaHidden[i]*=dSigmoid(hidden_layer[i]);

    //update values
    for(int i=0;i<NUMOUTPUTS;i++)
        output_layer_bias[i]-=deltaOutput[i];

    for(int i=0;i<NUMOUTPUTS;i++)
        for(int j=0;j<NUMHIDDEN;j++)
            output_layer_weights[i][j]-=deltaOutput[i]*(sigmoid(hidden_layer[j]));       

    for(int i=0;i<NUMHIDDEN;i++)
        hidden_layer_bias[i]-=deltaHidden[i];

    for(int i=0;i<NUMHIDDEN;i++)
        for(int j=0;j<NUMINPUTS;j++)
            hidden_layer_weights[i][j]-=deltaHidden[i]*input[j];      

}

void save_training(){ //print final weights and biases after training
FILE *fpreport=fopen("conf2.txt","w");
    for(int i=0;i<NUMHIDDEN;i++)
        for(int j=0;j<NUMINPUTS;j++)
            fprintf(fpreport,"%lf ",hidden_layer_weights[i][j]);
    putchar('\n');
    
    for(int i=0;i<NUMHIDDEN;i++)
        fprintf(fpreport,"%lf ",hidden_layer_bias[i]);
    putchar('\n');

    for(int i=0;i<NUMOUTPUTS;i++)
        for(int j=0;j<NUMHIDDEN;j++)
            fprintf(fpreport,"%lf ",output_layer_weights[i][j]);
    putchar('\n');

    for(int i=0;i<NUMOUTPUTS;i++)
        fprintf(fpreport,"%lf ",output_layer_bias[i]);

    fclose(fpreport);
}

void load(){ //load previously saved weights and biases
FILE *load=fopen("conf.txt","r");

    printf("LOADING DATA\n");
    for(int i=0;i<NUMHIDDEN;i++)
        for(int j=0;j<NUMINPUTS;j++)
            fscanf(load,"%lf ",&hidden_layer_weights[i][j]);
    
    for(int i=0;i<NUMHIDDEN;i++)
        fscanf(load,"%lf ",&hidden_layer_bias[i]);

    for(int i=0;i<NUMOUTPUTS;i++)
        for(int j=0;j<NUMHIDDEN;j++)
            fscanf(load,"%lf ",&output_layer_weights[i][j]);

    for (int i=0;i<NUMOUTPUTS;i++)
        fscanf(load,"%lf ",&output_layer_bias[i]);
        
    fclose(load);
    printf("LOADING ENDED\n");
}

void test(){ //test the nn on MNIST dataset
int corrects=0;
    for(int k=0;k<NUMTRAININGSETS;k++){
        ForwardPropagation(training_inputs[k]);
        if(outputs[k]==max_output(output_layer))
            corrects++;
    }
    printf("ACCURACY: %.6f\tCORRECT ANSWERS: %d\n",(double)corrects*100/NUMTRAININGSETS,corrects);
    corrects=0;
    for(int k=0;k<NUMTESTINGSETS;k++){
        ForwardPropagation(testing_inputs[k]);
        if(testing_outputs[k]==max_output(output_layer))
            corrects++;
    }
    printf("ACCURACY: %.6f\tCORRECT ANSWERS: %d\n\n",(double)corrects*100/NUMTESTINGSETS,corrects);
}

void train(){ //train the nn on MNIST dataset
    while(NUMEPOCHS<=0){
        printf("Number of epochs (suggested between 30 and 60): ");
        scanf("%d",&NUMEPOCHS);
    }
    while(LR<=0){
        printf("Learning rate (suggested 0.01): ");
        scanf("%lf",&LR);
    }

    for(int epoch=0;epoch<NUMEPOCHS;epoch++){
        for(int k=0;k<NUMTRAININGSETS;k++){
            ForwardPropagation(training_inputs[k]);
            BackPropagation(training_inputs[k],training_outputs[k]);
        }
        
        int corrects=0;
        for(int k=0;k<NUMTESTINGSETS;k++){
            ForwardPropagation(testing_inputs[k]);
            if(testing_outputs[k]==max_output(output_layer))
                corrects++;
        }
        printf("EPOCH: %d\tACCURACY: %.6f\tCORRECT ANSWERS: %d\n",epoch,(double)corrects*100/NUMTESTINGSETS,corrects);
    }
    save_training(); //save weights and biases from this training
}
