#include <stdlib.h>
#include <iostream>
#include <string>

#include "fishspear.h"

int main() {
    Fishspear<int> fs;
    while (true) {
        std::cout << fs << "\n";
        std::string op;
        std::cin >> op;
        if (op[0] == 'p') {
            int x = fs.top();
            fs.pop();
            std::cout << "Popped " << x << "\n";
        } else {
            int x = atoi(op.c_str());
            fs.push(x);
        }
    }
}
