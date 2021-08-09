#include "serial_linux.h"

#include <fcntl.h>   // open
#include <sys/file.h>// flock
#include <termios.h> // config
#include <unistd.h>  // close

#include <filesystem>

std::vector<std::string> list_ports() {
    std::vector<std::string> ports;
    for (const auto &entry : std::filesystem::directory_iterator("/sys/class/tty"))
        if (entry.is_directory()) {
            auto name = entry.path().filename().string();
            if (name.starts_with("ttyUSB") || name.starts_with("ttyACM") && name != "ttyACM0")
                ports.emplace_back(std::move(name));
        }
    return ports;
}

int open_serial(std::string const &name) {
    std::filesystem::path path("/dev");
    auto fd = open(path.append(name).c_str(), O_RDWR | O_NOCTTY);
    if (fd < 0) return fd;

    if (flock(fd, LOCK_EX | LOCK_NB)) {
        close(fd);
        return -1;
    }

    // @see https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
    termios tty{};
    cfsetspeed(&tty, B115200);
    tty.c_cflag |= CS8;           // 8 bits per byte
    tty.c_cflag |= CREAD | CLOCAL;// Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_cc[VTIME] = 0;// Wait for up to 100ms, returning as soon as any data is received.
    tty.c_cc[VMIN] = 1;

    if (tcsetattr(fd, TCSAFLUSH, &tty)) {
        close(fd);
        return -1;
    }

    return fd;
}
