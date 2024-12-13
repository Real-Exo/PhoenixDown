#include "Libs/Serial/Protocol.h"
#include "Libs/Serial/ComPort.h"
#include "Libs/Exception/Exception.h"
#include "Libs/Util/Narrow.h"

/* Packet format:
 * <STP>Databytes...<Checksum><ENP>
 * where: - STP = Start of packet
 *        - ENP = End of packet
 *        - DES = Data escape for control characters
 *        - Checksum = One's complement + 1 of the least significant byte of databytes sum */

namespace
{
constexpr uint8_t STP{ 0x55 };
constexpr uint8_t ENP{ 0xAA };
constexpr uint8_t DES{ 0x5A };

void
SendPacket(const Serial::ComPort& comPort, const std::vector<uint8_t>& data)
{
    comPort.Write(STP);

    uint8_t checksum{ 0 };
    for (const auto& byte : data)
    {
        checksum += byte;

        switch(byte)
        {
        case STP:
        case ENP:
        case DES:   comPort.Write(DES);
        default:    break;
        }

        comPort.Write(byte);
    }

    checksum = ~checksum + 1;
    comPort.Write(checksum);
    comPort.Write(ENP);
}

void
ReceivePacket(const Serial::ComPort& comPort, std::vector<uint8_t>& data)
{
    bool started{ false };
    uint8_t checksum{ 0 };

    while (true)
    {
        auto byte{ comPort.Read() };

        switch(byte)
        {
        case STP:   started = true;
                    checksum = 0;
                    data.clear();
                    break;

        case ENP:   if (!started)
                    {
                        break;
                    }
                    if (checksum)
                    {
                        throw Exception::RuntimeError{ Serial::ProtocolError::checksumWrong };
                    }
                    //Remove checksum from data
                    if (!data.empty())
                    {
                        data.pop_back();
                    }
                    return;

        case DES:   byte = comPort.Read();
                    [[fallthrough]];

        default:    if (!started)
                    {
                        break;
                    }
                    checksum += byte;
                    data.push_back(byte);
        }
    }
}

void
FilterExceptions()
{
    try
    {
        throw;
    }
    catch(const Exception::RuntimeError<Serial::ComPortError>& e)
    {
        switch(e.GetPayload())
        {
        case Serial::ComPortError::writeFailed:
        case Serial::ComPortError::readFailed:  return;
        default:                                throw;
        }
    }
    catch(const Exception::RuntimeError<Serial::ProtocolError>& e)
    {
        switch(e.GetPayload())
        {
        case Serial::ProtocolError::checksumWrong:
        case Serial::ProtocolError::responseLengthWrong:    return;
        default:                                            throw;
        }
    }
}
}

namespace Serial
{
void
SendCommand(const ComPort& comPort, const std::vector<uint8_t>& inData,
            std::vector<uint8_t>& outData, std::optional<int> responseLength)
{
    constexpr auto maxTries{ 5 };
    auto tries{ 0 };

    while (true)
    {
        try
        {
            SendPacket(comPort, inData);
            ReceivePacket(comPort, outData);

            if (responseLength)
            {
                if (Util::Narrow<int>(outData.size()) != *responseLength)
                {
                    throw Exception::RuntimeError{ ProtocolError::responseLengthWrong };
                }
            }

            return;
        }
        catch(...)
        {
            FilterExceptions();

            if (++tries >= maxTries)
            {
                throw;
            }
        }
    }
}
}
