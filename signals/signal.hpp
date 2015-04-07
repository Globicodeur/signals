#pragma once

#include <functional>
#include <vector>
#include <utility>

namespace signals {

    template <class... Args>
    class signal {

        using slot_t = std::function<void (Args...)>;
        using slots_t = std::vector<slot_t>;

    public:

        template <class Slot>
        friend void operator>>(signal & signal, Slot slot) {
            signal.slots.emplace_back(std::move(slot));
        }

        void operator()(Args &&... args) const {
            for (const slot_t & slot: slots)
                slot(std::forward<Args>(args)...);
        }

    private:

        slots_t slots;

    };

}
