#include <iostream>
#include "vm_app.h"
using namespace std;
int main() {
    char *a;
    char *b;
    char *c;
    a = (char *) vm_extend();
    b = (char*) vm_extend();
    c = (char*) vm_extend();
    a[0] = 'h';
    b[0] = 'i';
    c[0] = 't';
    vm_syslog(a,1);
    vm_syslog(b,1);
    vm_syslog(c,1);

    c[1] = 'h';
    vm_syslog(a,1);

    vm_syslog(b,1);
    vm_syslog(c,1);


}
