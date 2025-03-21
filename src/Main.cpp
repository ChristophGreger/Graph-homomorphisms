//
// Created by Christoph Greger on 24.12.24.
//

#include <iostream>
#include <string>

struct Flags {
    bool cfi = false;
    bool time = false;
};

Flags processFlags(int argc, char* argv[], int start) {
    bool cfi = false;
    bool time = false;

    // Process optional flags -cfi, -time and -sf <spasm_file>
    for (int i = start; i < argc; i++) {
        std::string option = argv[i];
        if (option == "--cfi") {
            cfi = true;
        } else if (option == "--time") {
            time = true;
        } else {
            std::cerr << "Unknown flag: " << option << std::endl;
            exit(1);
        }
    }
    const Flags flags = {cfi,time};
    return flags;
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "Usage: ./main (hom | emb | mat)" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "hom") {
        if (argc < 4) {
            std::cerr << "Usage: ./main hom <H> <G> [--cfi] [--time]";
            return 1;
        }
        const Flags flags = processFlags(argc, argv, 4);

        std::string H = argv[2];
        std::string G = argv[3];

        // Debugging output (or actual functionality based on flags)
        std::cout << "Command: " << command << std::endl;
        std::cout << "H: " << H << std::endl;
        std::cout << "G: " << G << std::endl;
        std::cout << "Option -cfi: " << (flags.cfi ? "enabled" : "disabled") << std::endl;
        std::cout << "Option -time: " << (flags.time ? "enabled" : "disabled") << std::endl;
    }
    if (command == "emb") {
        if (argc < 4) {
            std::cerr << "Usage: ./main emb <H> <G> [--spasmFile <spasmFile>] [--cfi] [--time]";
            return 1;
        }
        const Flags flags = processFlags(argc, argv, 4);
    }
    if (command == "mat") {
        if (argc < 4) {
            std::cerr << "Usage: ./main mat <k> <G> [--cfi] [--time]";
            return 1;
        }
        const Flags flags = processFlags(argc, argv, 4);
    }

    return 0;
}
