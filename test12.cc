#include "vm_app.h"
#include <iostream>

int main() {
    char *p1 = (char *) vm_extend(); // resident
    char *p2 = (char *) vm_extend(); // valid but nonresident

    p1[VM_PAGESIZE - 1] = 'X';
    p2[0] = 'Y';

    vm_syslog(p1 + VM_PAGESIZE - 1, 2);
}
