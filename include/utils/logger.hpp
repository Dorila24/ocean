#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <iostream>
#include <string>

template <typename Tag>
class Logger {
public:
    template <typename... Args>
    static void log(Args&&... args) {
        (std::cout << ... << args) << '\n';}};
struct LogTag {};
using Log = Logger<LogTag>;
#endif // LOGGER_HPP