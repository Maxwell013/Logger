#pragma once

#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <mutex>

#define FLAG(x) (1 << x)

enum class Flag {
    TIMESTAMPSPREFIX    = FLAG(0),
    LOGTYPESPREFIX      = FLAG(1),
    WHITESPACEPREFIX    = FLAG(2),
    LOGTAGPREFIX        = FLAG(3),
    LOGTYPECOLORS       = FLAG(4),
    LOGTYPEFILTER       = FLAG(5),
    LOGTAGFILTER        = FLAG(6),
    WHITELISTFILTER     = FLAG(7),
    ENDOFLINESUFFIX     = FLAG(8),
};

inline Flag operator|(Flag p_flag1, Flag p_flag2) { return (Flag)((int)p_flag1 | (int)p_flag2); }

inline int operator|(int p_int, Flag p_flag) { return p_int | (int)p_flag; }

inline int operator~(Flag p_flag) { return ~(int)p_flag; }

inline int operator&(int p_int, Flag p_flag) { return p_int & (int)p_flag; }

inline const int DEFAULTFLAGS = (int)   Flag::TIMESTAMPSPREFIX  | Flag::LOGTYPESPREFIX  | Flag::WHITESPACEPREFIX | // Prefix
                                        Flag::LOGTYPECOLORS     | Flag::WHITELISTFILTER |                          // Display
                                        Flag::ENDOFLINESUFFIX;                                                     // Suffix

enum LogType { Trace, Debug, Info, Warning, Error, Fatal, LogTypeCount};

inline std::ostream& operator<<(std::ostream& p_ostream, LogType p_logType) {
    switch (p_logType) {
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
    default:
        break;
    }

    return p_ostream;
}


class LogTag {

private:
    std::string m_name;
    unsigned char m_id;

    inline static unsigned char s_nextId = 0;
    static LogTag s_noTag;

public:

    LogTag(std::string p_name) : m_name(p_name), m_id(s_nextId++) { if (s_nextId == 0) { s_nextId++; } }

    std::string getName() { return m_name; }

    unsigned char getId() { return m_id; }

    static LogTag getNoTag() { return s_noTag; }
};

inline std::ostream& operator<<(std::ostream& p_ostream, LogTag p_logTag) { return p_ostream << "[" << p_logTag.getName() << "]"; }

inline bool operator==(LogTag p_logTag1, LogTag p_logTag2) { return p_logTag1.getId() == p_logTag2.getId(); }


class Logger {

private:

    inline static std::mutex s_mutex;
    inline static std::ostream* s_ostream = &std::cout;
    inline static int s_flags = DEFAULTFLAGS;
    inline static bool s_logTypeFilter[LogTypeCount] = { false };
    inline static bool s_logTagFilter[UCHAR_MAX+1] = { false };

    static bool isFlagSet(Flag p_flag) { return (0 != (s_flags & p_flag)); }

    static std::string printPrefix(LogType p_logType, LogTag p_logTag) {
        std::ostringstream stream;

        // Color
        if (isFlagSet(Flag::LOGTYPECOLORS) && s_ostream == &std::cout) {
            stream << printLogTypeColor(p_logType);
        }

        // Time
        if (isFlagSet(Flag::TIMESTAMPSPREFIX)) {
            std::time_t time = std::time(nullptr);
            char timeString[12];
            std::strftime(timeString, 12, "[%T]", std::gmtime(&time));
            stream << timeString;
        }

        // Log type
        if (isFlagSet(Flag::LOGTYPESPREFIX)) {
            stream << p_logType;
        }

        //Log tag
        if (isFlagSet(Flag::LOGTAGPREFIX)) {
            stream << p_logTag;
        }

        // Whitespace
        if (isFlagSet(Flag::WHITESPACEPREFIX)) {
            stream << '\t';
        }

        return stream.str();
    }

    static std::string printSuffix() {
        std::ostringstream stream;

        // Color
        if (isFlagSet(Flag::LOGTYPECOLORS) && s_ostream == &std::cout) {
            stream << endLogTypeColor();
        }

        // End of line
        if (isFlagSet(Flag::ENDOFLINESUFFIX)) {
            stream << std::endl;
        }

        return stream.str();
    }

    // Last argument + suffix
    template<typename T>
    static std::string print(const T p_arg) {
        std::ostringstream stream;
        stream << p_arg;
        stream << printSuffix();
        return stream.str();
    }

    // Recursive call (multiple arguments)
    template<typename T, typename... Ts>
    static std::string print(const T p_arg, const Ts... p_args) {
        std::ostringstream stream;
        stream << p_arg << print(p_args...);
        return stream.str();
    }

