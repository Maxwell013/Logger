# Logger
A lightweight logging library for C++

## Version 0.7.0 - Log tags and Filtering
- Major: Development
- Minor: Log tags and Filtering
- Patch: Log tags and Filtering

## Features
- A Logger class to store logging preferences
- Thread safe methods for all log types (Trace, Debug, Info, Warning, Error, Fatal)
- Colors for every log type
- Log tags for additonal specifications
- Highly customizable filtering for both log types and log tags
- Timestamps and log types prefixes for logged messages
- Customizable output streams
- Customizable output prefixes/suffixes using flags
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

For output preferences, Logger uses flags.
Simply use the `Logger::setFlag()` and `Logger::clearFlag()` methods.

Example:
```C++
// test.cpp
#include "Logger.h"

int main() {

    // Note: by default, most flags are set to true! (See flag defaults below)
    Logger::debug("Testing different output flags!");

    // Use the Logger::clearFlags() to set flags to false

    Logger::clearFlag(Flag::TIMESTAMPSPREFIX);
    Logger::debug("Testing different output flags!");

    Logger::clearFlag(Flag::WHITESPACEPREFIX);
    Logger::debug("Testing different output flags!");

    // Use the Logger::setFlags() to set them to true

    Logger::setFlag(Flag::TIMESTAMPSPREFIX);
    Logger::debug("Testing different output flags!");

    Logger::clearFlag(Flag::LOGTYPESPREFIX);
    Logger::debug("Testing different output flags!");

    // Note: multiple flags can be set/cleared like so:
    Logger::setFlag(Flag::WHITESPACEPREFIX | Flag::LOGTYPESPREFIX);
    Logger::debug("Testing different output flags!");

    return 0;
}
```

If you wish to specify further your logging, use the built in log tags!

Example:

```C++
//test.cpp
#include "Logger.h"

int main() {

    // Set the log tag prefix flag
    Logger::setFlag(Flag::LOGTAGPREFIX);

    LogTag testingTag("Testing"); // Create a log tag
    Logger::debug(testingTag, "Testing tag specification!");

    // Note: using multiple log tags or not specifying the log tag as the first argument will yeild unwanted behaviour

    return 0;
```

### Additional information about flags (NAME, default value, behaviour)
- `TIMESTAMPSPREFIX`,  `true`,  Toggles the display of the timestamp
- `LOGTYPESPREFIX`,    `true`,  Toggles the display of the log type
- `WHITESPACEPREFIX`,  `true`,  Toggles the display of whitespace between the prefix and the message
- `LOGTAGPREFIX`,      `false`, Toggles the display of the log tag
- `LOGTYPECOLORS`,     `true`,  Toggles the color of the message based on log type
- `LOGTYPEFILTER`,     `false`, Toggles the filtering of the message based on log type
- `LOGTAGFILTER`,      `false`, Toggles the filtering of the message based on log tag
- `WHITELISTFILTER`,   `true`,  Toggles the use of whitelisting for filtering (filters act as blacklists if this flag is cleared)
- `ENDOFLINESUFFIX`,   `true`,  Toggles the display of a end of line following the message

## Changelog

### Version 0.7.0 - Log tags and Filtering
- Added log tags for additional specification
- Added filtering using both tags and types
- Added flags for tags and filtering
- Changed attribute declaration for code clarity
- Changed the method implementations for code clarity

### Version 0.6.1 - Clear warnings
- Changed all possible ODR violations to proper inline methods/attributes

### Version 0.6.0 - Log type colors
- Added colors for all log types (only for console logging)
- Added an output flag for log type colors
- Added suffix flags (End of line)

### Version 0.5.0 - Output flags
- Added output flags for logging preferences (Timestamps Prefix, Log Type Prefix and Whitespace Prefix)

### Version 0.4.0 - Output streams
- Added support for alternate output streams

### Version 0.3.1 - Refactor and README update
- Added README sections (Features, Usage, Changelog)
- Changed the method implementations for code clarity

### Version 0.3.0 - Timestamps and Refactor
- Added timestamps to log methods
- Changed the method implementations for code clarity

### Version 0.2.0 - Thread safety
- Added lock_guard to all log methods for proper thread safety

### Version 0.1.0 - Basic logging
- Added Logger class
- Added log types (Trace, Debug, Info, Warning, Error, Fatal)
- Added static methods for all log types
