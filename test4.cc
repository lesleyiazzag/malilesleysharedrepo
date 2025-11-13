// extend multiple pages, ensure they are contiguous and valid
#include "vm_app.h"
#include <iostream>

int main() {
    char *p1 = (char *) vm_extend();
    char *p2 = (char *) vm_extend();
    p1[0] = 'A';
    p2[0] = 'B';
    vm_syslog(p1, 1);
    vm_syslog(p2, 1);
}
