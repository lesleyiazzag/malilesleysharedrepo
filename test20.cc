#include "vm_app.h"
#include <iostream>

int main() {
    char *p = (char*) vm_extend();
    p[0] = 'T';

    (void)p[0]; // read fault
    p[1] = 'Z';

    vm_syslog(p, 2);
}
