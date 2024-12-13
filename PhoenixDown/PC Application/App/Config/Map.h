#ifndef MAP_H
#define MAP_H

#include <map>
#include <string>
#include <sstream>
#include "Libs/Exception/Exception.h"

namespace Config
{
enum class MapError
{
    fileOpenFailed,
    fileFormatError,
    keyMissing,
    valueConversionFailed,
    invalidValue
};

class Map
{
public:

    Map();

    template <class T, class Validator>
    T
    ExtractValue(const std::string& key, const Validator& validator) const;

private:

    std::map<std::string, std::string> mMap;
};

template <class T, class Validator>
T
Map::ExtractValue(const std::string& key, const Validator& validator) const
{
    const auto it{ mMap.find(key) };
    if (it == mMap.end())
    {
        throw Exception::RuntimeError{ MapError::keyMissing, key };
    }

    T t;
    std::istringstream iss{ it->second };
    if (!(iss >> t))
    {
        throw Exception::RuntimeError{ MapError::valueConversionFailed, key + ' ' + it->second };
    }

    if (!validator(t))
    {
        throw Exception::RuntimeError{ MapError::invalidValue, key + ' ' + it->second };
    }

    return t;
}
}

#endif //MAP_H
