#include <iostream>
#include <utility>

void process(int& x) {
    std::cout << "Lvalue processed: " << x << std::endl;
}

void process(int&& x) {
    std::cout << "Rvalue processed: " << x << std::endl;
}

template <typename T>
void forwarder(T&& x) {
    process(std::forward<T>(x)); // 完美转发到 process
}

int main() {
    int a = 5;
    forwarder(a);        // 传递左值
    forwarder(10);       // 传递右值
}