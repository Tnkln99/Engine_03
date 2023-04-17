#include "Engine.h"

#include <cstdlib>
#include <iostream>

int main() {
    zt::Engine app{};

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}