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
#include "signals/literal/from.hpp"

using namespace std::string_literals;

namespace spec {
    auto sig1 = "sig1"s;
    auto sig2 = "sig2"s;
}

static void named_signals() {

    using dispatcher_t = signals::literal::from<std::string &>;

    struct Receiver {
        Receiver() {

            dispatcher_t::on<spec::sig1>([] {
                std::cout << spec::sig1 << " received\n";
            });

            dispatcher_t::on<spec::sig2, int, std::string>(
                [](auto a, auto b) {
                    std::cout << spec::sig2 << " received with "
                              << a << " and " << b << "\n";
                }
            );

            dispatcher_t::on_<spec::sig2>(
                [](int a, const std::string & b) {
                    std::cout << spec::sig2 << " received with "
                              << a << " and " << b << "\n";
                }
            );

        }
    };

    Receiver r1, r2;

    dispatcher_t::emit<spec::sig1>();
    dispatcher_t::emit<spec::sig2>(42, "Hello"s);
}

// =============================================================================

enum class Inputs {
    A,
    B,
    C,
    D
};

static void enumerated_signals() {

    using dispatcher_t = signals::literal::from<Inputs>;

    dispatcher_t::on<Inputs::A>([] {
        std::cout << "A\n";
    });

    dispatcher_t::on<Inputs::B, int>([](auto i) {
        std::cout << "B " << i << "\n";
    });

    dispatcher_t::on_<Inputs::C>([](std::string s, int i) {
        std::cout << "C " << s << " " << i << "\n";
    });

    dispatcher_t::emit<Inputs::A>();
    dispatcher_t::emit<Inputs::B>(42);
    dispatcher_t::emit<Inputs::C>("Hello"s, 1337);
    dispatcher_t::emit<Inputs::D>("ignored");
}

// =============================================================================

int main() {
    hello_world();
    something_else();

    named_signals();
    enumerated_signals();

    return 0;
}
