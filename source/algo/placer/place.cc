#include "place.hh"
#include "placestrategy.hh"

namespace kiwi::algo {

    auto place(
        hardware::Interposer* interposer,
        circuit::BaseDie* basedie,
        std::Vector<circuit::TopDieInstance>& topdies,
        const PlaceStrategy& strategy
    ) -> void {
        strategy.configure(*interposer, *basedie); 
        strategy.place(interposer, basedie, topdies);
    }

}
