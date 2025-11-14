#include "vm_app.h"
#include <iostream>

int main() {
    char *p = (char*) vm_extend();
    p[0] = 'O';

    (void)p[0]; // mark referenced

    for(int i = 0; i < 100; i++) vm_extend();

    vm_syslog(p, 1);
}
