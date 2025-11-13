// test vm_extend correctness and simple syslog
#include "vm_app.h"
#include <iostream>

int main() {
    char *p = (char *) vm_extend();
    p[0] = 'H';
    p[1] = 'i';
    vm_syslog(p, 2); // expect "Hi"
}
