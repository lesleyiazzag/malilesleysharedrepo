#include <iostream>
#include "vm_app.h"

using namespace std;

int main() {
    char *a;
    char *b;
    char *c;
    char j;
    a = (char *) vm_extend();
    b = (char*) vm_extend();
    c = (char*) vm_extend();
    char k;
    char l;
    k =  b[1];
    l = c[1];
    a[1] = 'h';
    b[1] = 'e';
    c[1] = 'l';
    a[2]= 'l';
    j = a[1];
    a[2]='h';

   

}
