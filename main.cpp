#include "CLI.h"
#include <iostream>

int main(const int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <terminal height>" << std::endl;
        return 1;
    }

    char* endPtr;
    const long value = std::strtol(argv[1], &endPtr, 10);
    if (*endPtr != '\0' || value <= 0) {
        std::cerr << "Terminal height must be a positive integer." << std::endl;
        return 1;
    }
    int boardSize = static_cast<int>(value);
    CLI c(boardSize);
    c.run();
    return 0;
}