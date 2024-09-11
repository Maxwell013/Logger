#pragma once

#include <iostream>

class Logger {

private:

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
        std::cout << "[Trace] \t" << p_arg;
        print(p_args...);
    }

    template<typename T>
    static void debug(const T p_arg) {
        std::cout << "[Debug] \t" << p_arg << std::endl;
    }

    template<typename T, typename... Args>
    static void debug(const T p_arg, Args... p_args) {
        std::cout << "[Debug] \t" << p_arg;
        print(p_args...);
    }

    template<typename T>
    static void info(const T p_arg) {
        std::cout << "[Info]  \t" << p_arg << std::endl;
    }

    template<typename T, typename... Args>
    static void info(const T p_arg, Args... p_args) {
        std::cout << "[Info]  \t" << p_arg;
        print(p_args...);
    }

    template<typename T>
    static void warning(const T p_arg) {
        std::cout << "[Warning]\t" << p_arg << std::endl;
    }

    template<typename T, typename... Args>
    static void warning(const T p_arg, Args... p_args) {
        std::cout << "[Warning]\t" << p_arg;
        print(p_args...);
    }

    template<typename T>
    static void error(const T p_arg) {
        std::cout << "[Error] \t" << p_arg << std::endl;
    }

    template<typename T, typename... Args>
    static void error(const T p_arg, Args... p_args) {
        std::cout << "[Error] \t" << p_arg;
        print(p_args...);
    }

    template<typename T>
    static void fatal(const T p_arg) {
        std::cout << "[Fatal] \t" << p_arg << std::endl;
    }

    template<typename T, typename... Args>
    static void fatal(const T p_arg, Args... p_args) {
        std::cout << "[Fatal] \t" << p_arg;
        print(p_args...);
    }
};