    // Prefix + single argument + suffix (logTag unspecified)
    template<typename T>
    static void print(const LogType p_logType, const T p_arg) {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::ostringstream stream;
        stream << printPrefix(p_logType, LogTag::getNoTag());
        stream << p_arg;
        stream << printSuffix();
        log(p_logType, LogTag::getNoTag(), stream.str());
    }

    // Prefix + recursive call (multiple arguments) (logTag unspecified)
    template<typename T, typename... Ts>
    static void print(const LogType p_logType, const T p_arg, const Ts... p_args) {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::ostringstream stream;
        stream << printPrefix(p_logType, LogTag::getNoTag());
        stream << p_arg << print(p_args...);
        log(p_logType, LogTag::getNoTag(), stream.str());
    }
    // Prefix + single argument + suffix (logTag specified)
    template<typename T>
    static void print(const LogType p_logType, const LogTag p_logTag, const T p_arg) {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::ostringstream stream;
        stream << printPrefix(p_logType, p_logTag);
        stream << p_arg;
        stream << printSuffix();
        log(p_logType, p_logTag, stream.str());
    }

    // Prefix + recursive call (multiple arguments) (logTag specified)
    template<typename T, typename... Ts>
    static void print(const LogType p_logType, const LogTag p_logTag, const T p_arg, const Ts... p_args) {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::ostringstream stream;
        stream << printPrefix(p_logType, p_logTag);
        stream << p_arg << print(p_args...);
        log(p_logType, p_logTag, stream.str());
    }

    static void log(LogType p_logType, LogTag p_logTag, std::string p_message) {
        if (isFlagSet(Flag::LOGTYPEFILTER) && filter(p_logType)) { std::cout<<"Typefilter!\n"; return;}
        if (isFlagSet(Flag::LOGTAGFILTER) && filter(p_logTag)) { std::cout<<"Tagfilter!\n"; return; }
        *s_ostream << p_message;
    }

    static bool filter(LogType p_logType) { // return true if should not be logged
        if (isFlagSet(Flag::WHITELISTFILTER)) { return !s_logTypeFilter[p_logType]; }
        else { return s_logTypeFilter[p_logType]; }
    }

    static bool filter(LogTag p_logTag) { // return true if should not be logged
        if (isFlagSet(Flag::WHITELISTFILTER)) { return !s_logTagFilter[p_logTag.getId()]; }
        else { return s_logTagFilter[p_logTag.getId()]; }
    }

    static std::string printLogTypeColor(LogType p_logType) {
        std::string colorCode;
        switch (p_logType) {
        case Trace:
            colorCode = "\033[0m";
            break;
        case Debug:
            colorCode = "\033[32m";
            break;
        case Info:
            colorCode = "\033[34m";
            break;
        case Warning:
            colorCode = "\033[33m";
            break;
        case Error:
            colorCode = "\033[31m";
            break;
        case Fatal:
            colorCode = "\033[37;41m";
            break;
        default:
            colorCode = "\033[0m";
            break;
        }

        return colorCode;
    }

    static std::string endLogTypeColor() { return "\033[0m"; }

public:

    static void setOutputStream(std::ostream* p_ostream) {

        if (s_ostream != &std::cout && dynamic_cast<std::ofstream*>(s_ostream)) {
            static_cast<std::ofstream*>(s_ostream)->close();
        }
        s_ostream = p_ostream;
    }

    static void setFlag(Flag p_flag) { s_flags = s_flags | p_flag; }

    static void clearFlag(Flag p_flag) { s_flags &= ~p_flag; }

    template<typename... Ts>
    static void trace(const Ts... p_args) { print(Trace, p_args...); }

    template<typename... Ts>
    static void debug(const Ts... p_args) { print(Debug, p_args...); }

    template<typename... Ts>
    static void info(const Ts... p_args) { print(Info, p_args...); }

    template<typename... Ts>
    static void warning(const Ts... p_args) { print(Warning, p_args...); }

    template<typename... Ts>
    static void error(const Ts... p_args) { print(Error, p_args...); }

    template<typename... Ts>
    static void fatal(const Ts... p_args) { print(Fatal, p_args...); }

    static void setFilter(LogType p_logType) { s_logTypeFilter[p_logType] = true; }

    static void setFilter(LogTag p_logTag) { s_logTagFilter[p_logTag.getId()] = true; }

    static void clearFilter(LogType p_logType) { s_logTypeFilter[p_logType] = false; }

    static void clearFilter(LogTag p_logTag) { s_logTagFilter[p_logTag.getId()] = false; }
};

inline LogTag LogTag::s_noTag = LogTag("NONE");
