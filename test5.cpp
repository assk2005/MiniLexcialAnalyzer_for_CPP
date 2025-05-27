#include <stdexcept>
int main() {
    try {
        int* ptr = new int(10);
        if (*ptr > 5)
            throw std::runtime_error("Value too large");
        delete ptr;
    } catch (std::exception& e) {}
    return 0;
}