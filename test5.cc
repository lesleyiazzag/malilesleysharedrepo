// trigger error in syslog by spanning invalid memory
#include "vm_app.h"
#include <iostream>

int main() {
    char *p = (char *) vm_extend();
    p[0] = 'O';
    p[1] = 'K';
    // try to log across valid and invalid boundary
    vm_syslog(p, VM_PAGESIZE + 10);
}
