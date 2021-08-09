#include "serial_linux.h"

#include <unistd.h>// close

#include <cstring>
#include <iostream>
#include <string>

int main(int argc, const char *argv[]) {
    if (argc != 2) return 1;
    std::cout << "dev = " << argv[1] << std::endl;
    auto serial = open_serial(argv[1]);
    if (serial < 0) return 1;
    char buffer[256];
    auto ptr = 0;
    while (true) {
        auto n = read(serial, buffer + ptr, sizeof(buffer) - ptr);
        if (n == 0) continue;
        if (n < 0) return 1;
        auto begin = 0;
        const auto end = ptr + n;
        buffer[end] = '\0';
        while (true) {
            for (; begin < end && buffer[begin] != '$'; ++begin)
                ;
            if (begin == end) break;
            auto i = begin;
            for (; i < end && buffer[i] != '\n'; ++i)
                ;
            if (i == end) break;
            buffer[i] = '\0';

            constexpr static auto GNGGA = "$GNGGA,";
            constexpr static auto GNGGA_LEN = std::strlen(GNGGA);
            if (0 == std::strncmp(buffer + begin, GNGGA, GNGGA_LEN))
                std::cout << buffer + begin << std::endl;
            begin = i + 1;
        }
        std::memmove(buffer, buffer + begin, end - begin);
        ptr = end - begin;
    }
    return 0;
}
