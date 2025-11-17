#include <iostream>
#include "vm_app.h"

using namespace std;
//end of syslog should be out of range 
int main() {
    char *a;
    char *b;
    char *c;
    a = (char *) vm_extend();
    b = (char*) vm_extend();
    c = (char*) vm_extend();
    a[1] = 'h';
    b[1] = 'e';
    c[1] = 'l';
    a[2]='h';
    vm_syslog(a,VM_PAGESIZE*3+5);
   

}
