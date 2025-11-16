// syslog a message crossing page boundary
#include "vm_app.h"
#include <iostream>

int main() {
    char *p1 = (char *) vm_extend();
    char *p2 = (char *) vm_extend();

    for (int i = 0; i < VM_PAGESIZE - 2; i++) p1[i] = 'A';
    p2[0] = 'B';
    p2[1] = 'C';
    // spans pages 1 and 2
    vm_syslog(p1 + VM_PAGESIZE - 2, 3); 
}
