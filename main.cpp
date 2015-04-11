#include <iostream>

#include "signals/signal.hpp"

static void print_hello_world() {
    std::cout << "Hello World!\n";
}

static void hello_world() {
    using my_signal_t = signals::signal<>;

    struct {
        void operator()() { std::cout << "Hello World!\n"; }
    } hello_printer;

    my_signal_t sig;

    sig >> print_hello_world;
    sig >> std::bind(printf, "Hello World!\n");
    sig >> [] { std::cout << "Hello World!\n"; };
    sig >> hello_printer;

    sig();
}

// =============================================================================

static void something_else() {
    struct Miner {

        Miner(std::string name):
            name(name)
        { }

        std::string name;
        signals::signal<std::string, int> dug_something;

    };

    struct Supervisor {

        Supervisor(Miner & miner) {
            miner.dug_something >> [&](auto type, auto quantity) {
                std::cout
                    << miner.name
                    << " found " << quantity
                    << " of " << type
                    << std::endl;
            };
        }

    };

    Miner joe { "Joe" };
    Supervisor jack { joe };

    joe.dug_something("Gold", 42);
}

// =============================================================================
#include "signals/named/signal.hpp"
#include "signals/named/on.hpp"
#include "signals/named/emit.hpp"

using namespace std::string_literals;

namespace spec {
    constexpr char sig1[] = "sig1";
    constexpr char sig2[] = "sig2";
}

static void named_signals() {

    struct Receiver {
        Receiver() {

            signals::named::on<spec::sig1>([] {
                std::cout << spec::sig1 << " received\n";
            });

            signals::named::on<spec::sig2, int, std::string>(
                [](auto a, auto b) {
                    std::cout << spec::sig2 << " received with "
                              << a << " and " << b << "\n";
                }
            );

            signals::named::on_<spec::sig2>(
                [](int a, const std::string & b) {
                    std::cout << spec::sig2 << " received with "
                              << a << " and " << b << "\n";
                }
            );

        }
    };

    Receiver r1, r2;

    signals::named::emit<spec::sig1>();
    signals::named::emit<spec::sig2>(42, "Hello"s);
}

// =============================================================================

int main() {
    hello_world();
    something_else();

    named_signals();

    return 0;
}
