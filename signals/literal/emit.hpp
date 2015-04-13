#pragma once

#include "signal.hpp"

namespace signals {
    namespace literal {

        template <class LiteralT, LiteralT literal, class... Args>
        void emit(const Args &... args) {
            signal<LiteralT, literal, Args...>::emit(args...);
        }

    }
}
