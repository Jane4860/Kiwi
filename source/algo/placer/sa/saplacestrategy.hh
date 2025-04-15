#pragma once

#include "../placestrategy.hh"
#include <std/utility.hh>
#include <std/integer.hh>
#include <std/collection.hh>
#include <random>
#include <cmath>

namespace kiwi::circuit { class TopDieInstance; }

namespace kiwi::algo {

    class SAPlaceStrategy : public PlaceStrategy {
    public:
        SAPlaceStrategy(
            double initial_temp = 1000.0,
            double cooling_rate = 0.95,
            int max_iterations = 10000
        );

        void configure(
            const hardware::Interposer& interposer,
            const circuit::BaseDie& basedie
        ) override;

        virtual auto place(
            hardware::Interposer* interposer,
            circuit::BaseDie* basedie,
            std::Vector<circuit::TopDieInstance>& topdies
        ) const -> void override;

    private:
        auto net_cost(circuit::Net* net) const -> std::i64;
        auto check_placement_legality(const circuit::TopDieInstance* inst) const -> bool;
        auto randomly_choose_swappable_insts(std::Vector<circuit::TopDieInstance>& topdies) const 
            -> std::Tuple<circuit::TopDieInstance*, circuit::TopDieInstance*>;

        const hardware::Interposer* interposer_ = nullptr;
        const circuit::BaseDie* basedie_ = nullptr;
        double initial_temp_;
        double cooling_rate_;
        int max_iterations_;
    };

}
