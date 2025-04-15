#include "saplacestrategy.hh"
#include "hardware/interposer.hh"
#include "circuit/topdieinstance.hh"
#include "circuit/pin.hh"
#include <random>
#include <cmath>
#include <debug/debug.hh>

namespace kiwi::algo {

    SAPlaceStrategy::SAPlaceStrategy(double initial_temp, double cooling_rate, int max_iterations)
        : initial_temp_(initial_temp), cooling_rate_(cooling_rate), max_iterations_(max_iterations) {}

    void SAPlaceStrategy::configure(
        const hardware::Interposer& interposer,
        const circuit::BaseDie& basedie
    ) {
        interposer_ = &interposer;
        basedie_ = &basedie;
    }

    auto SAPlaceStrategy::place(
        hardware::Interposer* interposer,
        circuit::BaseDie* basedie,
        std::Vector<circuit::TopDieInstance>& topdies
    ) const -> void {
        double current_temp = initial_temp_;
        std::random_device rd;
        std::mt19937 gen(rd());

        for (int i = 0; i < max_iterations_; ++i) {
            auto [inst1, inst2] = randomly_choose_swappable_insts(topdies);
            if (!inst1 || !inst2) continue;

            // 检查物理合法性（如禁止区域）
            if (!interposer_->is_placement_legal(inst1->coord()) || 
                !interposer_->is_placement_legal(inst2->coord())) {
                continue;
            }

            // 成本计算（含硬件约束）
            std::i64 current_cost = net_cost(inst1) + net_cost(inst2);
            std::swap(inst1->coord(), inst2->coord());
            std::i64 new_cost = net_cost(inst1) + net_cost(inst2);
            std::i64 delta_cost = new_cost - current_cost;

            // 退火决策
            if (!decide_to_swap(current_temp, delta_cost)) {
                std::swap(inst1->coord(), inst2->coord());
            }

            current_temp *= cooling_rate_;

            // 调试日志
            debug::info_fmt("Iter: {}, Temp: {:.2f}, ΔCost: {}", i, current_temp, delta_cost);
        }
    }

    auto SAPlaceStrategy::net_cost(circuit::Net* net) const -> std::i64 {
        std::i64 cost = 0;
        for (auto* bump : net->bumps()) {
            // 考虑布线层权重（例如：高层金属成本更低）
            auto layer_weight = interposer_->get_layer_weight(bump->layer());
            cost += layer_weight * (std::abs(bump->coord().x) + std::abs(bump->coord().y));
        }
        return cost;
    }

    auto SAPlaceStrategy::check_placement_legality(const circuit::TopDieInstance* inst) const -> bool {
        // 跳过固定引脚（VDD/GND）
        for (const auto& pin : inst->pins()) {
            if (pin.is_fixed()) return false;
        }
        return true;
    }

    auto SAPlaceStrategy::randomly_choose_swappable_insts(
        std::Vector<circuit::TopDieInstance>& topdies
    ) const -> std::Tuple<circuit::TopDieInstance*, circuit::TopDieInstance*> {
        std::vector<circuit::TopDieInstance*> swappable;
        for (auto& inst : topdies) {
            if (check_placement_legality(&inst)) swappable.push_back(&inst);
        }

        if (swappable.size() < 2) return {nullptr, nullptr};

        std::shuffle(swappable.begin(), swappable.end(), std::mt19937{std::random_device{}()});
        return {swappable[0], swappable[1]};
    }

}
