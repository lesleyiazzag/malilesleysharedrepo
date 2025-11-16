#include "vm_app.h"
#include <iostream>

using namespace std;

int main()
{
    char* p[7];

    // extend calls
    for (int i = 0; i < 7; ++i) {
        p[i] = (char*) vm_extend();
    }

    // dirty the pages
    p[0][VM_PAGESIZE - 3] = 'a';
    p[0][VM_PAGESIZE - 2] = 'b';
    p[0][VM_PAGESIZE - 1] = 'c';
    p[2][0] = 'C';
    p[2][1] = 'S';
    p[3][0] = 'C';
    p[1][0] = 'I';
    p[0][0] = '4';
    p[4][0] = '3';
    p[5][0] = '2';
    p[6][0] = 'O';
    p[3][1] = 'S';
    p[6][1] = '!';


    vm_syslog(p[0] + VM_PAGESIZE / 2, 4);
    p[4][1] = 'h';

    return 0;
}
