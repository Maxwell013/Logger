#pragma once

#include <iostream>
#include <ostream>
#include <ctime>
#include <mutex>

enum LogType {
    Trace, Debug, Info, Warning, Error, Fatal
};

std::ostream& operator<<(std::ostream& p_ostream, LogType p_logtype) {
    switch (p_logtype) {
    case LogType::Trace:
    p_ostream << "[Trace]  ";
    break;
    case LogType::Debug:
    p_ostream << "[Debug]  ";
    break;
    case LogType::Info:
    p_ostream << "[Info]   ";
    break;
    case LogType::Warning:
    p_ostream << "[Warning]";
    break;
    case LogType::Error:
    p_ostream << "[Error]  ";
    break;
    case LogType::Fatal:
    p_ostream << "[Fatal]  ";
    break;
    }

    return p_ostream;
}

class Logger {

private:

    static std::mutex s_mutex;

    static void printPrefix(LogType p_logtype) {

        // Time
        std::time_t time = std::time(nullptr);
        char timeString[80];
        std::strftime(timeString, 80, "[%T]", std::gmtime(&time));
        std::cout << timeString;

        // Log type
        std::cout << p_logtype;

        // Whitespace
        std::cout << '\t';
    }

    template<typename T>
    static void print(const T p_arg) {
        std::cout << p_arg << std::endl;
    }

    template<typename T, typename... Args>
    static void print(const T p_arg, const Args... p_args) {
        std::cout << p_arg;
        print(p_args...);
    }

    template<typename T>
    static void print(const LogType p_logtype, const T p_arg) {
        std::lock_guard<std::mutex> lock(s_mutex);
        printPrefix(p_logtype);
        std::cout << p_arg << std::endl;
    }
    template<typename T, typename... Args>
    static void print(const LogType p_logtype, const T p_arg, const Args... p_args) {
        std::lock_guard<std::mutex> lock(s_mutex);
        printPrefix(p_logtype);
        std::cout << p_arg;
        print(p_args...);
    }

public:

    template<typename T>
    static void trace(const T p_arg) {
        print(LogType::Trace, p_arg);
    }

    template<typename... Args>
    static void trace(const Args... p_args) {
        print(LogType::Trace, p_args...);
    }

    template<typename T>
    static void debug(const T p_arg) {
        print(LogType::Debug, p_arg);
    }

    template<typename... Args>
    static void debug(const Args... p_args) {
        print(LogType::Debug, p_args...);
    }

    template<typename T>
    static void info(const T p_arg) {
        print(LogType::Info, p_arg);
    }

    template<typename... Args>
    static void info(const Args... p_args) {
        print(LogType::Info, p_args...);
    }

    template<typename T>
    static void warning(const T p_arg) {
        print(LogType::Warning, p_arg);
    }

    template<typename... Args>
    static void warning(const Args... p_args) {
        print(LogType::Warning, p_args...);
    }

    template<typename T>
    static void error(const T p_arg) {
        print(LogType::Error, p_arg);
    }

    template<typename... Args>
    static void error(const Args... p_args) {
        print(LogType::Error, p_args...);
    }

    template<typename T>
    static void fatal(const T p_arg) {
        print(LogType::Fatal, p_arg);
    }

    template<typename... Args>
    static void fatal(const Args... p_args) {
        print(LogType::Fatal, p_args...);
    }
};

std::mutex Logger::s_mutex;
