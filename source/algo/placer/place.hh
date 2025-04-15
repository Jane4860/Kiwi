#pragma once

#include <std/memory.hh>
#include <std/collection.hh>
#include "circuit/topdieinstance.hh"
#include "hardware/interposer.hh"
#include "circuit/basedie.hh"

namespace kiwi::hardware { class Interposer; }

namespace kiwi::circuit { class Net; }

namespace kiwi::algo {

    struct PlaceStrategy;

    auto place(
        hardware::Interposer* interposer,
        circuit::BaseDie* basedie, 
        std::Vector<circuit::TopDieInstance>& topdies,
        const PlaceStrategy& strategy
    ) -> void;

}
