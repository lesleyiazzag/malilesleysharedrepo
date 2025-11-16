#include "vm_app.h"
#include <iostream>

int main() {
    char *dirty = (char*) vm_extend();
    dirty[0] = 'D';

    char *clean = (char*) vm_extend();
    // force a read
    (void)clean[0];

    for(int i = 0; i < 100; i++) vm_extend();  // eviction

    vm_syslog(clean, 1);
}
