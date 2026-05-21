#pragma once
#include <iostream>
#include <string>
#include "IObject.h"


class playableItem: public IObject {
public:
    virtual void simulatePlayItem() = 0;
    virtual long long calculateTotalStreams() const = 0;
    
    std::string toString() const override = 0;
    
    virtual ~playableItem() noexcept = 0;

};


inline playableItem::~playableItem() {};   