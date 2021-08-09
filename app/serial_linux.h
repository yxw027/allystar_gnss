#ifndef SERIAL_LINUX_HPP
#define SERIAL_LINUX_HPP

#include <string>
#include <vector>

std::vector<std::string> list_ports();
int open_serial(std::string const &);

#endif// SERIAL_LINUX_HPP
