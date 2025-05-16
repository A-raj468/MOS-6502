#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <rom_file>\n";
        return 1;
    }

    std::string rom_path = argv[1];

    std::cout << "Rom path: " << rom_path << "\n";
    return 9;
}
