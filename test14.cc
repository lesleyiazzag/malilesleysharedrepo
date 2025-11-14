#include "vm_app.h"
#include <iostream>

int main() {
    char *a = (char*) vm_extend();
    char *b = (char*) vm_extend();

    a[0] = 'A';
    b[0] = 'B';

    // cause eviction by creating many pages
    for (int i = 0; i < 100; i++) vm_extend();

    // access p1 and p2; whichever was evicted will lose content
    vm_syslog(a, 1);
    vm_syslog(b, 1);
}
