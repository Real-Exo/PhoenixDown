#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <vector>
#include <cstdint>
#include <optional>

namespace Serial
{
class ComPort;

enum class ProtocolError
{
    checksumWrong,
    responseLengthWrong
};

void
SendCommand(const ComPort& comPort, const std::vector<uint8_t>& inData,
            std::vector<uint8_t>& outData, std::optional<int> responseLength = std::nullopt);
}

#endif //PROTOCOL_H
