#include <iostream>
#include "App/Screen/Console.h"
#include "Libs/Exception/Exception.h"

namespace
{
void
ThrowOnError(bool error, Screen::ConsoleError errorType)
{
    if (error)
    {
        throw Exception::RuntimeError{ errorType, std::to_string(GetLastError()) };
    }
}

HANDLE
GetHandle(DWORD n)
{
    const auto handle{ GetStdHandle(n) };
    ThrowOnError(handle == INVALID_HANDLE_VALUE, Screen::ConsoleError::getStdHandleFailed);

    return handle;
}

CONSOLE_SCREEN_BUFFER_INFO
GetScreenBufferInfo(HANDLE handle)
{
    CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;

    const auto result{ GetConsoleScreenBufferInfo(handle, &screenBufferInfo) };
    ThrowOnError(!result, Screen::ConsoleError::getConsoleScreenBufferInfoFailed);

    return screenBufferInfo;
}

void
HideCursor(HANDLE handle)
{
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;

    const auto result{ SetConsoleCursorInfo(handle, &cursorInfo) };
    ThrowOnError(!result, Screen::ConsoleError::setConsoleCursorInfoFailed);
}

void
ConfigureWindow()
{
    const auto windowHandle{ GetConsoleWindow() };
    ThrowOnError(windowHandle == NULL, Screen::ConsoleError::getConsoleWindowFailed);

    const auto windowLong{ GetWindowLong(windowHandle, GWL_STYLE) };
    ThrowOnError(!windowLong, Screen::ConsoleError::getWindowLongFailed);

    /* Igore result. On older windows versions, like xp, this does not work.
     * but it's not really required as resizing the window results in scroll bars.
     * Under win10, where resizing the window breaks the layout, this does work...
    const auto result{ SetWindowLong(windowHandle, GWL_STYLE,
                                     windowLong & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX) };
    ThrowOnError(!result, Screen::ConsoleError::setWindowLongFailed); */
    SetWindowLong(windowHandle, GWL_STYLE, windowLong & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
}

void
ConfigureScreenBuffer(HANDLE handle)
{
    const auto screenBufferInfo{ GetScreenBufferInfo(handle) };
    const SHORT columns = screenBufferInfo.srWindow.Right - screenBufferInfo.srWindow.Left + 1;
    const SHORT rows = screenBufferInfo.srWindow.Bottom - screenBufferInfo.srWindow.Top + 1;

    const auto result{ SetConsoleScreenBufferSize(handle, { columns, rows }) };
    ThrowOnError(!result, Screen::ConsoleError::setConsoleScreenBufferSizeFailed);
}

void
DisableInput()
{
    const auto inputHandle{ GetHandle(STD_INPUT_HANDLE) };

    const auto result{ SetConsoleMode(inputHandle, ENABLE_EXTENDED_FLAGS) };
    ThrowOnError(!result, Screen::ConsoleError::setConsoleModeFailed);
}

void
ResetCursor(HANDLE handle)
{
    const auto result{ SetConsoleCursorPosition(handle, { 0, 0 }) };
    ThrowOnError(!result, Screen::ConsoleError::setConsoleCursorPositionFailed);
}

void
SetTextAttributes(HANDLE handle, uint16_t textAttributes)
{
    const auto result{ SetConsoleTextAttribute(handle, textAttributes) };
    ThrowOnError(!result, Screen::ConsoleError::setConsoleTextAttributeFailed);
}
}

namespace Screen
{
Console::Console() :
    mHandle{ GetHandle(STD_OUTPUT_HANDLE) },
    mOriginalTextAttributes{ GetScreenBufferInfo(mHandle).wAttributes },
    mTextAttributes{ mOriginalTextAttributes }
{
    HideCursor(mHandle);
    ConfigureWindow();
    ConfigureScreenBuffer(mHandle);
    DisableInput();
}

void
Console::SetTitle(const std::string& title) const
{
    const auto result{ SetConsoleTitleA(title.c_str()) };
    ThrowOnError(!result, ConsoleError::setConsoleTitleFailed);
}

void
Console::TerminateLine() const
{
    const auto screenBufferInfo{ GetScreenBufferInfo(mHandle) };
    const auto n{ screenBufferInfo.dwSize.X - screenBufferInfo.dwCursorPosition.X };

    std::cout << std::string(n, ' ');
}

void
Console::TerminateScreen() const
{
    const auto screenBufferInfo{ GetScreenBufferInfo(mHandle) };
    const auto nRows{ screenBufferInfo.dwSize.Y - screenBufferInfo.dwCursorPosition.Y };
    const auto rowLength{ screenBufferInfo.dwSize.X };
    const auto n{ nRows * rowLength - screenBufferInfo.dwCursorPosition.X - 1 };

    std::cout << std::string(n, ' ');
    ResetCursor(mHandle);
}

void
Console::SetForegroundColor(uint8_t color)
{
    color &= 0x0F;
    mTextAttributes &= 0xFFF0;
    mTextAttributes |= color;
    SetTextAttributes(mHandle, mTextAttributes);
}

void
Console::SetBackgroundColor(uint8_t color)
{
    color &= 0x0F;
    mTextAttributes &= 0xFF0F;
    mTextAttributes |= (color << 4);
    SetTextAttributes(mHandle, mTextAttributes);
}

void
Console::RestoreOriginalTextAttributes()
{
    mTextAttributes = mOriginalTextAttributes;
    SetTextAttributes(mHandle, mTextAttributes);
}
}
