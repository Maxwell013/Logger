#pragma once

#include <fstream>
#include <iostream>
#include <sstream>

#define LOGGER_TRACE(...) Logger::Logger::print(Logger::LogType::Trace, __VA_ARGS__)
#define LOGGER_DEBUG(...) Logger::Logger::print(Logger::LogType::Debug, __VA_ARGS__)
#define LOGGER_INFO(...) Logger::Logger::print(Logger::LogType::Info, __VA_ARGS__)
#define LOGGER_WARNING(...) Logger::Logger::print(Logger::LogType::Warning, __VA_ARGS__)
#define LOGGER_ERROR(...) Logger::Logger::print(Logger::LogType::Error, __VA_ARGS__)
#define LOGGER_FATAL(...) Logger::Logger::print(Logger::LogType::Fatal, __VA_ARGS__)

#define LOGGER_SETOUTPUTSTREAM(...) Logger::Logger::setOutputStream(__VA_ARGS__)
#define LOGGER_SETFLAG(...) Logger::Logger::setFlag(__VA_ARGS__)
#define LOGGER_CLEARFLAG(...) Logger::Logger::clearFlag(__VA_ARGS__)
#define LOGGER_SETFILTER(...) Logger::Logger::setFilter(__VA_ARGS__)
#define LOGGER_CLEARFILTER(...) Logger::Logger::clearFilter(__VA_ARGS__)

namespace Logger {

    enum class Flag {
        TIMESTAMPSPREFIX    = 1 << 0,
        LOGTYPESPREFIX      = 1 << 1,
        WHITESPACEPREFIX    = 1 << 2,
        LOGTAGPREFIX        = 1 << 3,
        LOGTYPECOLORS       = 1 << 4,
        LOGTYPEFILTER       = 1 << 5,
        LOGTAGFILTER        = 1 << 6,
        WHITELISTFILTER     = 1 << 7,
        ENDOFLINESUFFIX     = 1 << 8,
    };

    inline const int DEFAULTFLAGS = (int)Flag::TIMESTAMPSPREFIX |
                                    (int)Flag::LOGTYPESPREFIX |
                                    (int)Flag::WHITESPACEPREFIX |
                                    (int)Flag::LOGTYPECOLORS |
                                    (int)Flag::WHITELISTFILTER |
                                    (int)Flag::ENDOFLINESUFFIX;

    enum class LogType { Trace, Debug, Info, Warning, Error, Fatal, LogTypeCount};

    inline std::ostream& operator<<(std::ostream& p_ostream, LogType p_logType) {
        switch (p_logType) {
        case LogType::Trace:
            p_ostream << "[Trace]";
            break;
        case LogType::Debug:
            p_ostream << "[Debug]";
            break;
        case LogType::Info:
            p_ostream << "[Info]";
            break;
        case LogType::Warning:
            p_ostream << "[Warning]";
            break;
        case LogType::Error:
            p_ostream << "[Error]";
            break;
        case LogType::Fatal:
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

        inline static std::ostream* s_ostream = &std::cout;
        inline static int s_flags = DEFAULTFLAGS;
        inline static bool s_logTypeFilter[(int)LogType::LogTypeCount] = { false };
        inline static bool s_logTagFilter[UCHAR_MAX+1] = { false };

        static bool isFlagSet(Flag p_flag) { return (0 != ((int)s_flags & (int)p_flag)); }

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
                stream << "\033[0m";
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

        static void log(LogType p_logType, LogTag p_logTag, std::string p_message) {
            if (isFlagSet(Flag::LOGTYPEFILTER) && filter(p_logType)) { return;}
            if (isFlagSet(Flag::LOGTAGFILTER) && filter(p_logTag)) { return; }
            *s_ostream << p_message;
        }

        static bool filter(LogType p_logType) { // return true if should not be logged
            if (isFlagSet(Flag::WHITELISTFILTER)) { return !s_logTypeFilter[(int)p_logType]; }
            else { return s_logTypeFilter[(int)p_logType]; }
        }

        static bool filter(LogTag p_logTag) { // return true if should not be logged
            if (isFlagSet(Flag::WHITELISTFILTER)) { return !s_logTagFilter[p_logTag.getId()]; }
            else { return s_logTagFilter[p_logTag.getId()]; }
        }

        static std::string printLogTypeColor(LogType p_logType) {
            std::string colorCode;
            switch (p_logType) {
            case LogType::Trace:
                colorCode = "\033[0m";
                break;
            case LogType::Debug:
                colorCode = "\033[32m";
                break;
            case LogType::Info:
                colorCode = "\033[34m";
                break;
            case LogType::Warning:
                colorCode = "\033[33m";
                break;
            case LogType::Error:
                colorCode = "\033[31m";
                break;
            case LogType::Fatal:
                colorCode = "\033[37;41m";
                break;
            default:
                colorCode = "\033[0m";
                break;
            }

            return colorCode;
        }

    public:

        // Prefix + single argument + suffix (logTag unspecified)
        template<typename T>
        static void print(const LogType p_logType, const T p_arg) {
            std::ostringstream stream;
            stream << printPrefix(p_logType, LogTag::getNoTag()) << p_arg << printSuffix();
            log(p_logType, LogTag::getNoTag(), stream.str());
        }

        // Prefix + recursive call (multiple arguments) (logTag unspecified)
        template<typename T, typename... Ts>
        static void print(const LogType p_logType, const T p_arg, const Ts... p_args) {
            std::ostringstream stream;
            stream << printPrefix(p_logType, LogTag::getNoTag()) << p_arg << print(p_args...);
            log(p_logType, LogTag::getNoTag(), stream.str());
        }
        // Prefix + single argument + suffix (logTag specified)
        template<typename T>
        static void print(const LogType p_logType, const LogTag p_logTag, const T p_arg) {
            std::ostringstream stream;
            stream << printPrefix(p_logType, p_logTag) << p_arg << printSuffix();
            log(p_logType, p_logTag, stream.str());
        }

        // Prefix + recursive call (multiple arguments) (logTag specified)
        template<typename T, typename... Ts>
        static void print(const LogType p_logType, const LogTag p_logTag, const T p_arg, const Ts... p_args) {
            std::ostringstream stream;
            stream << printPrefix(p_logType, p_logTag) << p_arg << print(p_args...);
            log(p_logType, p_logTag, stream.str());
        }

        static void setOutputStream(std::ostream* p_ostream) {
            if (s_ostream != &std::cout && dynamic_cast<std::ofstream*>(s_ostream)) { static_cast<std::ofstream*>(s_ostream)->close(); }
            s_ostream = p_ostream;
        }

        static void setFlag(Flag p_flag) { s_flags = (int)s_flags | (int)p_flag; }

        static void clearFlag(Flag p_flag) { s_flags &= ~(int)p_flag; }

        static void setFilter(LogType p_logType) { s_logTypeFilter[(int)p_logType] = true; }

        static void setFilter(LogTag p_logTag) { s_logTagFilter[p_logTag.getId()] = true; }

        static void clearFilter(LogType p_logType) { s_logTypeFilter[(int)p_logType] = false; }

        static void clearFilter(LogTag p_logTag) { s_logTagFilter[p_logTag.getId()] = false; }
    };

    inline LogTag LogTag::s_noTag = LogTag("NONE");
}
