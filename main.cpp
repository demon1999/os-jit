//
// Created by demon1999 on 08.04.19.
//

#include <string>
#include <iostream>
#include <a.out.h>
#include <limits>
#include <sys/mman.h>
#include <cstring>

const int len = 4;
unsigned char code[] = {0x8d, 0x47, 0x01, 0xc3};
void *func_ptr = nullptr;

void execute(int k) {
    mprotect(func_ptr, 4, PROT_READ | PROT_EXEC);
    if (k == std::numeric_limits<int>::max()) {
        std::cerr << "Too big number!";
        return;
    }
    std::cout << ((int(*)(int))func_ptr)(k) << "\n";
    mprotect(func_ptr, 4, PROT_READ | PROT_WRITE);
}

void write_func() {
    func_ptr = mmap(nullptr, len, PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    std::memcpy(func_ptr, code, len);
}

void delete_func() {
    int status = munmap(func_ptr, len);
    if (status == -1) {
        std::cerr << "Can't free memory " << strerror(errno) << "\n";
    }
}

int main() {
    write_func();
    int x;
    while (std::cin >> x) {
        execute(x);
    }
    delete_func();
}