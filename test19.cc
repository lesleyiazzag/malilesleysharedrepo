#include "vm_app.h"
#include <iostream>

int main() {
    // get a valid address
    char *valid = (char*) vm_extend();
    // make invalid address by going below valid
    char *bad = valid - VM_PAGESIZE;
    // should fail
    vm_syslog(bad, 1);
}
