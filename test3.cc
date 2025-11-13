// verify that newly extended pages are zero-initialized
#include "vm_app.h"
#include <iostream>

int main() {
    char *p = (char *) vm_extend();
    vm_syslog(p, 4); // should print 4 null chars 
}
