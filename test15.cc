#include "vm_app.h"
#include <iostream>

int main() {
    char *p = (char*)vm_extend();
    p[0] = 'F'; // dirty

    for (int i = 0; i < 50; i++) vm_extend(); // force eviction
 // force a read fault
    (void)p[0];
 // attempt write
    p[1] = 'Z';

    vm_syslog(p, 2);
}
