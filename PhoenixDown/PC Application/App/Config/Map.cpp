#include <fstream>
#include "App/Config/Map.h"

namespace
{
std::map<std::string, std::string>
LoadMap()
{
    const std::string fileName{ "Config.ini" };
    std::ifstream is{ fileName };

    if (!is.is_open())
    {
        throw Exception::RuntimeError{ Config::MapError::fileOpenFailed, fileName };
    }

    std::map<std::string, std::string> map;
    std::string line;
    while (getline(is, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::istringstream iss{ line };
        std::string key;
        std::string value;
        if (!(iss >> key >> value))
        {
            throw Exception::RuntimeError{ Config::MapError::fileFormatError, fileName };
        }

        map[key] = value;
    }

    return map;
}
}

namespace Config
{
Map::Map() :
    mMap{ LoadMap() }
{
}
}
