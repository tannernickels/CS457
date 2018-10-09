#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main(int argc, char** argv){
    int opt, a, b, c, d;
    while((opt = getopt(argc, argv, "abc:d:*")) != EOF){
        switch(opt){
            case 'a': a = 1; cout << "a is enabled " << a << endl; break;
            case 'b': b = 1; cout << "b is enabled " << b << endl; break;
            case 'c': c = 1; cout << "value of c is " << optarg << endl; break;
            case '*': d = 1; cout << "hidden option found " << optarg << endl; break; 
            case '?': fprintf(stderr, "usage is \n -a : for enabling a \n -b : for enabling b \n -c : <value>\n"); break;
            default: cout<<endl; abort();
        }
    }
    return 0;
    
}
