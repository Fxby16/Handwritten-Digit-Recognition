#include <stdio.h>
#include <stdbool.h>
#include <nn.h>
#include <gui.h>

void menu(bool *quit){
    printf("q: quit\n1: train the neural network\n2: test the neural network on MNIST dataset\n3: test the neural neural network with your digits\n");
    char choice=getchar();
    switch(choice){
        case 'q': *quit=true;
            break;
        case '1': 
            read_input(); //read MNIST dataset
            init(); //init weights and biases
            train(); //train the neural network
            break;
        case '2':
            read_input(); //read MNIST dataset
            load(); //load weights and biases from previous training
            test(); //test the accuracy on MNIST dataset
            break;
        case '3':
            load(); //load weights and biases from previous training
            test_with_custom_draw(); //draw your own digits to be recognized
            break;
        default:
            break;
    }
    while(getchar()!='\n'); //clear stdin
}