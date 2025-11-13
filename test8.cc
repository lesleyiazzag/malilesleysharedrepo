// force eviction with small memory
#include "vm_app.h"
#include <iostream>

int main() {
    char *a = (char *) vm_extend();
    char *b = (char *) vm_extend();
    // should cause eviction when memory = 2
    char *c = (char *) vm_extend(); 
    a[0] = 'A';
    b[0] = 'B';
    c[0] = 'C';
    vm_syslog(a, 1);
    vm_syslog(b, 1);
    vm_syslog(c, 1);
}
