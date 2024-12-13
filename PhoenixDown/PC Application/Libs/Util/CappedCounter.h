#ifndef CAPPEDCOUNTER_H
#define CAPPEDCOUNTER_H

#include <limits>
#include <string>
#include "Libs/Exception/Exception.h"

namespace Util
{
enum class CappedCounterError
{
    impossibleValues
};

template <class T, bool circular = false>
class CappedCounter
{
public:

    CappedCounter() {};

    explicit
    CappedCounter(T initialValue, T maxValue = std::numeric_limits<T>::max()) :
        mInitialValue{ initialValue },
        mMaxValue{ maxValue },
        mValue{ initialValue }
    {
        if (initialValue >= maxValue)
        {
            throw Exception::RuntimeError{ CappedCounterError::impossibleValues,
                                           std::to_string(initialValue) + ">=" +
                                           std::to_string(maxValue) };
        }
    }

    void
    operator ++ ()
    {
        if constexpr (circular)
        {
            if (++mValue >= mMaxValue)
            {
                mValue = mInitialValue;
            }
        }
        else
        {
            if (++mValue >= mMaxValue)
            {
                mValue = mMaxValue;
            }
        }
    }

    T
    operator * () const
    {
        return mValue;
    }

private:

    T mInitialValue{};
    T mMaxValue{ std::numeric_limits<T>::max() };
    T mValue{};
};

template <class T>
using CircularCounter = CappedCounter<T, true>;
}

#endif //CAPPEDCOUNTER_H
