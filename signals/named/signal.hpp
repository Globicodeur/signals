#pragma once

#include "../signal.hpp"

namespace signals {
    namespace named {

        template <const char * name, class... Args>
        class signal {

        public:

            signal(const signal &) = delete;
            signal & operator=(const signal &) = delete;

            template <class F>
            static void bind(F && f) {
                self().sig_ >> std::forward<F>(f);
            }

            static void emit(Args &&... args) {
                self().sig_(std::forward<Args>(args)...);
            }

        private:

            signal() = default;

            static signal & self() {
                static signal<name, Args...> sig;
                return sig;
            }

            signals::signal<Args...> sig_;

        };

        template <const char * name, class... Args, class F>
        void on(F && f) {
            signal<name, Args...>::bind(std::forward<F>(f));
        }

        template <const char * name, class... Args>
        void emit(Args &&... args) {
            signal<name, Args...>::emit(std::forward<Args>(args)...);
        }

    }
}
