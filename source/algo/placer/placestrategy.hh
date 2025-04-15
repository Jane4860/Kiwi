#pragma once

#include <std/memory.hh>
#include <std/collection.hh>
#include "hardware/interposer.hh"
#include "circuit/basedie.hh"

namespace kiwi::hardware { class Interposer; }

namespace kiwi::circuit { class TopDieInstance; }

namespace kiwi::algo {

    struct PlaceStrategy {
        virtual void configure(
            const hardware::Interposer& interposer,
            const circuit::BaseDie& basedie
        ) = 0;

        virtual auto place(
            hardware::Interposer* interposer,
            circuit::BaseDie* basedie,
            std::Vector<circuit::TopDieInstance>& topdies
        ) const -> void = 0;
    };

}
