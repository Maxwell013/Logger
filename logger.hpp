#pragma once

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

#define LOGGER_TRACE(...) logger::Logger::log(logger::LogType::Trace, __VA_ARGS__)
#define LOGGER_DEBUG(...) logger::Logger::log(logger::LogType::Debug, __VA_ARGS__)
#define LOGGER_INFO(...) logger::Logger::log(logger::LogType::Info, __VA_ARGS__)
#define LOGGER_WARNING(...) logger::Logger::log(logger::LogType::Warning, __VA_ARGS__)
#define LOGGER_ERROR(...) logger::Logger::log(logger::LogType::Error, __VA_ARGS__)
#define LOGGER_FATAL(...) logger::Logger::log(logger::LogType::Fatal, __VA_ARGS__)

#define LOGGER_SET_OUTPUT_STREAM(...) logger::Logger::setOutputStream(__VA_ARGS__)
#define LOGGER_SET_FLAG(...) logger::Logger::setFlag(__VA_ARGS__)
#define LOGGER_CLEAR_FLAG(...) logger::Logger::clearFlag(__VA_ARGS__)
#define LOGGER_SET_FILTER(...) logger::Logger::setFilter(__VA_ARGS__)
#define LOGGER_CLEAR_FILTER(...) logger::Logger::clearFilter(__VA_ARGS__)

namespace logger {

    enum class Flag {
        TIMESTAMPS_PREFIX   = 1 << 0,
        LOGTYPES_PREFIX     = 1 << 1,
        WHITESPACE_PREFIX   = 1 << 2,
        LOGTAG_PREFIX       = 1 << 3,
        LOGTYPE_COLORS      = 1 << 4,
        LOGTYPE_FILTER      = 1 << 5,
        LOGTAG_FILTER       = 1 << 6,
        WHITELIST_FILTER    = 1 << 7,
        END_OF_LINE_SUFFIX  = 1 << 8,
    };

    inline const int DEFAULT_FLAGS =
        (int)Flag::TIMESTAMPS_PREFIX    |
        (int)Flag::LOGTYPES_PREFIX      |
        (int)Flag::WHITESPACE_PREFIX    |
        (int)Flag::LOGTYPE_COLORS       |
        (int)Flag::WHITELIST_FILTER     |
        (int)Flag::END_OF_LINE_SUFFIX;

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
        const std::string m_name;
        const unsigned char m_id;

        inline static unsigned char s_nextId = 0;
        static LogTag s_noTag;

    public:

        LogTag(std::string p_name) : m_name(p_name), m_id(s_nextId++) { if (s_nextId == 0) s_nextId++; }

        std::string getName() const { return m_name; }

        unsigned char getId() const { return m_id; }

        static LogTag getNoTag() { return s_noTag; }
    };

    inline std::ostream& operator<<(std::ostream& p_ostream, LogTag p_logTag) { return p_ostream << "[" << p_logTag.getName() << "]"; }

    inline bool operator==(LogTag p_logTag1, LogTag p_logTag2) { return p_logTag1.getId() == p_logTag2.getId(); }


    class Logger {

    private:

        inline static std::ostream* s_ostream = &std::cout;
        inline static int s_flags = DEFAULT_FLAGS;
        inline static bool s_logTypeFilter[(int)LogType::LogTypeCount] = { false };
        inline static bool s_logTagFilter[UCHAR_MAX+1] = { false };

        static bool isFlagSet(Flag p_flag) { return (0 != ((int)s_flags & (int)p_flag)); }

        static std::string formatPrefix(LogType p_logType, LogTag p_logTag) {
            std::ostringstream stream;

            if (isFlagSet(Flag::LOGTYPE_COLORS) && s_ostream == &std::cout)
                stream << formatLogTypeColor(p_logType);

            if (isFlagSet(Flag::TIMESTAMPS_PREFIX))
                stream << formatTimestamp();

            if (isFlagSet(Flag::LOGTYPES_PREFIX))
                stream << p_logType;

            if (isFlagSet(Flag::LOGTAG_PREFIX))
                stream << p_logTag;

            if (isFlagSet(Flag::WHITESPACE_PREFIX))
                stream << "    ";

            return stream.str();
        }

        static std::string formatSuffix() {
            std::ostringstream stream;

            if (isFlagSet(Flag::LOGTYPE_COLORS) && s_ostream == &std::cout)
                stream << "\033[0m";

            if (isFlagSet(Flag::END_OF_LINE_SUFFIX))
                stream << std::endl;

            return stream.str();
        }

        template<typename Type>
        static std::string formatContent(const Type p_arg) {
            std::ostringstream stream;
            stream << p_arg;
            stream << formatSuffix();
            return stream.str();
        }

        template<typename Type, typename... Types>
        static std::string formatContent(const Type p_arg, const Types... p_args) {
            std::ostringstream stream;
            stream << p_arg << formatContent(p_args...);
            return stream.str();
        }

        static void logMessage(LogType p_logType, LogTag p_logTag, std::string p_message) {
            if (isFlagSet(Flag::LOGTYPE_FILTER) && filter(p_logType)) return;
            if (isFlagSet(Flag::LOGTAG_FILTER) && filter(p_logTag)) return;
            *s_ostream << p_message;
        }

        static bool filter(LogType p_logType) {
            if (isFlagSet(Flag::WHITELIST_FILTER))
                return !s_logTypeFilter[(int)p_logType];
            else
                return s_logTypeFilter[(int)p_logType];
        }

        static bool filter(LogTag p_logTag) {
            if (isFlagSet(Flag::WHITELIST_FILTER))
                return !s_logTagFilter[p_logTag.getId()];
            else
                return s_logTagFilter[p_logTag.getId()];
        }

        static std::string formatLogTypeColor(LogType p_logType) {
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

        static std::string formatTimestamp() {
            const std::time_t time = std::time(nullptr);
            char buffer[12];
            std::tm bt {};

            #if defined (_WIN32) || defined (WIN32)
                localtime_s(&bt, &time);
            #else
                localtime_r(&time, &bt);
            #endif
            return std::string(buffer, std::strftime(buffer, sizeof(buffer), "[%T]", &bt));
        }

    public:

        template<typename Type>
        static void log(const LogType p_logType, const Type p_arg) {
            std::ostringstream stream;
            stream << formatPrefix(p_logType, LogTag::getNoTag()) << p_arg << formatSuffix();
            logMessage(p_logType, LogTag::getNoTag(), stream.str());
        }

        template<typename Type, typename... Types>
        static void log(const LogType p_logType, const Type p_arg, const Types... p_args) {
            std::ostringstream stream;
            stream << formatPrefix(p_logType, LogTag::getNoTag()) << p_arg << formatContent(p_args...);
            logMessage(p_logType, LogTag::getNoTag(), stream.str());
        }

        template<typename Type>
        static void log(const LogType p_logType, const LogTag p_logTag, const Type p_arg) {
            std::ostringstream stream;
            stream << formatPrefix(p_logType, p_logTag) << p_arg << formatSuffix();
            logMessage(p_logType, p_logTag, stream.str());
        }

        template<typename Type, typename... Types>
        static void log(const LogType p_logType, const LogTag p_logTag, const Type p_arg, const Types... p_args) {
            std::ostringstream stream;
            stream << formatPrefix(p_logType, p_logTag) << p_arg << formatContent(p_args...);
            logMessage(p_logType, p_logTag, stream.str());
        }

        static void setOutputStream(std::ostream* p_ostream) {
            if (s_ostream != &std::cout && dynamic_cast<std::ofstream*>(s_ostream))
                static_cast<std::ofstream*>(s_ostream)->close();
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
} // namespace logger
