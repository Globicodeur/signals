#pragma once

#include "signal.hpp"
#include "on.hpp"
#include "emit.hpp"

namespace signals {
    namespace literal {

        template <class LiteralT>
        struct from {

            template <LiteralT literal, class... Args, class F>
            static void on(F && f) {
                signals::literal::on<LiteralT, literal, Args...>(
                    std::forward<F>(f)
                );
            }

            template <LiteralT literal, class F>
            static void on_(F && f) {
                signals::literal::on_<LiteralT, literal>(std::forward<F>(f));
            }

            template <LiteralT literal, class... Args>
            static void emit(Args &&... args) {
                signals::literal::emit<LiteralT, literal, Args...>(
                    std::forward<Args>(args)...
                );
            }

        };

    }
}
