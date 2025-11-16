#include "vm_app.h"
#include <iostream>
#include <cstring>

using namespace std;

int main() {
    char *p0 = (char *) vm_extend();
    char *p1 = (char *) vm_extend();
    char *p2 = (char *) vm_extend();

    // write to pages
    p0[0] = 'h';
    p1[0] = 'a';
    p2[0] = 'y';

    // first access
    vm_syslog(p0, 1);
    vm_syslog(p1, 1);
    vm_syslog(p2, 1);

    vm_syslog(p1, 1);
    vm_syslog(p2, 1);
    vm_syslog(p0, 1);
    vm_syslog(p2, 1);

    p0[0] = '1';
    p1[0] = '2';

    vm_syslog(p0, 1);
    vm_syslog(p1, 1);
    vm_syslog(p2, 1);

    p2[0] = 'i';

    vm_syslog(p0,1);
    vm_syslog(p1,1);
    vm_syslog(p2,1);

    p1[0] = 'g';
    p2[0] = 'd';
    vm_syslog(p1,1);
    vm_syslog(p0,1);
    vm_syslog(p1, 1);

    p2[0] = 'b';
    vm_syslog(p0, 1);
    vm_syslog(p1, 1);
    vm_syslog(p2, 1);
}
