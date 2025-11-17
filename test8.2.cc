#include <iostream>
#include "vm_app.h"
#include <queue>
using namespace std;
//phys mem only has 2 pages, this should catch when syslog doesn't check residency repeatedly
int main() {
    char *a;
    char *b;
    char *c;
    a = (char *) vm_extend();
    b = (char*) vm_extend();
    c = (char*) vm_extend();
    for(int i=0; i<VM_PAGESIZE;i++){
      a[i] = 'i';
      b[i]= 'i';
      c[i] = 'i';}
    vm_syslog(a, 2*VM_PAGESIZE+5);

}
