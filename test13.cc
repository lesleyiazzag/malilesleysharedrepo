#include "vm_app.h"
#include <iostream>

int main() {
    char *p = (char *) vm_extend();
    p[0] = 'C'; // dirty

    for (int i = 0; i < 100; i++)
        vm_extend(); // force eviction of p

    vm_syslog(p, 1); 
}
