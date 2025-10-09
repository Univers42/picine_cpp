#include "Scanner.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cerrno>
#include <cstring>

int main(int argc, char **argv)
{
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <input-file> <pattern> <replacement>\n";
        return 1;
    }

    const char *infile = argv[1];
    std::string pattern = argv[2];
    std::string replacement = argv[3];

    std::ifstream in(infile, std::ios::in | std::ios::binary);
    if (!in) {
        std::cerr << "cannot open input: " << infile << " - " << std::strerror(errno) << "\n";
        return 1;
    }
    std::ofstream out("output", std::ios::out | std::ios::binary | std::ios::trunc);
    if (!out) {
        std::cerr << "cannot open output: output - " << std::strerror(errno) << "\n";
        return 1;
    }

    Scanner scanner(in);

    std::string pending;
    std::size_t P = pattern.size();
    int ch;
    while ((ch = scanner.get()) != -1) {
        pending.push_back(static_cast<char>(ch));

        if (P == 0) {
            scanner.write(pending.data(), pending.size());
            pending.clear();
            continue;
        }

        if (pending.size() >= P &&
            pending.compare(pending.size() - P, P, pattern) == 0)
        {
            if (pending.size() > P)
                scanner.write(pending.data(), pending.size() - P);

            if (!replacement.empty())
                scanner.write(replacement);

            pending.clear();
        } else {
            if (pending.size() > P) {
                scanner.write(&pending[0], 1);
                pending.erase(0, 1);
            }
        }
    }

    if (!pending.empty())
        scanner.write(pending.data(), pending.size());

    scanner.flush(out);
    out.flush();

    return 0;
}