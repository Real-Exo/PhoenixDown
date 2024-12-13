#include <string>
#include <stdexcept>
#include <windows.h>
#include "App/FatalException.h"
#include "Libs/Exception/Exception.h"
#include "Libs/Util/Narrow.h"
#include "Libs/Util/CappedCounter.h"
#include "Libs/Serial/ComPort.h"
#include "Libs/Serial/Protocol.h"
#include "Libs/ICMP/File.h"
#include "Libs/ICMP/IPv4.h"
#include "App/Config/Map.h"
#include "App/MouseSynth/LeftClick.h"
#include "App/Screen/Console.h"

std::string
MakeCappedCounterErrorMessage(Util::CappedCounterError e)
{
    switch(e)
    {
    case Util::CappedCounterError::impossibleValues:
        return "Impossible values for CappedCounter constructor!\n";
    default:
        return "Unhandled CappedCounter error!\n";
    }
}

std::string
MakeComPortErrorMessage(Serial::ComPortError e)
{
    switch(e)
    {
    case Serial::ComPortError::openFailed:
        return "Failed to open COM port!\n";
    case Serial::ComPortError::configureFailed:
        return "Failed to configure COM port!\n";
    case Serial::ComPortError::writeFailed:
        return "Failed to write to COM port!\n";
    case Serial::ComPortError::readFailed:
        return "Failed to read from COM port!";
    default:
        return "Unhandled COM port error!\n";
    }
}

std::string
MakeProtocolErrorMessage(Serial::ProtocolError e)
{
    switch(e)
    {
    case Serial::ProtocolError::checksumWrong:
        return "Invalid checksum while communicating trough COM port!\n";
    case Serial::ProtocolError::responseLengthWrong:
        return "Invalid response length while communicating trough COM port!\n";
    default:
        return "Unhandled COM port protocol error!\n";
    }
}

std::string
MakeICMPFileErrorMessage(ICMP::FileError e)
{
    switch(e)
    {
    case ICMP::FileError::createFailed:
        return "Failed to create ICMP file for ping operations!\n";
    default:
        return "Unhandled ICMP file error!\n";
    }
}

std::string
MakeIPv4ErrorMessage(ICMP::IPv4Error e)
{
    switch(e)
    {
    case ICMP::IPv4Error::conversionFailed:
        return "IPv4 string conversion failed!\n";
    default:
        return "Unhandled IPv4 error!\n";
    }
}

std::string
MakeConfigMapErrorMessage(Config::MapError e)
{
    switch(e)
    {
    case Config::MapError::fileOpenFailed:
        return "Failed to open configuration file!\n";
    case Config::MapError::fileFormatError:
        return "File format error while reading configuration file!\n";
    case Config::MapError::keyMissing:
        return "Missing key value while reading configuration file!\n";
    case Config::MapError::valueConversionFailed:
        return "Type conversion of a value failed while reading configuration file!\n";
    case Config::MapError::invalidValue:
        return "Invalid value while reading configuration file!\n";
    default:
        return "Unhandled error while trying to read configuration file!\n";
    }
}

std::string
MakeMouseSynthLeftClickErrorMessage(MouseSynth::LeftClickError e)
{
    switch(e)
    {
    case MouseSynth::LeftClickError::invalidConfig:
        return "Invalid configuration parameters for mouse synthesizer!\n";
    default:
        return "Unhandled mouse synthesizer error!\n";
    }
}

std::string
MakeConsoleErrorMessage(Screen::ConsoleError e)
{
    switch(e)
    {
    case Screen::ConsoleError::getStdHandleFailed:
        return "Aquiring console handle failed!\n";
    case Screen::ConsoleError::getConsoleScreenBufferInfoFailed:
        return "Aquiring console screen buffer info failed!\n";
    case Screen::ConsoleError::setConsoleCursorInfoFailed:
        return "Setting console cursor info failed!\n";
    case Screen::ConsoleError::getConsoleWindowFailed:
        return "Aquiring console window handle failed!\n";
    case Screen::ConsoleError::getWindowLongFailed:
        return "Aquiring console window information failed!\n";
    case Screen::ConsoleError::setWindowLongFailed:
        return "Setting console window information failed!\n";
    case Screen::ConsoleError::setConsoleScreenBufferSizeFailed:
        return "Setting console screen buffer size failed\n";
    case Screen::ConsoleError::setConsoleModeFailed:
        return "Setting console mode failed\n";
    case Screen::ConsoleError::setConsoleCursorPositionFailed:
        return "Setting console cursor position failed!\n";
    case Screen::ConsoleError::setConsoleTextAttributeFailed:
        return "Setting console text attributes failed!\n";
    case Screen::ConsoleError::setConsoleTitleFailed:
        return "Setting console title failed!\n";
    default:
        return "Unhandled console error!\n";
    }
}

void
FatalException()
{
    std::string message;

    try
    {
        throw;
    }
    catch(const Util::NarrowingError& e)
    {
        message = e.what();
    }
    catch(const Exception::RuntimeError<Util::CappedCounterError>& e)
    {
        message = MakeCappedCounterErrorMessage(e.GetPayload()) + e.what();
    }
    catch(const Exception::RuntimeError<Serial::ComPortError>& e)
    {
        message = MakeComPortErrorMessage(e.GetPayload()) + e.what();
    }
    catch(const Exception::RuntimeError<Serial::ProtocolError>& e)
    {
        message = MakeProtocolErrorMessage(e.GetPayload()) + e.what();
    }
    catch(const Exception::RuntimeError<ICMP::FileError>& e)
    {
        message = MakeICMPFileErrorMessage(e.GetPayload()) + e.what();
    }
    catch(const Exception::RuntimeError<ICMP::IPv4Error>& e)
    {
        message = MakeIPv4ErrorMessage(e.GetPayload()) + e.what();
    }
    catch(const Exception::RuntimeError<Config::MapError>& e)
    {
        message = MakeConfigMapErrorMessage(e.GetPayload()) + e.what();
    }
    catch(const Exception::RuntimeError<MouseSynth::LeftClickError>& e)
    {
        message = MakeMouseSynthLeftClickErrorMessage(e.GetPayload()) + e.what();
    }
    catch(const Exception::RuntimeError<Screen::ConsoleError>& e)
    {
        message = MakeConsoleErrorMessage(e.GetPayload()) + e.what();
    }
    catch(const std::runtime_error& e)
    {
        message = std::string{ "Generic run-time error!\n" } + e.what();
    }
    catch(...)
    {
        message = "Unknown error!\n";
    }

    MessageBoxA(NULL, message.c_str(), "Phoenix Down fatal error!", MB_OK | MB_ICONERROR);
}
