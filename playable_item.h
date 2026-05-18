#pragma once
#include <iostream>

class playableItem {
public:
    virtual void simulatePlayItem() = 0;
    virtual long long calculateTotalStreams() const = 0;
    virtual ~playableItem() = 0;

};


inline playableItem::~playableItem() {};   