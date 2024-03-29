#include "OnHand.h"
#include "Poker.h"

OnHand::OnHand(const std::vector<Poker>& porkers) {
    this->porkers = porkers;
}

const std::vector<Poker>& OnHand::getPorkers() const {
    return porkers;
}

void OnHand::setPorkers(const std::vector<Poker>& porkers) {
    this->porkers = porkers;
}