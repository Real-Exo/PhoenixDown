#ifndef GREATERTHENZEROVALIDATOR_H
#define GREATERTHENZEROVALIDATOR_H

namespace Config
{
constexpr auto greaterThenZeroValidator{ [](int i){ return i > 0; } };
}

#endif //GREATERTHENZEROVALIDATOR_H
