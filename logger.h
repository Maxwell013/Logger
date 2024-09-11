#pragma once

#include <iostream>
#include <ostream>
#include <ctime>
#include <mutex>
#include <sstream>
#include <string>

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

    static std::string printPrefix(LogType p_logtype) {
        std::ostringstream stream;
        // Time
        std::time_t time = std::time(nullptr);
        char timeString[12];
        std::strftime(timeString, 12, "[%T]", std::gmtime(&time));
        stream << timeString;

        // Log type
        stream << p_logtype;

        // Whitespace
        stream << '\t';

        return stream.str();
    }

    template<typename T>
    static std::string print(const T p_arg) {
        std::ostringstream stream;
        stream << p_arg << std::endl;
        return stream.str();
    }

    template<typename T, typename... Args>
    static std::string print(const T p_arg, const Args... p_args) {
        std::ostringstream stream;
        stream << p_arg << print(p_args...);
        return stream.str();
    }

    template<typename T>
    static void print(const LogType p_logtype, const T p_arg) {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::ostringstream stream;
        stream << printPrefix(p_logtype);
        stream << p_arg << std::endl;
        std::cout << stream.str();
    }

    template<typename T, typename... Args>
    static void print(const LogType p_logtype, const T p_arg, const Args... p_args) {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::ostringstream stream;
        stream << printPrefix(p_logtype);
        stream << p_arg << print(p_args...);
        std::cout << stream.str();
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
