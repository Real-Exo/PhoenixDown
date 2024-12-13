#ifndef PINGSTATUS_H
#define PINGSTATUS_H

namespace Network
{
struct PingStatus
{
    bool online;
    int time;
    int failures;
};
}

#endif //PINGSTATUS_H
