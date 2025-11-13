// trigger write fault on read-only page if pager sets protections
#include "vm_app.h"
#include <iostream>

int main() {
    char *p = (char *) vm_extend();
    p[0] = 'X';
    vm_yield();       // allow pager to change protection then trigger write fault again
    p[0] = 'Y';      
    vm_syslog(p, 1);
}
