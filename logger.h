#pragma once

#include <fstream>
#include <ios>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <ctime>
#include <mutex>

#define FLAG(x) (1 << x)

enum class Flag {
    TIMESTAMPSPREFIX = FLAG(0),
    LOGTYPESPREFIX = FLAG(1),
    WHITESPACEPREFIX = FLAG(2)
};


Flag operator|(Flag p_flag1, Flag p_flag2) { return (Flag)((int)p_flag1 | (int)p_flag2); }

int operator|(int p_int, Flag p_flag) { return p_int | (int)p_flag; }

int operator~(Flag p_flag) { return ~(int)p_flag; }

int operator&(int p_int, Flag p_flag) { return p_int & (int)p_flag; }

enum LogType { Trace, Debug, Info, Warning, Error, Fatal };

std::ostream& operator<<(std::ostream& p_ostream, LogType p_logtype) {
    switch (p_logtype) {
    case Trace:
    p_ostream << "[Trace]";
    break;
    case Debug:
    p_ostream << "[Debug]";
    break;
    case Info:
    p_ostream << "[Info]";
    break;
    case Warning:
    p_ostream << "[Warning]";
    break;
    case Error:
    p_ostream << "[Error]";
    break;
    case Fatal:
    p_ostream << "[Fatal]";
    break;
    }

    return p_ostream;
}

class Logger {

private:

    static std::mutex s_mutex;
    static std::ostream* s_ostream;
    static int s_flags;

    static bool hasFlag(Flag p_flag) {
        return (0 != (s_flags & p_flag));
    }

    static std::string printPrefix(LogType p_logtype) {
        std::ostringstream stream;

        // Time
        if (hasFlag(Flag::TIMESTAMPSPREFIX)) {
            std::time_t time = std::time(nullptr);
            char timeString[12];
            std::strftime(timeString, 12, "[%T]", std::gmtime(&time));
            stream << timeString;
        }

        // Log type
        if (hasFlag(Flag::LOGTYPESPREFIX)) {
            stream << p_logtype;
        }

        // Whitespace
        if (hasFlag(Flag::WHITESPACEPREFIX)) {
            stream << '\t';
        }

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
        *s_ostream << stream.str();
    }

    template<typename T, typename... Args>
    static void print(const LogType p_logtype, const T p_arg, const Args... p_args) {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::ostringstream stream;
        stream << printPrefix(p_logtype);
        stream << p_arg << print(p_args...);
        *s_ostream << stream.str();
    }

public:

    static void setOutputStream(std::ostream* p_ostream) {

        if (s_ostream != &std::cout && dynamic_cast<std::ofstream*>(s_ostream)) {
            static_cast<std::ofstream*>(s_ostream)->close();
        }
        s_ostream = p_ostream;
    }

    static void setFlag(Flag p_flag) { s_flags = s_flags | p_flag; }

    static void clearFlag(Flag p_flag) { s_flags &= ~p_flag; }

    template<typename T>
    static void trace(const T p_arg) { print(Trace, p_arg); }

    template<typename... Args>
    static void trace(const Args... p_args) { print(Trace, p_args...); }

    template<typename T>
    static void debug(const T p_arg) { print(Debug, p_arg); }

    template<typename... Args>
    static void debug(const Args... p_args) { print(Debug, p_args...); }

    template<typename T>
    static void info(const T p_arg) { print(Info, p_arg); }

    template<typename... Args>
    static void info(const Args... p_args) { print(Info, p_args...); }

    template<typename T>
    static void warning(const T p_arg) { print(Warning, p_arg); }

    template<typename... Args>
    static void warning(const Args... p_args) { print(Warning, p_args...); }

    template<typename T>
    static void error(const T p_arg) { print(Error, p_arg); }

    template<typename... Args>
    static void error(const Args... p_args) { print(Error, p_args...); }

    template<typename T>
    static void fatal(const T p_arg) { print(Fatal, p_arg); }

    template<typename... Args>
    static void fatal(const Args... p_args) { print(Fatal, p_args...); }
};

std::mutex Logger::s_mutex;
std::ostream* Logger::s_ostream = &std::cout;
int Logger::s_flags = (int) Flag::TIMESTAMPSPREFIX | Flag::LOGTYPESPREFIX | Flag::WHITESPACEPREFIX;
