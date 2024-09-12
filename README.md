# Logger
A lightweight logging library for C++

## Version 0.4.0 - Output streams
- Major: Development
- Minor: Output streams
- Patch: Output streams

## Features
- A Logger class to store logging preferences
- Thread safe methods for all log types (Trace, Debug, Info, Warning, Error, Fatal)
- Timestamps and log types prefixes for logged messages
- Customizable output streams
- Single file header only implementation

## Usage
To get started with Logger, copy the Logger.h file from the repository and add it to your project.
Include the `"Logger.h"` header files to your cpp files and use any of the logging methods from the example bellow!

Example:
```C++
//test.cpp
#include "Logger.h"

int main() {
    // Logging with different log types
    Logger::trace("Testing TRACE logging!");
    Logger::debug("Testing DEBUG logging!");
    Logger::info("Testing INFO logging!");
    Logger::warning("Testing WARNING logging!");
    Logger::error("Testing ERROR logging!");
    Logger::fatal("Testing FATAL logging!");

    // Logging with multiple arguments to the methods
    Logger::debug("Testing ", "multiple ", "arguments!");
    Logger::debug("Testing ", 2, " different ", "argument types!");
    Logger::debug("Testing multiple different types: ", 100, ' ', 100.00, " ", 0x64, "!");
    // Note: if you wish to use a custom class or type make sure it has the << operator defined!

    return 0;
}
```

That's it! It's as simple as that! Feel free to use this library in your personnal projects!

If you wish to keep your logs in a file rather than outputing to the console, Logger supports alternate output streams.
Simply use the `Logger::setOutputStream()` method.

Example:
```C++
//test.cpp
#include "Logger.h"

#include <fstream>
#include <string>

int main() {

    std::string filePath = "log.txt";
    std::ofstream ofstream(filePath);
    Logger::setOutputStream(&ofstream);

    Logger::debug("Testing alternate output streams!");

    return 0;
}
```

## Changelog

### Version 0.4.0 - Output streams
- Added support for alternate output streams

### Version 0.3.1 - Refactor and README update
- Added README sections (Features, Usage, Changelog)
- Changed the method implementations for code clarity

### Version 0.3 - Timestamps and Refactor
- Added timestamps to log methods
- Changed the method implementations for code clarity

### Version 0.2 - Thread Safety
- Added lock_guard to all log methods for proper thread safety

### Version 0.1 - Basic logging
- Added Logger class
- Added log types (Trace, Debug, Info, Warning, Error, Fatal)
- Added static methods for all log types
