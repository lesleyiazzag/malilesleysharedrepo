#include "vm_app.h"
#include <iostream>
using namespace std;

int main() {
    char* p0 = (char*) vm_extend();

    p0[0] = '1';
    p0[1] = '2';
    p0[2] = '3';

    vm_syslog(p0, 3);
    char* invalid_addr_before = p0 - 1;
    char* invalid_addr_after = p0 + VM_PAGESIZE;

    invalid_addr_before[0] = '4';  
    invalid_addr_after[0] = '5';
    p0[0] = '6';
    vm_syslog(p0, 1);
    return 0;
}
