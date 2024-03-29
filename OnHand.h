
#pragma once
#ifndef ONHAND_H
#define ONHAND_H

#include <vector>
#include "Poker.h"

class OnHand {
public:
    std::vector<Poker> porkers;
    OnHand(const std::vector<Poker>& porkers);
    const std::vector<Poker>& getPorkers() const;
    void setPorkers(const std::vector<Poker>& porkers);
};

#endif  // ONHAND_H


