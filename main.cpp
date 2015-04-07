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

int main() {
    hello_world();

    something_else();

    return 0;
}
