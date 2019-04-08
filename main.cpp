//
// Created by demon1999 on 08.04.19.
//

#include <string>
#include <iostream>
#include <a.out.h>
#include <limits>
#include <sys/mman.h>
#include <cstring>

const int len = 4096;
const int code_len = 228;

/* function in code
int pow1(int k) {
    const int mod = 100000007;
    const int a = 999999999;
    int p = a;
    int ans = 1;
    while (k) {
        if (k % 2) {
            ans = (1LL * ans * p) % mod;
        }
        p = (1LL * p * p) % mod;
        k /= 2;
    }
    return ans;
}
 */

unsigned char code[] = {0x55, 0x48, 0x89, 0xe5, 0x89, 0x7d, 0xec, 0xc7, 0x45, 0xf8, 0x07, 0xe1, 0xf5, 0x05, 0xc7, 0x45, 0xfc, /*first change place*/0xff, 0xe0, 0xf5, 0x05, 0xc7, 0x45, 0xf0, /*second change place*/0xff, 0xe0, 0xf5, 0x05, 0xc7, 0x45, 0xf4, 0x01, 0x00, 0x00, 0x00, 0x83, 0x7d, 0xec, 0x00, 0x0f, 0x84, 0xb2, 0x00, 0x00, 0x00, 0x8b, 0x45, 0xec, 0x83, 0xe0, 0x01, 0x85, 0xc0, 0x74, 0x4a, 0x8b, 0x45, 0xf4, 0x48, 0x63, 0xd0, 0x8b, 0x45, 0xf0, 0x48, 0x98, 0x48, 0x89, 0xd1, 0x48, 0x0f, 0xaf, 0xc8, 0x48, 0xba, 0x05, 0x95, 0x80, 0xc1, 0x47, 0x76, 0xcc, 0xab, 0x48, 0x89, 0xc8, 0x48, 0xf7, 0xea, 0x48, 0x8d, 0x04, 0x0a, 0x48, 0xc1, 0xf8, 0x1a, 0x48, 0x89, 0xc2, 0x48, 0x89, 0xc8, 0x48, 0xc1, 0xf8, 0x3f, 0x48, 0x29, 0xc2, 0x48, 0x89, 0xd0, 0x48, 0x69, 0xc0, 0x07, 0xe1, 0xf5, 0x05, 0x48, 0x29, 0xc1, 0x48, 0x89, 0xc8, 0x89, 0x45, 0xf4, 0x8b, 0x45, 0xf0, 0x48, 0x63, 0xd0, 0x8b, 0x45, 0xf0, 0x48, 0x98, 0x48, 0x89, 0xd1, 0x48, 0x0f, 0xaf, 0xc8, 0x48, 0xba, 0x05, 0x95, 0x80, 0xc1, 0x47, 0x76, 0xcc, 0xab, 0x48, 0x89, 0xc8, 0x48, 0xf7, 0xea, 0x48, 0x8d, 0x04, 0x0a, 0x48, 0xc1, 0xf8, 0x1a, 0x48, 0x89, 0xc2, 0x48, 0x89, 0xc8, 0x48, 0xc1, 0xf8, 0x3f, 0x48, 0x29, 0xc2, 0x48, 0x89, 0xd0, 0x48, 0x69, 0xc0, 0x07, 0xe1, 0xf5, 0x05, 0x48, 0x29, 0xc1, 0x48, 0x89, 0xc8, 0x89, 0x45, 0xf0, 0x8b, 0x45, 0xec, 0x89, 0xc2, 0xc1, 0xea, 0x1f, 0x01, 0xd0, 0xd1, 0xf8, 0x89, 0x45, 0xec, 0xe9, 0x44, 0xff, 0xff, 0xff, 0x8b, 0x45, 0xf4, 0x5d, 0xc3};

void *func_ptr = nullptr;

void execute(int k) {
    if (mprotect(func_ptr, len, PROT_READ | PROT_EXEC) == -1) {
        std::cerr << "Can't change priviliges: " << strerror(errno) << "\n";
        return;
    }
    std::cout << ((int(*)(int))func_ptr)(k) << "\n";
}

void write_func() {
    errno = 0;
    func_ptr = mmap(nullptr, len, PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (errno != 0) {
        std::cerr << "Can't allocate memory: " << strerror(errno) << "\n";
        exit(0);
    }
    std::memcpy(func_ptr, code, code_len);
}

void delete_func() {
    int status = munmap(func_ptr, len);
    if (status == -1) {
        std::cerr << "Can't free memory: " << strerror(errno) << "\n";
    }
}

void print_info() {
    std::cout << "Info:\n" <<
                 "Function has one argument n.\n" <<
                 "It calculates a^n % mod, where mod is a constant integer equal to 10^9 + 7 and a equals to 999999999 by default.\n" <<
                 "If you want to execute function with argument n (n must be positive integers no more than 2 * 10^9),\ntype \"exec n\".\n" <<
                 "If you want to change a with value p (p must be positive integer no more than 2 * 10^9), type \"change p\".\n" <<
                 "Type \"exit\" if you want to exit\n";
}

void change_a(int a) {
    if (mprotect(func_ptr, len, PROT_READ | PROT_WRITE) == -1) {
        std::cerr << "Can't change priviliges: " << strerror(errno) << "\n";
        return;
    }
    for (int j = 0; j < 4; j++) {
        code[j + 17] = static_cast<unsigned char>(a % 256);
        code[j + 24] = static_cast<unsigned char>(a % 256);
        a /= 256;
    }
    std::memcpy(func_ptr, code, code_len);
};

const int max_val = int(2e9);

int get_int() {
    int x = 0;
    std::string s;
    std::cin >> s;

    if (s.length() == 0 || s[0] > '9' || s[0] < '0')
        return -1;
    for (char j : s) {
        if (j < '0' || j > '9') {
            return -1;
        }
        if (x > max_val / 10) {
            return -1;
        }
        x *= 10;
        if (x > max_val - (j - '0')) {
            return -1;
        }
        x += j - '0';
    }
    return x;
}


int main() {
    write_func();
    print_info();
    std::string s;
    int x, y;
    while (true) {
        std::cin >> s;
        if (s == "exit") {
            break;
        } else
        if (s == "change") {
            x = get_int();
            if (x == -1) {
                std::cout << "Illegal command: illegal value of p\n";
                continue;
            }
            change_a(x);
        } else
        if (s == "exec") {
            x = get_int();
            if (x == -1) {
                std::cerr << "Illegal command: illegal value of n\n";
                continue;
            }
            execute(x);
        } else {
            std::cerr << "Illegal command: wrong name\n";
        }
    }
    delete_func();
}