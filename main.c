#include <stdbool.h>
#include <utilities.h>

int main(int argc,char *argv[]){
bool quit=false;
    while(!quit)
        menu(&quit);

    return 0;
}
