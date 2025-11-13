// trigger read fault on uninitialized page
#include "vm_app.h"
#include <iostream>

int main() {
    char *p = (char *) vm_extend();
    char x = p[0]; // read should succeed (zero-filled)
    (void)x;
    vm_syslog(p, 1);
}
