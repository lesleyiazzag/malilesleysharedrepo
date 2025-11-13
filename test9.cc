// trigger disk exhaustion and ensure pager returns NULL
#include "vm_app.h"
#include <iostream>

int main() {
    for (int i = 0; i < 10000; i++) {
        if (vm_extend() == nullptr) {
            break; // stop once no more disk space
        }
    }
}
