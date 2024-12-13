#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>
#include <cstdint>
#include <windows.h>

namespace Screen
{
enum class ConsoleError
{
    getStdHandleFailed,
    getConsoleScreenBufferInfoFailed,
    setConsoleCursorInfoFailed,
    getConsoleWindowFailed,
    getWindowLongFailed,
    setWindowLongFailed,
    setConsoleScreenBufferSizeFailed,
    setConsoleModeFailed,
    setConsoleCursorPositionFailed,
    setConsoleTextAttributeFailed,
    setConsoleTitleFailed
};

class Console
{
public:

    Console();
    Console(const Console&) = delete;
    Console(Console&&) = default;
    Console& operator = (const Console&) = delete;
    Console& operator = (Console&&) = default;

    void
    SetTitle(const std::string& title) const;

    void
    TerminateLine() const;

    void
    TerminateScreen() const;

    void
    SetForegroundColor(uint8_t color);

    void
    SetBackgroundColor(uint8_t color);

    void
    RestoreOriginalTextAttributes();

private:

    HANDLE mHandle;
    uint16_t mOriginalTextAttributes;
    uint16_t mTextAttributes;
};
}

#endif //CONSOLE_H
