#pragma once

#include <iostream>
#include <mutex>

class Logger {

private:

    static std::mutex s_mutex;

    template<typename T>
    static void print(const T p_arg) {
        std::cout << p_arg << std::endl;
    }

    template<typename T, typename... Args>
    static void print(const T p_arg, Args... p_args) {
        std::cout << p_arg;
        print(p_args...);
    }

public:

    template<typename T>
    static void trace(const T p_arg) {
        std::cout << "[Trace] \t" << p_arg << std::endl;
    }

    template<typename T, typename... Args>
    static void trace(const T p_arg, Args... p_args) {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[Trace] \t" << p_arg;
        print(p_args...);
    }

    template<typename T>
    static void debug(const T p_arg) {
        std::cout << "[Debug] \t" << p_arg << std::endl;
    }

    template<typename T, typename... Args>
    static void debug(const T p_arg, Args... p_args) {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[Debug] \t" << p_arg;
        print(p_args...);
    }

    template<typename T>
    static void info(const T p_arg) {
        std::cout << "[Info]  \t" << p_arg << std::endl;
    }

    template<typename T, typename... Args>
    static void info(const T p_arg, Args... p_args) {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[Info]  \t" << p_arg;
        print(p_args...);
    }

    template<typename T>
    static void warning(const T p_arg) {
        std::cout << "[Warning]\t" << p_arg << std::endl;
    }

    template<typename T, typename... Args>
    static void warning(const T p_arg, Args... p_args) {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[Warning]\t" << p_arg;
        print(p_args...);
    }

    template<typename T>
    static void error(const T p_arg) {
        std::cout << "[Error] \t" << p_arg << std::endl;
    }

    template<typename T, typename... Args>
    static void error(const T p_arg, Args... p_args) {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[Error] \t" << p_arg;
        print(p_args...);
    }

    template<typename T>
    static void fatal(const T p_arg) {
        std::cout << "[Fatal] \t" << p_arg << std::endl;
    }

    template<typename T, typename... Args>
    static void fatal(const T p_arg, Args... p_args) {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << "[Fatal] \t" << p_arg;
        print(p_args...);
    }
};

std::mutex Logger::s_mutex;
