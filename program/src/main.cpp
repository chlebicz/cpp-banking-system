#include <iostream>
#include "UserInterface.h"

int main() {
    UserInterface userInterface;

    try {
        userInterface.entryLoop();
    } catch(const Bankruptcy& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}