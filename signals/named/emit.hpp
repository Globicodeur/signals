#pragma once

#include "signal.hpp"

namespace signals {
    namespace named {

        template <const char * name, class... Args>
        void emit(Args &&... args) {
            signal<name, Args...>::emit(std::forward<Args>(args)...);
        }

    }
}
