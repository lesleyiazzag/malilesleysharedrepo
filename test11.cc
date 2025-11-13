#include "vm_app.h"
#include <iostream>
// check vm_destroy frees resources correctly
int main() {
    for (int i = 0; i < 5; i++) {
        char *p = (char *) vm_extend();
        p[0] = 'Z';
        vm_syslog(p, 1);
    }
}
